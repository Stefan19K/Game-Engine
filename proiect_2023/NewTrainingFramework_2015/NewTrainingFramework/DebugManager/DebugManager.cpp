#include "stdafx.h"
#include "DebugManager.h"
#include "../Utilities/memDbg.h"

using namespace debugManager;

DebugManager* DebugManager::dmInstance = nullptr;

DebugManager* DebugManager::GetInstance()
{
	if (!dmInstance)
		dmInstance = new DebugManager();

	return dmInstance;
}

void DebugManager::Init()
{
	flags = 0;
	bufferLen = 0;
	firstWrite = false;
}

void DebugManager::KeyPress(unsigned char key)
{
	if ((flags & (1 << T)) == 0)
		return;

	if (!firstWrite) {
		firstWrite = true;
		return;
	}

	// Make sure Enter(\n) is represented by the right ascii code.
	if (key != c_enterCode && bufferLen < c_maxBufferSize - 1) {
		this->buffer[this->bufferLen++] = key;
		std::cout << key;
	}
	else {
		key = (char)c_bSlashNCode;
		std::cout << key;
		this->buffer[this->bufferLen] = '\0';

		ParseCommand();
		memset(this->buffer, 0, c_maxBufferSize);
		bufferLen = 0;
	}
}

DebugManager::Flag debugManager::DebugManager::CharToFlag(char c)
{
	if (c == 'S')
		return S;

	if (c == 'H')
		return H;

	if (c == 'N')
		return N;

	if (c == 'C')
		return C;

	if (c == 'O')
		return O;

	return FLAG_COUNT;
}


void DebugManager::SwitchTerminalMode()
{
	int bit = ((flags & (1 << T)) ^ 1) << T;
	switch (bit) {
	case 0:
		flags &= ~(1 << T);
		memset(this->buffer, 0, c_maxBufferSize);
		bufferLen = 0;
		firstWrite = false;
		std::cout << "Terminal Mode OFF.\n";
		break;
	default:
		flags |= (1 << T);
		std::cout << "Terminal Mode ON.\nterminal@mode:>";
		break;
	}
}

void DebugManager::ClearTBit()
{
	flags &= ~(1 << T);
	memset(this->buffer, 0, c_maxBufferSize);
	bufferLen = 0;
	firstWrite = false;
	std::cout << "Terminal Mode OFF.\n";
}

void DebugManager::ParseCommand()
{
	if (!buffer)
		return;

	char* p = strtok(buffer, " ");

	for (int i = 0; i < c_commands.size(); ++i) {
		if (strcmp(c_commands[i].c_str(), p) == 0) {
			DecodeCommand((Command)i);
			return;
		}
	}

	std::cout << "Invalid command. Please use \"MAN\" to see all possible commands.\nterminal@mode:>";
}

void DebugManager::DecodeCommand(Command code)
{
	switch (code)
	{
	case debugManager::DebugManager::MAN:
		Man();
		break;
	case debugManager::DebugManager::SHOW:
		Show();
		break;
	case debugManager::DebugManager::SHOW_ALL:
		ShowAll();
		break;
	case debugManager::DebugManager::HIDE:
		Hide();
		break;
	case debugManager::DebugManager::HIDE_ALL:
		HideAll();
		break;
	case debugManager::DebugManager::EXIT:
		ClearTBit();
		break;
	case debugManager::DebugManager::COMMAND_COUNT:
		break;
	default:
		break;
	}
}

void DebugManager::Man()
{
	std::cout << "This is the official book of the game engine terminal tutorial.\n";
	std::cout << "Commands:\n";

	std::cout << "SHOW -S/H/N/C/O:\n"
		<< "This command sets up the flags entered as parameter\n"
		<< "in order to show different settings in debug mode.\n";
	std::cout << "\t-S : show the objects wired.\n";
	std::cout << "\t-H : show the hitboxes of the objects.\n";
	std::cout << "\t-N : show the normals of the objects.\n";
	std::cout << "\t-C : show the coordinate system of the scene.\n";
	std::cout << "\t-O : show the coordinate system of the objects.\n";

	std::cout << "\n";

	std::cout << "HIDE -S/H/N/C/O:\n"
		<< "This command clears up the flags entered as parameter\n"
		<< "in order to hide different settings in debug mode.\n";
	std::cout << "\t-S : show the objects wired.\n";
	std::cout << "\t-H : show the hitboxes of the objects.\n";
	std::cout << "\t-N : show the normals of the objects.\n";
	std::cout << "\t-C : show the coordinate system of the scene.\n";
	std::cout << "\t-O : show the coordinate system of the objects.\n";

	std::cout << "\n";

	std::cout << "SHOWALL:\n"
		<< "This command sets up all the flags \n"
		<< "in order to show different settings in debug mode.\n";

	std::cout << "\n";

	std::cout << "HIDEALL:\n"
		<< "This command clears up all the flags\n"
		<< "in order to hide different settings in debug mode.\n";

	std::cout << "\n";

	std::cout << "EXIT:\n"
		<< "This command clears up the terminal mode flag\n"
		<< "and exits the terminal mode.\n";

	std::cout << "\nterminal@mode:>";
	
}

void DebugManager::Show()
{
	char* p = strtok(NULL, " ");
	if (p == NULL || strlen(p) < 2)
		return;

	// -67 is the ascii code for '-' in this source code.
	if (p[0] != c_lineCode) {
		std::cout << "Invalid command. Please use \"MAN\" to see all possible commands.\nterminal@mode:>";
		return;
	}

	for (int i = 1; i < strlen(p); ++i) {
		Flag f = CharToFlag(p[i]);
		switch (f)
		{
		case DebugManager::S:
			flags |= (1 << S);
			break;
		case DebugManager::H:
			flags |= (1 << H);
			break;
		case DebugManager::N:
			flags |= (1 << N);
			break;
		case DebugManager::C:
			flags |= (1 << C);
			break;
		case DebugManager::O:
			flags |= (1 << O);
			break;
		case DebugManager::FLAG_COUNT:
			break;
		default:
			break;
		}
	}
}

void DebugManager::ShowAll()
{
	flags |= 1 << S;
	flags |= 1 << H;
	flags |= 1 << N;
	flags |= 1 << C;
	flags |= 1 << O;
}

void DebugManager::Hide()
{
	char* p = strtok(NULL, " ");
	if (p == NULL || strlen(p) < 2)
		return;

	// -67 is the ascii code for '-' in this source code.
	if (p[0] != c_lineCode) {
		std::cout << "Invalid command. Please use \"MAN\" to see all possible commands.\nterminal@mode:>";
		return;
	}

	for (int i = 1; i < strlen(p); ++i) {
		Flag f = CharToFlag(p[i]);
		switch (f)
		{
		case DebugManager::S:
			flags &= ~(1 << S);
			break;
		case DebugManager::H:
			flags &= ~(1 << H);
			break;
		case DebugManager::N:
			flags &= ~(1 << N);
			break;
		case DebugManager::C:
			flags &= ~(1 << C);
			break;
		case DebugManager::O:
			flags &= ~(1 << O);
			break;
		case DebugManager::FLAG_COUNT:
			break;
		default:
			break;
		}
	}
}

void DebugManager::HideAll()
{
	flags &= ~(1 << S);
	flags &= ~(1 << H);
	flags &= ~(1 << N);
	flags &= ~(1 << C);
	flags &= ~(1 << O);
}

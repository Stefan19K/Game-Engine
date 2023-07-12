#pragma once
#include <basetsd.h>
#include <iostream>
#include <vector>

namespace debugManager {
	const int c_maxBufferSize{ 256 };
	const int c_lineCode{ -67 };
	const int c_enterCode{ 13 };
	const int c_bSlashNCode{ 10 };

	class DebugManager {
	private:
		enum Command {
			MAN,
			SHOW,
			SHOW_ALL,
			HIDE,
			HIDE_ALL,
			EXIT,
			COMMAND_COUNT
		};

		const std::vector<std::string> c_commands = {
			"MAN",
			"SHOW",
			"SHOWALL",
			"HIDE",
			"HIDEALL",
			"EXIT"
		};

		enum Flag {
			T,			// Terminal mode on
			S,			// Show wired objects
			H,			// Show hitboxes
			N,			// Show objects normals
			C,			// Show coordinate system
			O,			// Show objects coordinate systems
			FLAG_COUNT
		};

		static DebugManager* dmInstance;
		UINT16 flags;
		int bufferLen;
		char buffer[c_maxBufferSize];
		bool firstWrite;

	public:
		DebugManager() { Init(); }
		static DebugManager* GetInstance();
		void Init(); //initializari - pot fi si in constructor
		void KeyPress(unsigned char key);

		Flag CharToFlag(char c);

		void SwitchTerminalMode();
		void ClearTBit();

		void ParseCommand();
		void DecodeCommand(Command code);

		void Man();
		void Show();
		void ShowAll();
		void Hide();
		void HideAll();

		// Getters
		UINT16		GetFlags()				{ return this->flags; }
		UINT16		GetWiredFlag()			{ return this->flags & (1 << S); }
		UINT16		GetHitboxesFlag()		{ return this->flags & (1 << H); }
		UINT16		GetNormalsFlag()		{ return this->flags & (1 << N); }
		UINT16		GetCoordSysFlag()		{ return this->flags & (1 << C); }
		UINT16		GetObjCoordSysFlag()	{ return this->flags & (1 << O); }
		UINT16		GetTerminalMode()		{ return this->flags & (1 << T); }

	};
}

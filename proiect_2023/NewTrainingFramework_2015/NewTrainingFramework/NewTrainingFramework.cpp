// NewTrainingFramework.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ResourceManager/ResourceManager.h"
#include "SceneManager/SceneManager.h"
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include <iostream>
#include < windowsx.h >
#include <vector>
#include <string.h>

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "../Utilities/memDbg.h"

using namespace resourceManager;
using namespace sceneManager;

constexpr float c_PI{ 3.1415f };
constexpr double c_min_interval{0.05};

GLuint vboId1;
GLuint vboId2;
GLuint vboId3;
GLuint vboId4;
GLuint textureId;
Shaders myShaders;
float angle{ 0.0f };
float step{ c_PI / 10.0f };
Camera cam = Camera();
Matrix matrix;
unsigned int verticesCount{};
unsigned int indicesCount{};
std::vector<Vertex>vertices;
std::vector<unsigned short>indices;
bool wired = false;
GLenum mode = GL_TRIANGLES;
float cntDeltaTime = 0;
float maxLevel = 0.025f;

void Mouse(ESContext* esContext, MouseButton button, MouseEvent event, Vector2 coord);

int Init ( ESContext *esContext )
{
	ResourceManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
	
	return 0;	
}

void Draw ( ESContext *esContext )
{
	SceneManager::GetInstance()->Draw();

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	cntDeltaTime += deltaTime;
	if (cntDeltaTime > maxLevel) {
		SceneManager::GetInstance()->Update(cntDeltaTime);
		cntDeltaTime -= maxLevel;
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	SceneManager::GetInstance()->Key(key, bIsPressed);
}

void Mouse(ESContext* esContext, MouseButton button, MouseEvent event, float x, float y)
{
	switch (button)
	{
	case LEFT:
		if (x < Globals::screenWidth / 2.0f) {
			angle += step;
			if (angle > 2 * c_PI)
				angle -= 2 * c_PI;
			matrix.SetRotationZ(angle);
		}
		else {
			angle -= step;
			if (angle < 0)
				angle += 2 * c_PI;
			matrix.SetRotationZ(angle);
		}
		break;
	case RIGHT:
		break;
	default:
		break;
	}
}

void CleanUp()
{
	/*glDeleteBuffers(1, &vboId1);
	glDeleteBuffers(1, &vboId2);
	glDeleteBuffers(1, &vboId3);
	glDeleteBuffers(1, &vboId4);*/
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}


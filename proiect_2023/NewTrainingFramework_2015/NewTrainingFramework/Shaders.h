#pragma once
#include <vector>
#include <string>
#include "../Utilities/utilities.h"

constexpr GLint c_max_textures{ 5 };
constexpr GLint c_max_lights{ 10 };

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint positionAttribute;
	GLint normAttribute;
	GLint colorAttribute;
	GLint heightUniform;
	GLint uvOffsetUniform;
	GLint modelUniform;
	GLint viewUniform;
	GLint projectionUniform;
	GLint textureAttribute;
	GLint textureAttribute2;

	// Fog data
	GLint minFieldUniform;
	GLint maxFieldUniform;
	GLint fogColorUniform;
	GLint camPosUniform;

	// Fire data
	GLint timeUniform;
	GLint dispMaxUniform;

	// Light data
	GLint ambColorUniform;
	GLint kaUniform;
	GLint lightCountUniform;
	// GLint lightUniforms;

	std::vector<GLint> textureUniforms;

	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};
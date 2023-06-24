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
	
	// attributes
	GLint positionAttribute;
	GLint normAttribute;
	GLint colorAttribute;
	GLint binormAttribute;
	GLint tgtAttribute;
	GLint textureAttribute;
	GLint textureAttribute2;

	// uniforms
	GLint heightUniform;
	GLint uvOffsetUniform;
	GLint modelUniform;
	GLint viewUniform;
	GLint projectionUniform;
	std::vector<GLint> textureUniforms;

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

	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};
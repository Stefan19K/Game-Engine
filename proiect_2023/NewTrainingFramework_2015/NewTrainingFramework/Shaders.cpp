#include <stdafx.h>
#include "Shaders.h"
#include "../Utilities/memDbg.h"

int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	normAttribute = glGetAttribLocation(program, "a_norm");
	colorAttribute = glGetAttribLocation(program, "a_colL");
	heightUniform = glGetUniformLocation(program, "u_height");
	uvOffsetUniform = glGetUniformLocation(program, "u_uv_offset");
	modelUniform = glGetUniformLocation(program, "u_model");
	viewUniform = glGetUniformLocation(program, "u_view");
	projectionUniform = glGetUniformLocation(program, "u_proj");
	textureAttribute = glGetAttribLocation(program, "a_uv");
	textureAttribute2 = glGetAttribLocation(program, "a_uv2");
	binormAttribute = glGetAttribLocation(program, "a_binorm");
	tgtAttribute = glGetAttribLocation(program, "a_tgt");

	minFieldUniform = glGetUniformLocation(program, "u_min_field");
	maxFieldUniform = glGetUniformLocation(program, "u_max_field");
	fogColorUniform = glGetUniformLocation(program, "u_fog_color");
	camPosUniform = glGetUniformLocation(program, "u_cam_pos");

	timeUniform = glGetUniformLocation(program, "u_time");
	dispMaxUniform = glGetUniformLocation(program, "u_dispMax");

	ambColorUniform = glGetUniformLocation(program, "u_amb_color");
	kaUniform = glGetUniformLocation(program, "u_ka");
	lightCountUniform = glGetUniformLocation(program, "u_lightCount");

	std::vector<GLint> aux(c_max_textures);
	textureUniforms = aux;
	for (int i = 0; i < c_max_textures; i++) {
		textureUniforms[i] = glGetUniformLocation(program, (std::string("u_texture_") + std::to_string(i)).c_str());
	}

	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
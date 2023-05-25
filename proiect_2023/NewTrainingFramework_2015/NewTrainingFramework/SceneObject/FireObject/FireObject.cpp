#include "stdafx.h"
#include "FireObject.h"
#include "ResourceManager/ResourceManager.h"
#include "SceneManager/SceneManager.h"

using namespace resourceManager;
using namespace sceneManager;

void FireObject::Draw(Camera* cam)
{
	Model* model = ResourceManager::GetInstance()->LoadModel(modelId);
	Shader* shader = ResourceManager::GetInstance()->LoadShader(shaderId);
	vector<Texture*> textures(textureIds.size());
	for (int i = 0; i < textureIds.size(); i++) {
		textures[i] = ResourceManager::GetInstance()->LoadTexture(textureIds[i]);
	}

	Shaders* shdr = shader->GetShader();

	glUseProgram(shader->GetShader()->program);

	/*if (textureIds.size() > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(texture->GetTextureResource()->type, texture->GetTextureId());
		if (shdr->textureUniforms[0] != -1)
		{
			glUniform1i(shdr->textureUniforms[0], 0);
		}
	}*/
	for (int i = 0; i < textureIds.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(textures[i]->GetTextureResource()->type, textures[i]->GetTextureId());
		if (shdr->textureUniforms[i] != -1)
		{
			glUniform1i(shdr->textureUniforms[i], i);
		}
	}

	GLuint ibold;
	GLuint nrIndices;
	vector<GLuint> indices;

	if (wired) {
		ibold = model->GetWiredIbold();
		nrIndices = model->GetNrWiredIndices();
		indices = model->GetWiredIndices();
	}
	else {
		ibold = model->GetIbold();
		nrIndices = model->GetNrIndices();
		indices = model->GetIndices();
	}

	glBindBuffer(GL_ARRAY_BUFFER, model->GetVbold());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibold);

	if (shdr->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->positionAttribute);
		glVertexAttribPointer(shdr->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (shdr->colorAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->colorAttribute);
		glVertexAttribPointer(shdr->colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	if (shdr->textureAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->textureAttribute);
		glVertexAttribPointer(shdr->textureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 5));
	}

	if (shdr->textureAttribute2 != -1)
	{
		glEnableVertexAttribArray(shdr->textureAttribute2);
		glVertexAttribPointer(shdr->textureAttribute2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 5 + sizeof(Vector2)));
	}

	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)this->matrix.m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0, (GLfloat*)cam->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0, (GLfloat*)cam->GetProjectionMatrix().m);
	}

	if (shdr->minFieldUniform != -1)
	{
		glUniform1f(shdr->minFieldUniform, SceneManager::GetInstance()->GetMinFogView());
	}

	if (shdr->maxFieldUniform != -1)
	{
		glUniform1f(shdr->maxFieldUniform, SceneManager::GetInstance()->GetMaxFogView());
	}

	if (shdr->fogColorUniform != -1)
	{
		Vector3 col = SceneManager::GetInstance()->GetFogColor();
		glUniform3f(shdr->fogColorUniform, col.x, col.y, col.z);
	}

	if (shdr->camPosUniform != -1)
	{
		Vector3 pos = SceneManager::GetInstance()->GetActiveCamera()->GetPosition();
		glUniform3f(shdr->camPosUniform, pos.x, pos.y, pos.z);
	}

	if (shdr->timeUniform != -1)
	{
		glUniform1f(shdr->timeUniform, this->time);
	}

	if (shdr->dispMaxUniform != -1)
	{
		glUniform1f(shdr->dispMaxUniform, this->dispMax);
	}

	GLenum mode = this->wired ? GL_LINES : GL_TRIANGLES;
	glDrawElements(mode, nrIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FireObject::Update(GLfloat deltaTime)
{
	this->time += deltaTime;
	if (this->time > 1.0f)
		this->time -= 1.0f;
}


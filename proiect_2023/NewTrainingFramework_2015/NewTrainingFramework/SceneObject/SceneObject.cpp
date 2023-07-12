#include "stdafx.h"
#include "SceneObject.h"
#include "ResourceManager/ResourceManager.h"
#include "SceneManager/SceneManager.h"
#include "../Utilities/memDbg.h"

using namespace resourceManager;
using namespace sceneManager;

void SceneObject::UpdateTranformationMatrix()
{
	this->matrix = Matrix();
	this->matrix.SetScale(this->scale);
	this->matrix = this->matrix * Matrix().SetRotationX(this->rotation.x);
	this->matrix = this->matrix * Matrix().SetRotationX(this->rotation.y);
	this->matrix = this->matrix * Matrix().SetRotationX(this->rotation.z);
	this->matrix = this->matrix *  Matrix().SetTranslation(this->position);
}

void SceneObject::SortTextures()
{
	sort(textureIds.begin(), textureIds.end());
}

void SceneObject::SendCommonData()
{
	Model* model = ResourceManager::GetInstance()->LoadModel(modelId);
	Shader* shader = ResourceManager::GetInstance()->LoadShader(shaderId);
	vector<Texture*> textures(textureIds.size());
	for (int i = 0; i < textureIds.size(); i++) {
		textures[i] = ResourceManager::GetInstance()->LoadTexture(textureIds[i]);
	}

	//texture->GetTextureResource()->print();
	Shaders* shdr = shader->GetShader();
	/*model->GetModelResource()->print();
	shader->GetShaderResource()->print();*/

	glUseProgram(shader->GetShader()->program);

	size_t size = textureIds.size();

	for (int i = 0; i < size; i++) {
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

	Texture* skyTexture = ResourceManager::GetInstance()->LoadTexture(ResourceManager::GetInstance()->GetSkyTextureId());
	if (skyTexture) {
		glActiveTexture(GL_TEXTURE0 + size);
		glBindTexture(skyTexture->GetTextureResource()->type, skyTexture->GetTextureId());
		if (shdr->textureUniforms[size] != -1)
		{
			glUniform1i(shdr->textureUniforms[size], size);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, model->GetVbold());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibold);

	// attributes
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

	if (shdr->normAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->normAttribute);
		glVertexAttribPointer(shdr->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 2));
	}

	if (shdr->binormAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->binormAttribute);
		glVertexAttribPointer(shdr->binormAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 3));
	}

	if (shdr->tgtAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->tgtAttribute);
		glVertexAttribPointer(shdr->tgtAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 4));
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

	// matrix uniforms
	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)this->matrix.m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0, 
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0, 
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix().m);
	}

	// fog uniforms
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

	// light uniforms
	if (shdr->ambColorUniform != -1)
	{
		Vector3 col = SceneManager::GetInstance()->GetAmbientColor();
		glUniform3f(shdr->ambColorUniform, col.x, col.y, col.z);
	}

	if (shdr->kaUniform != -1)
	{
		glUniform1f(shdr->kaUniform, SceneManager::GetInstance()->GetKa());
	}

	if (shdr->lightCountUniform != -1)
	{
		glUniform1i(shdr->lightCountUniform, lightIds.size());
	}

	for (int i = 0; i < lightIds.size(); i++) {
		Light* light = SceneManager::GetInstance()->GetLight(lightIds[i]);

		if (!light)
			continue;

		string name = string("u_lights[") + to_string(i) + string("].type");
		GLuint location = glGetUniformLocation(shdr->program, name.c_str());
		glUniform1i(location, light->type);

		name = string("u_lights[") + to_string(i) + string("].pos");
		location = glGetUniformLocation(shdr->program, name.c_str());
		glUniform3f(location, light->pos.x, light->pos.y, light->pos.z);

		name = string("u_lights[") + to_string(i) + string("].direction");
		location = glGetUniformLocation(shdr->program, name.c_str());
		glUniform3f(location, light->direction.x, light->direction.y, light->direction.z);

		name = string("u_lights[") + to_string(i) + string("].diffColor");
		location = glGetUniformLocation(shdr->program, name.c_str());
		glUniform3f(location, light->diffColor.x, light->diffColor.y, light->diffColor.z);

		name = string("u_lights[") + to_string(i) + string("].specColor");
		location = glGetUniformLocation(shdr->program, name.c_str());
		glUniform3f(location, light->specColor.x, light->specColor.y, light->specColor.z);

		name = string("u_lights[") + to_string(i) + string("].kd");
		location = glGetUniformLocation(shdr->program, name.c_str());
		glUniform1f(location, light->kd);

		name = string("u_lights[") + to_string(i) + string("].ks");
		location = glGetUniformLocation(shdr->program, name.c_str());
		glUniform1f(location, light->ks);

		name = string("u_lights[") + to_string(i) + string("].shininess");
		location = glGetUniformLocation(shdr->program, name.c_str());
		glUniform1f(location, light->shininess);

		name = string("u_lights[") + to_string(i) + string("].spotAngle");
		location = glGetUniformLocation(shdr->program, name.c_str());
		glUniform1f(location, light->spotAngle);
	}
}

void SceneObject::UpdateLinearTrajectory(float deltaTimeSeconds)
{
	if (traj->iterCount == 0)
		return;

	Vector3 dir;
	
	dir = traj->points[traj->nextPointPos] - traj->points[traj->currPointPos];
	dir = dir.Normalize();
	this->position.x += traj->speed * deltaTimeSeconds * dir.x;
	this->position.y += traj->speed * deltaTimeSeconds * dir.y;
	this->position.z += traj->speed * deltaTimeSeconds * dir.z;

	if ((this->position - traj->points[traj->currPointPos]).Length() > (traj->points[traj->currPointPos] - traj->points[traj->nextPointPos]).Length()) {
		if (traj->dirType == ALTERN) {
			Vector3 aux = traj->points[traj->currPointPos];
			traj->points[traj->currPointPos] = traj->points[traj->nextPointPos];
			traj->points[traj->nextPointPos] = aux;
		}

		if (traj->dirType == NORMAL) {
			if (traj->iterCount > 0)
				traj->iterCount--;

			if (traj->iterCount != 0)
				this->position = traj->points[traj->currPointPos];
		}
	}

	this->UpdateTranformationMatrix();
}

void SceneObject::UpdateLineStripTrajectory(float deltaTimeSeconds)
{
	if (traj->iterCount == 0)
		return;

	Vector3 dir;

	dir = traj->points[traj->nextPointPos] - traj->points[traj->currPointPos];
	dir = dir.Normalize();
	this->position.x += traj->speed * deltaTimeSeconds * dir.x;
	this->position.y += traj->speed * deltaTimeSeconds * dir.y;
	this->position.z += traj->speed * deltaTimeSeconds * dir.z;

	if ((this->position - traj->points[traj->currPointPos]).Length() > (traj->points[traj->currPointPos] - traj->points[traj->nextPointPos]).Length()) {
		this->position = traj->points[traj->nextPointPos];
		if (traj->nextPointPos != traj->points.size() - 1) {
			traj->currPointPos++;
			traj->nextPointPos++;
		}
		else {
			if (traj->dirType == ALTERN) {
				for (int i = 0; i < traj->points.size() / 2; ++i) {
					Vector3 aux = traj->points[i];
					traj->points[i] = traj->points[traj->points.size() - 1 - i];
					traj->points[traj->points.size() - 1 - i] = aux;
				}
			}

			if (traj->dirType == NORMAL) {
				if (traj->iterCount > 0)
					traj->iterCount--;

				if (traj->iterCount != 0) {
					traj->currPointPos = 0;
					traj->nextPointPos = 1;
					this->position = traj->points[traj->currPointPos];
				}
			}
		}
	}

	this->UpdateTranformationMatrix();
}

void SceneObject::UpdateLineLoopTrajectory(float deltaTimeSeconds)
{
	if (traj->iterCount == 0)
		return;

	Vector3 dir;

	dir = traj->points[traj->nextPointPos] - traj->points[traj->currPointPos];
	dir = dir.Normalize();
	this->position.x += traj->speed * deltaTimeSeconds * dir.x;
	this->position.y += traj->speed * deltaTimeSeconds * dir.y;
	this->position.z += traj->speed * deltaTimeSeconds * dir.z;

	if ((this->position - traj->points[traj->currPointPos]).Length() > (traj->points[traj->currPointPos] - traj->points[traj->nextPointPos]).Length()) {
		this->position = traj->points[traj->nextPointPos];
		traj->currPointPos = (traj->currPointPos + 1) % traj->points.size();
		traj->nextPointPos = (traj->nextPointPos + 1) % traj->points.size();
		if (traj->currPointPos == 0)
			traj->iterCount--;
	}

	this->UpdateTranformationMatrix();
}

void SceneObject::UpdateCircularTrajectory(float deltaTimeSecondsk)
{
}

void SceneObject::Draw(Camera* cam)
{
	SendCommonData();

	// draw mode
	Model* model = ResourceManager::GetInstance()->LoadModel(modelId);
	GLint nrIndices;
	if (wired) {
		nrIndices = model->GetNrWiredIndices();
	}
	else {
		nrIndices = model->GetNrIndices();
	}
	GLenum mode = this->wired ? GL_LINES : GL_TRIANGLES;
	glDrawElements(mode, nrIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SceneObject::DrawDebugMode()
{
	Model* model = ResourceManager::GetInstance()->LoadModel(modelId);
	Shader* shader = ResourceManager::GetInstance()->LoadShader(0);
	Shaders* shdr = shader->GetShader();

	glUseProgram(shader->GetShader()->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->GetVbold());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetWiredIbold());

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

	/*if (shdr->normAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->normAttribute);
		glVertexAttribPointer(shdr->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 2));
	}*/

	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)this->matrix.m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0, 
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0, 
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix().m);
	}

	glDrawElements(GL_LINES, model->GetNrWiredIndices(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// hitbox
	glBindBuffer(GL_ARRAY_BUFFER, model->GetHitbox()->vbold);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetHitbox()->ibold);

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

	/*if (shdr->normAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->normAttribute);
		glVertexAttribPointer(shdr->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 2));
	}*/

	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)this->matrix.m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix().m);
	}

	glDrawElements(GL_LINES, model->GetHitbox()->nrIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normals
	glBindBuffer(GL_ARRAY_BUFFER, model->GetNormal()->vbold);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetNormal()->ibold);

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

	/*if (shdr->normAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->normAttribute);
		glVertexAttribPointer(shdr->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 2));
	}*/

	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)this->matrix.m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix().m);
	}

	glDrawElements(GL_LINES, model->GetNormal()->nrIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::DrawWired()
{
	Model* model = ResourceManager::GetInstance()->LoadModel(modelId);
	Shader* shader = ResourceManager::GetInstance()->LoadShader(0);
	Shaders* shdr = shader->GetShader();

	glUseProgram(shader->GetShader()->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->GetVbold());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetWiredIbold());

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

	/*if (shdr->normAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->normAttribute);
		glVertexAttribPointer(shdr->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 2));
	}*/

	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)this->matrix.m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix().m);
	}

	glDrawElements(GL_LINES, model->GetNrWiredIndices(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::DrawHitboxes()
{
	Model* model = ResourceManager::GetInstance()->LoadModel(modelId);
	Shader* shader = ResourceManager::GetInstance()->LoadShader(0);
	Shaders* shdr = shader->GetShader();

	glUseProgram(shader->GetShader()->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->GetHitbox()->vbold);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetHitbox()->ibold);

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

	/*if (shdr->normAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->normAttribute);
		glVertexAttribPointer(shdr->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 2));
	}*/

	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)this->matrix.m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix().m);
	}

	glDrawElements(GL_LINES, model->GetHitbox()->nrIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::DrawNormals()
{
	Model* model = ResourceManager::GetInstance()->LoadModel(modelId);
	Shader* shader = ResourceManager::GetInstance()->LoadShader(0);
	Shaders* shdr = shader->GetShader();

	glUseProgram(shader->GetShader()->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->GetNormal()->vbold);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetNormal()->ibold);

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

	/*if (shdr->normAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->normAttribute);
		glVertexAttribPointer(shdr->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 2));
	}*/

	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)this->matrix.m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix().m);
	}

	glDrawElements(GL_LINES, model->GetNormal()->nrIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::DrawCoordSystem(CoordSys* coordSys)
{
	Model* model = ResourceManager::GetInstance()->LoadModel(modelId);
	Shader* shader = ResourceManager::GetInstance()->LoadShader(0);
	Shaders* shdr = shader->GetShader();

	glBindBuffer(GL_ARRAY_BUFFER, coordSys->vbold);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coordSys->ibold);

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

	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)this->matrix.m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix().m);
	}

	glDrawElements(GL_LINES, coordSys->nrIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::Update(float deltaTimeSeconds)
{
	// Update the position of the objects that have a trajectory defined
	// in the configuration file.
	if (traj) {
		switch (traj->trajType) {
		case LINEAR:
			UpdateLinearTrajectory(deltaTimeSeconds);
			return;
		case LINE_STRIP:
			UpdateLineStripTrajectory(deltaTimeSeconds);
			return;
		case LINE_LOOP:
			UpdateLineLoopTrajectory(deltaTimeSeconds);
			return;
		case CIRCLE:
			UpdateCircularTrajectory(deltaTimeSeconds);
			return;
		}
	}
}

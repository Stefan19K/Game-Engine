#pragma once
#include "SceneObject/SceneObject.h"

class SkyBoxObject : public SceneObject {
protected:
	GLuint followingCamIdx;
	Vector3 folAxes;
	GLfloat offsetY;

public:
	SkyBoxObject() :
		followingCamIdx(0),
		folAxes(Vector3()),
		offsetY(0.0f),
		SceneObject() {}

	/*SkyBoxObject(
		GLuint _id,
		GLuint _modelId,
		GLuint _shaderId,
		vector<GLuint> _textureIds,
		vector<GLuint> _lightIds,
		string _type,
		string _name,
		Vector3 _position,
		Vector3 _rotation,
		Vector3 _scale,
		bool _depthTest,
		GLuint _folCamId,
		Vector3 _folAxes,
		GLfloat _offsetY
	) :
		followingCamIdx(_folCamId),
		folAxes(_folAxes),
		offsetY(_offsetY),
		SceneObject(_id, _modelId, _shaderId, _textureIds, _lightIds, _type, _name, _position, _rotation, _scale, _depthTest) {}

	SkyBoxObject(
		GLuint _id,
		GLuint _modelId,
		GLuint _shaderId,
		string _type,
		string _name,
		Vector3 _color,
		Vector3 _position,
		Vector3 _rotation,
		Vector3 _scale,
		bool _wired,
		bool _depthTest,
		GLuint _folCamId,
		Vector3 _folAxes,
		GLfloat _offsetY
	) :
		followingCamIdx(_folCamId),
		folAxes(_folAxes),
		offsetY(_offsetY),
		SceneObject(_id, _modelId, _shaderId, _type, _name, _color, _position, _rotation, _scale, _wired, _depthTest) {}*/

	void Draw(Camera* cam);
	void Update(GLfloat deltaTime);

	// Setters
	void SetFolCamID(GLuint id)		{ this->followingCamIdx = id; }
	void SetFolAxes(Vector3 axes)	{ this->folAxes = axes; }
	void SetOffsetY(GLfloat offset) { this->offsetY = offset; }

	// Getters
	GLuint	GetFolCamId()	{ return this->followingCamIdx; }
	GLfloat GetOffsetY()	{ return this->offsetY; }

};
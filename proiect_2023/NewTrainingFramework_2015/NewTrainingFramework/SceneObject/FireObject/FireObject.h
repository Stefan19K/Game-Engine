#pragma once
#include "SceneObject/SceneObject.h"

class FireObject : public SceneObject {
protected:
	GLfloat dispMax;
	GLfloat time;

public:
	FireObject() :
		dispMax(0.0f),
		time(0.0f),
		SceneObject() {}

	FireObject(
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
		GLfloat _dispMax,
		GLfloat _time
	) :
		dispMax(_dispMax),
		time(_time),
		SceneObject(_id, _modelId, _shaderId, _textureIds, _lightIds, _type, _name, _position, _rotation, _scale, _depthTest) {}

	FireObject(
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
		GLfloat _dispMax,
		GLfloat _time
	) :
		dispMax(_dispMax),
		time(_time),
		SceneObject(_id, _modelId, _shaderId, _type, _name, _color, _position, _rotation, _scale, _wired, _depthTest) {}

	void Draw(Camera* cam);
	void Update(GLfloat deltaTime);

	// Setters
	void SetDispMax(GLfloat max)	{ this->dispMax = max; }
	void SetTime(GLfloat time)		{ this->time = time; }
};
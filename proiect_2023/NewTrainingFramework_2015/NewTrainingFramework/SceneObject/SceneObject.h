#pragma once
#include "Resources.h"
#include "Camera.h"
#include <GLES2/gl2.h>
#include <Utilities/Math.h>
#include <string>

class SceneObject {
protected:
	GLuint id;
	GLuint modelId;
	GLuint shaderId;
	vector<GLuint> textureIds;
	vector<GLuint> lightIds;
	string type;
	string name;
	Vector3 color;
	Vector3 hitBoxColor;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Matrix matrix;
	Trajectory* traj;
	bool wired;
	bool depthTest;

	void UpdateLinearTrajectory(float deltaTimeSeconds);
	void UpdateLineStripTrajectory(float deltaTimeSeconds);
	void UpdateLineLoopTrajectory(float deltaTimeSeconds);
	void UpdateCircularTrajectory(float deltaTimeSeconds);
	// Add more trajectory updates when adding new trajectories.

public:
	SceneObject() :
		id(MAXUINT),
		modelId(MAXUINT),
		shaderId(MAXUINT),
		type("normal"),
		name("dummyObject"),
		color(Vector3()),
		hitBoxColor(Vector3()),
		position(Vector3()),
		rotation(Vector3()),
		scale(Vector3(1.0f, 1.0f, 1.0f)),
		traj(nullptr),
		wired(false),
		depthTest(false) 
	{
		UpdateTranformationMatrix();
	}

	SceneObject(
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
		Trajectory* _traj,
		bool _depthTest
	) :
		id(_id),
		modelId(_modelId),
		shaderId(_shaderId),
		textureIds(_textureIds),
		lightIds(_lightIds),
		type(_type),
		name(_name),
		color(Vector3()),
		hitBoxColor(Vector3()),
		position(_position),
		rotation(_rotation),
		scale(_scale),
		traj(_traj),
		wired(false),
		depthTest(_depthTest)
	{
		UpdateTranformationMatrix();
	}

	SceneObject(
		GLuint _id,
		GLuint _modelId,
		GLuint _shaderId,
		string _type,
		string _name,
		Vector3 _color,
		Vector3 _hitBoxColor,
		Vector3 _position,
		Vector3 _rotation,
		Vector3 _scale,
		Trajectory* _traj,
		bool _wired,
		bool _depthTest
	) :
		id(_id),
		modelId(_modelId),
		shaderId(_shaderId),
		type(_type),
		name(_name),
		color(_color),
		hitBoxColor(_hitBoxColor),
		position(_position),
		rotation(_rotation),
		scale(_scale),
		traj(_traj),
		wired(_wired),
		depthTest(_depthTest)
	{
		UpdateTranformationMatrix();
	}

	SceneObject(const SceneObject& obj) :
		id(obj.id),
		modelId(obj.modelId),
		shaderId(),
		textureIds(obj.textureIds),
		type(obj.type),
		name(obj.name),
		color(obj.color),
		hitBoxColor(obj.hitBoxColor),
		position(obj.position),
		rotation(obj.rotation),
		scale(obj.scale),
		traj(obj.traj),
		wired(obj.wired),
		depthTest(obj.depthTest)
	{
		UpdateTranformationMatrix();
	}

	void Draw(Camera* cam);
	void DrawDebugMode();
	void DrawCoordSystem(CoordSys* coordSys);
	void Update(float deltaTimeSeconds);
	void UpdateTranformationMatrix();
	void SortTextures();
	void SendCommonData();

	// Setters
	void SetModelId(GLuint id)				{ this->modelId = id; }
	void SetShaderId(GLuint id)				{ this->shaderId = id; }
	void AddTextureId(GLuint id)			{ this->textureIds.push_back(id); }
	void AddLightId(GLuint id)				{ this->lightIds.push_back(id); }
	void SetType(string& type)				{ this->type = type; }
	void SetName(string& name)				{ this->name = name; }
	void SetColor(Vector3 col)				{ this->color = col; }
	void SetHitBoxColor(Vector3 col)		{ this->hitBoxColor = col; }
	void SetPos(Vector3 pos)				{ this->position = pos; }
	void SetRotation(Vector3 rot)			{ this->rotation = rot; }
	void SetScale(Vector3 scale)			{ this->scale = scale; }
	void SetTrajectory(Trajectory* traj)	{ this->traj = traj; }
	void SetDepthTest(bool val)				{ this->depthTest = val; }
	void SetWired(bool val)					{ this->wired = val; }

	// Getters
	string	GetType()						{ return this->type; }
	GLuint	GetModelId()					{ return this->modelId; }
	Vector3 GetPosition()					{ return this->position; }
	Vector3 GetHitBoxColor()				{ return this->hitBoxColor; }
	Matrix	getTransformationMatrix()		{ return this->matrix; }
};
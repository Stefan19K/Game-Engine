#pragma once
#include "Resources.h"
#include "../Utilities/Math.h"

class Camera {
private:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	GLfloat moveSpeed;
	GLfloat rotateSpeed;
	GLfloat zNear;
	GLfloat zFar;
	GLfloat fov;
	GLfloat deltaTime;
	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;
	Matrix viewMatrix;
	Matrix projMatrix;
	Matrix worldMatrix;

public:
	CoordSys* coordSys;

	Camera() :
		position(Vector3() + Vector3(0.0f, 1.0f, 10.0f)),
		target(Vector3() + Vector3(0.0f, 0.0f, 0.0f)),
		up(Vector3(0.0f, 1.0f, 0.0f)),
		moveSpeed(30.0f),
		rotateSpeed(1.0f),
		zNear(0.1f),
		zFar(2000.0f),
		fov(30.0f),
		projMatrix(Matrix().SetPerspective(30.0f, 1.0f, 0.1f, 500.0f)) {}

	Camera(
		Vector3 _position,
		Vector3 _target,
		Vector3 _up,
		GLfloat _moveSpeed,
		GLfloat _rotateSpeed,
		Matrix _projMatrix
	) :
		position(_position),
		target(_target),
		up(_up),
		moveSpeed(_moveSpeed),
		rotateSpeed(_rotateSpeed),
		projMatrix(_projMatrix)
	{
		UpdateWorldView();
	}

	void UpdateWorldView();
	void UpdateProjMatrix() { this->projMatrix = Matrix().SetPerspective(fov, 1.0f, zNear, zFar); }

	void moveOx(float direction);
	void moveOy(float direction);
	void moveOz(float direction);
	// TODO : Fix camera rotation
	void rotateOX(float direction);
	void rotateOY(float direction);
	void rotateOZ(float direction);
	// TODO : Fix camera rotation

	// Setters
	void SetDeltaTime(float deltaTime);
	void SetPosition(Vector3 pos) { this->position = pos; }
	void SetTarget(Vector3 target) { this->target = target; }
	void SetUp(Vector3 up) { this->up = up; }
	void SetMoveSpeed(GLfloat speed) { this->moveSpeed = speed; }
	void SetRotateSpeed(GLfloat speed) { this->rotateSpeed = speed; }
	void SetZNear(GLfloat zNear) { this->zNear = zNear; }
	void SetZFar(GLfloat zFar) { this->zFar = zFar; }
	void SetFov(GLfloat fov) { this->fov = fov; }

	// Getters
	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix();
	Vector3 GetPosition() { return this->position; }

};

#include "stdafx.h"
#include "Camera.h"
#include "../Utilities/memDbg.h"

void Camera::moveOx(float direction)
{
	Vector3 forward = xAxis * direction;
	Vector3 distance = forward * moveSpeed * deltaTime;
	position += distance;
	target += distance;
	UpdateWorldView();
}

void Camera::moveOy(float direction)
{
	Vector3 distance = Vector3(0.0f, 1.0f, 0.0f) * moveSpeed * direction * deltaTime;
	position += distance;
	target += distance;
	UpdateWorldView();
}

void Camera::moveOz(float direction)
{
	Vector3 forward = -(target - position).Normalize() * direction;
	Vector3 distance = forward * moveSpeed * deltaTime;
	position += distance;
	target += distance;
	UpdateWorldView();
}

void Camera::rotateOX(float direction)
{
	Matrix matrix = Matrix().SetRotationX(rotateSpeed * direction * deltaTime);
	Vector4 rotatedUp = Vector4(up, 1.0f) * matrix;
	Vector4 up_4 = rotatedUp * worldMatrix;
	up = Vector3(up_4.x, up_4.y, up_4.z);
	up = up.Normalize();
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * matrix;
	Vector4 target_4 = rotatedTarget * worldMatrix;
	target = Vector3(target_4.x, target_4.y, target_4.z);
	UpdateWorldView();
}

void Camera::rotateOY(float direction)
{
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Matrix matrix = Matrix().SetRotationY(rotateSpeed * direction * deltaTime);
	Vector4 rotatedTarget = localTarget * matrix;
	Vector4 target_4 = rotatedTarget * worldMatrix;
	target = Vector3(target_4.x, target_4.y, target_4.z);
	UpdateWorldView();
}

void Camera::rotateOZ(float direction)
{
	Matrix matrix = Matrix().SetRotationZ(rotateSpeed * direction * deltaTime);
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * matrix;
	Vector4 target_4 = rotatedTarget * worldMatrix;
	target = Vector3(target_4.x, target_4.y, target_4.z);
	UpdateWorldView();
}

void Camera::UpdateWorldView()
{
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
	Matrix R;
	R.m[0][0] = xAxis.x;  R.m[0][1] = xAxis.y;  R.m[0][2] = xAxis.z;  R.m[0][3] = 0.0f;
	R.m[1][0] = yAxis.x;  R.m[1][1] = yAxis.y;  R.m[1][2] = yAxis.z;  R.m[1][3] = 0.0f;
	R.m[2][0] = zAxis.x;  R.m[2][1] = zAxis.y;  R.m[2][2] = zAxis.z;  R.m[2][3] = 0.0f;
	R.m[3][0] = 0.0f;     R.m[3][1] = 0.0f;     R.m[3][2] = 0.0f;     R.m[3][3] = 1.0f;
	Matrix T;
	T.SetTranslation(Vector3(position.x, position.y, position.z));
	worldMatrix = R * T;

	T.SetTranslation(Vector3(-position.x, -position.y, -position.z));
	R.Transpose();
	viewMatrix = T * R;
}

void Camera::SetDeltaTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}

Matrix Camera::GetViewMatrix()
{
	return this->viewMatrix;
}

Matrix Camera::GetProjectionMatrix()
{
	return this->projMatrix;
}


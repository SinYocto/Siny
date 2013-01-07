#ifndef OBJECT_H
#define OBJECT_H

#include"Math.h"

class Object
{
public:
	Object(Vector3 pos = Vector3(0,0,0), Quaternion rot = Quaternion(0, 0, 0));
	void LookAt(Vector3 lookAtPos);
	void Rotate(float eulerX, float eulerY, float eulerZ);
	void RotateLocal(float eulerX, float eulerY, float eulerZ);
	D3DXMATRIX LocalToWorldMatrix();
	void UpdateLocalAxis();
	void Translate(Vector3 moveVector);
	void TranslateLocal(Vector3 moveVector);
	Vector3 LocalVectorToWorld(Vector3 localVec);

public:
	Vector3 position;
	Quaternion rotation;

	Vector3 right;
	Vector3 up;
	Vector3 forward;
};


#endif
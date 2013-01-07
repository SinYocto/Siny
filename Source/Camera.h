#ifndef CAMERA_H
#define CAMERA_H

#include"Object.h"
#include"Math.h"

class Camera:public Object
{
public:
	Camera(float fov = PI/2, float asp = 1.0f, float nZ = 1.0f, float fZ = 1000.0f, float width = 10.0f);
	D3DXMATRIX ProjMatrix();
	D3DXMATRIX OrthoProjMatrix();
	D3DXMATRIX ViewMatrix();
public:
	float FOV;
	float aspect;
	float nearZ;
	float farZ;
	float orthoWidth;
};

extern Camera mainCamera;



#endif
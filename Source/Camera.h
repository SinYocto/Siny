#ifndef CAMERA_H
#define CAMERA_H

#include"Object.h"
#include"Math.h"
#include"Bound.h"

class Camera:public Object
{
public:
	Camera(float fov = PI/2, float asp = 1.0f, float nZ = 1.0f, float fZ = 1000.0f, float width = 10.0f);
	D3DXMATRIX ProjMatrix();
	D3DXMATRIX OrthoProjMatrix();
	D3DXMATRIX ViewMatrix();

	void ExtractFrustumPlanes();
	bool isVisable(BoundingBox boundingBox);
public:
	float FOV;
	float aspect;
	float nearZ;
	float farZ;
	float orthoWidth;

	D3DXPLANE frustumPlanes[6];

	bool isTranformDirty;
};

extern Camera mainCamera;



#endif
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include"Mesh.h"
#include"Math.h"

class Cube : public Mesh
{
public:
	Cube();
};

class Cylinder : public Mesh
{
public:
	Cylinder() {}
	Cylinder(float topR, float bottomR, float cylinderHeight, int segmentsWidth, int segmentsHeight, D3DXCOLOR cylinderColor);
public:
	float topRadius;
	float bottomRadius;
	float height;
	int segmentsW;
	int segmentsH;

	D3DXCOLOR color;
};

class Sphere : public Mesh
{
public:
	Sphere() {}
	Sphere(float r, int sw, int sh);
public:
	float radius;
	int segmentsW;
	int segmentsH;
};

#endif
#ifndef MATH_H
#define MATH_H

#include"d3dx9.h"
#include<math.h>


#define PI 3.141592654f

class Vector3
{
public:
	Vector3(float vec_x = 0, float vec_y = 0, float vec_z = 0);

	Vector3 operator+(Vector3 vec);
	Vector3 operator-(Vector3 vec);
	friend Vector3 operator*(float value, Vector3 vec);
	bool operator==(Vector3 vec);
	bool operator!=(Vector3 vec);

	float length();
	void normalize();
	Vector3 normalized();
	float Dot(Vector3 vec);
	Vector3 Cross(Vector3 vec);

public:
	float x,y,z;
	static Vector3 Zero;
};

class Vector2
{
public:
	Vector2(float vec_x = 0, float vec_y = 0);
public:
	float x,y;
};

class Quaternion
{
public:
	Quaternion();
	Quaternion(float qw, float qx, float qy, float qz);
	Quaternion(float eulerX, float eulerY, float eulerZ);

	Quaternion(D3DXMATRIX matrix);

	friend Quaternion operator*(Quaternion q1, Quaternion q2);

	friend Vector3 operator*(Vector3 vec, Quaternion q);

	Quaternion Conjugate();

	Vector3 eulerAngle();

	D3DXMATRIX matrix();

public:
	float w,x,y,z;
};

unsigned int factorial(unsigned int value);

#endif
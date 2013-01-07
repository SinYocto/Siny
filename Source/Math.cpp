#include"Math.h"

// Vector3
Vector3 Vector3::Zero = Vector3(0, 0, 0);

Vector3::Vector3(float vec_x, float vec_y, float vec_z)
{
	x = vec_x;
	y = vec_y;
	z = vec_z;
}

Vector3 Vector3::operator+(Vector3 vec) 
{ 
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}


Vector3 Vector3::operator-(Vector3 vec) 
{ 
	return Vector3(x - vec.x, y - vec.y, z - vec.z); 
}


Vector3 operator*(float value, Vector3 vec) 
{
	return Vector3(value*vec.x, value*vec.y, value*vec.z); 
}

bool Vector3::operator==(Vector3 vec) 
{ 
	return x == vec.x && y == vec.y && z == vec.z; 
}

bool Vector3::operator!=(Vector3 vec) 
{ 
	return !(*this == vec); 
}

float Vector3::length() 
{ 
	return sqrt(x*x + y*y + z*z);
}
	
void Vector3::normalize() 
{ 
	float len = this->length(); 
	if(len != 0){
		x /= len; y /= len; z/= len; 
	}
}

Vector3 Vector3::normalized()
{
	Vector3 vec = *this;
	vec.normalize();
	return vec;
}
	
float Vector3::Dot(Vector3 vec) 
{ 
	return x*vec.x + y*vec.y + z*vec.z; 
}
	
Vector3 Vector3::Cross(Vector3 vec) 
{ 
	return Vector3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x); 
}



// Vector2
Vector2::Vector2(float vec_x, float vec_y) 
{ 
	x = vec_x; 
	y = vec_y; 
}



// Quaternion
Quaternion::Quaternion()
{
	w = 1.0f;
	x = 0;
	y = 0;
	z = 0;
}

Quaternion::Quaternion(float qw, float qx, float qy, float qz)
{
	w = qw;
	x = qx;
	y = qy;
	z = qz;
}

// 欧拉角转成四元数
Quaternion::Quaternion(float eulerX, float eulerY, float eulerZ)
{
	float sx,sy,sz,cx,cy,cz;

	sx = sin(eulerX/2);
	cx = cos(eulerX/2);
	sy = sin(eulerY/2);
	cy = cos(eulerY/2);
	sz = sin(eulerZ/2);
	cz = cos(eulerZ/2);

	w = cy * cx * cz + sy * sx * sz;
	x = cy * sx * cz + sy * cx * sz;
	y = sy * cx * cz - cy * sx * sz;
	z = cy * cx * sz - sy * sx * cz;
}

// 矩阵转成四元数
Quaternion::Quaternion(D3DXMATRIX matrix)
{
	float m11 = matrix._11;
	float m12 = matrix._12;
	float m13 = matrix._13;

	float m21 = matrix._21;
	float m22 = matrix._22;
	float m23 = matrix._23;

	float m31 = matrix._31;
	float m32 = matrix._32;
	float m33 = matrix._33;

	float tw = m11 + m22 + m33;
	float tx = m11 - m22 - m33;
	float ty = m22 - m11 - m33;
	float tz = m33 - m11 - m22;

	int biggestIx = 0;
	float tb = tw;
	if(tx > tb){
		tb = tx;
		biggestIx = 1;
	}
	if(ty > tb){
		tb = ty;
		biggestIx = 2;
	}
	if(tz > tb){
		tb = tz;
		biggestIx = 3;
	}

	float biggestVal = sqrt(tb + 1) * 0.5f;
	float mult = 0.25f / biggestVal;

	switch(biggestIx){
	case 0:
		w = biggestVal;
		x = (m23 - m32) * mult;
		y = (m31 - m13) * mult;
		z = (m12 - m21) * mult;
		break;

	case 1:
		x = biggestVal;
		w = (m23 - m32) * mult;
		y = (m12 + m21) * mult;
		z = (m31 + m13) * mult;
		break;

	case 2:
		y = biggestVal;
		w = (m31 - m13) * mult;
		x = (m12 + m21) * mult;
		z = (m23 + m32) * mult;
		break;

	case 3:
		z = biggestVal;
		w = (m12 - m21) * mult;
		x = (m31 + m13) * mult;
		y = (m23 + m32) * mult;
		break;
	}
}

// 四元数乘法
Quaternion operator*(Quaternion q1, Quaternion q2)
{
	Quaternion q;
	q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	q.x = q1.y * q2.z - q1.z * q2.y + q1.w * q2.x + q1.x * q2.w;
	q.y = q1.z * q2.x - q1.x * q2.z + q1.w * q2.y + q1.y * q2.w;
	q.z = q1.x * q2.y - q1.y * q2.x + q1.w * q2.z + q1.z * q2.w;

	return q;
}

// 点或向量乘以四元数（点或向量经四元数变换）
Vector3 operator*(Vector3 vec, Quaternion q)
{
	Quaternion vecQ = Quaternion(0, vec.x, vec.y, vec.z);
	Quaternion resultQ = q * vecQ * q.Conjugate();

	return Vector3(resultQ.x, resultQ.y, resultQ.z);
}

// 四元数的共轭
Quaternion Quaternion::Conjugate()
{
	return Quaternion(w, -x, -y, -z);
}

// 四元数转成欧拉角
Vector3 Quaternion::eulerAngle()
{
	float p,h,b;
	float sp = -2.0f * (y*z + w*x);

	if(fabs(sp) > 0.9999f){
		p = PI/2;
		h = atan2(-x*z + w*y, 0.5f - y*y - z*z);
		b = 0.0f;
	}
	else{
		p = asin(sp);
		h = atan2(x*z + w*y, 0.5f - x*x - y*y);
		b = atan2(x*y + w*z, 0.5f - x*x - z*z);
	}

	return Vector3(p,h,b);
}

// 四元数转成矩阵
D3DXMATRIX Quaternion::matrix()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	mat._11 = 1 - 2*(y*y + z*z);
	mat._12 = 2*(x*y + w*z);
	mat._13 = 2*(x*z - w*y);

	mat._21 = 2*(x*y - w*z);
	mat._22 = 1 - 2*(x*x + z*z);
	mat._23 = 2*(y*z + w*x);

	mat._31 = 2*(x*z + w*y);
	mat._32 = 2*(y*z - w*x);
	mat._33 = 1 - 2*(x*x +y*y);

	return mat;
}

#include"Object.h"
#include"stdio.h"

// Object构造函数，参数指定position和rotation
Object::Object(Vector3 pos, Quaternion rot)
{
	position = pos; 
	rotation = rot;
	UpdateLocalAxis();
}

// LookAt指定position，Up向量为(0, 1, 0)
void Object::LookAt(Vector3 lookAtPos)
{
	// 使用DirectX函数得到矩阵，然后再转换为四元数
	D3DXMATRIX matrix;
	D3DXMatrixLookAtLH(&matrix, &(D3DXVECTOR3(position.x, position.y, position.z)), &(D3DXVECTOR3(lookAtPos.x, lookAtPos.y, lookAtPos.z)), &(D3DXVECTOR3(0, 1, 0)));
	D3DXMatrixTranspose(&matrix, &matrix);
	rotation = Quaternion(matrix);
	UpdateLocalAxis();
}

// 返回本地坐标到世界坐标的转换矩阵
// 由position和rotation计算，不包含scale
D3DXMATRIX Object::LocalToWorldMatrix()
{
	D3DXMATRIX matRot, matTrans;
	D3DXMatrixTranslation(&matTrans, position.x, position.y, position.z);
	matRot = rotation.matrix();

	return  matRot * matTrans;
}

// 在世界坐标系下以欧拉角(eulerX, eulerY, eulerZ)旋转
void Object::Rotate(float eulerX, float eulerY, float eulerZ)
{
	rotation = Quaternion(eulerX, eulerY, eulerZ) * rotation;
	UpdateLocalAxis();
}

// 在本地坐标系下以欧拉角(eulerX, eulerY, eulerZ)旋转
void Object::RotateLocal(float eulerX, float eulerY, float eulerZ)
{
	// 将本地旋转轴变换为世界旋转轴，得到世界下的旋转四元数，再乘以之前的旋转四元数即可
	Quaternion localRotation = Quaternion(eulerX, eulerY, eulerZ);
	if(localRotation.w == 1)
		return;

	float s = sqrt(1 - localRotation.w * localRotation.w);

	Vector3 localRotAxis = (1 / s) * Vector3(localRotation.x, localRotation.y, localRotation.z);
	Vector3 worldRotAxis = localRotAxis * rotation;

	Quaternion worldRotation = Quaternion(localRotation.w,
		worldRotAxis.x * s,
		worldRotAxis.y * s,
		worldRotAxis.z * s);

	rotation = worldRotation * rotation;
	UpdateLocalAxis();
}

// 由rotation更新本地方向轴在世界坐标系下的值
// 每次改变rotation时调用此函数更新本地方向轴
void Object::UpdateLocalAxis()
{
	right = Vector3(1, 0, 0);
	up = Vector3(0, 1, 0);
	forward = Vector3(0, 0, 1);

	right = right * rotation;
	up = up * rotation;
	forward = forward * rotation;
}

// 返回本地方向向量在世界坐标系下的值
Vector3 Object::LocalVectorToWorld(Vector3 localVec)
{
	return localVec * rotation;
}

// 在世界坐标系下按moveVector平移
void Object::Translate(Vector3 moveVector)
{
	position = position + moveVector;
}

// 在本地坐标系下按moveVector平移
void Object::TranslateLocal(Vector3 moveVector)
{
	moveVector = LocalVectorToWorld(moveVector);
	Translate(moveVector);
}


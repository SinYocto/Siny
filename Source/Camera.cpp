#include"Camera.h"

Camera::Camera(float fov, float asp, float nZ, float fZ, float width) 
{ 
	FOV = fov; 
	aspect = asp; 
	nearZ = nZ; 
	farZ = fZ; 
	orthoWidth = width;
}

D3DXMATRIX Camera::ProjMatrix()
{
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, FOV, aspect, nearZ, farZ);
	return matProj;
}

D3DXMATRIX Camera::OrthoProjMatrix()
{
	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, orthoWidth, orthoWidth / aspect, nearZ, farZ);
	return matProj;
}

D3DXMATRIX Camera::ViewMatrix()
{
	D3DXMATRIX matView;

	D3DXMATRIX matRotTranspose, matTransInverse;
	D3DXMatrixTranspose(&matRotTranspose, &(rotation.matrix()));
	D3DXMatrixTranslation(&matTransInverse, -position.x, -position.y, -position.z);
	matView = matTransInverse * matRotTranspose; 

	return matView;
}
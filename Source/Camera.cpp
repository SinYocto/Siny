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

void Camera::ExtractFrustumPlanes()
{
	D3DXMATRIX matVP = ViewMatrix() * ProjMatrix();

	D3DXVECTOR4 col0(matVP(0,0), matVP(1,0), matVP(2,0), matVP(3,0));
	D3DXVECTOR4 col1(matVP(0,1), matVP(1,1), matVP(2,1), matVP(3,1));
	D3DXVECTOR4 col2(matVP(0,2), matVP(1,2), matVP(2,2), matVP(3,2));
	D3DXVECTOR4 col3(matVP(0,3), matVP(1,3), matVP(2,3), matVP(3,3));

	// Planes face inward.
	frustumPlanes[0] = (D3DXPLANE)(col2);        // near
	frustumPlanes[1] = (D3DXPLANE)(col3 - col2); // far
	frustumPlanes[2] = (D3DXPLANE)(col3 + col0); // left
	frustumPlanes[3] = (D3DXPLANE)(col3 - col0); // right
	frustumPlanes[4] = (D3DXPLANE)(col3 - col1); // top
	frustumPlanes[5] = (D3DXPLANE)(col3 + col1); // bottom

	for(int i = 0; i < 6; i++)
		D3DXPlaneNormalize(&frustumPlanes[i], &frustumPlanes[i]);

}

bool Camera::isVisable(BoundingBox boundingBox)
{
	for(int i = 0; i < 6; ++i){

		bool isBehind = true;
		for(int j = 0; j < 8; ++j){
			if(boundingBox.GetVertex(j).Dot(Vector3(frustumPlanes[i].a, frustumPlanes[i].b, frustumPlanes[i].c)) + frustumPlanes[i].d > 0)
				isBehind = false;
		}

		if(isBehind)
			return false;
	}

	return true;
}
#include"SkyBox.h"
#include"Scene.h"

SkyBox::SkyBox()
{
	distance = 500.0f;
	
	vertexBuffer = NULL; 
	indexBuffer = NULL; 
	cubeTex = NULL;
}

void SkyBox::Build()
{
	Vector3 *pos = new Vector3[24];
	pos[0] = distance * Vector3(-1.0f, 1.0f,  1.0f);
	pos[1] = distance * Vector3( 1.0f, 1.0f,  1.0f);
	pos[2] = distance * Vector3(-1.0f, 1.0f, -1.0f);
	pos[3] = distance * Vector3( 1.0f, 1.0f, -1.0f);

	pos[4] = distance * Vector3( 1.0f, -1.0f,  1.0f);
	pos[5] = distance * Vector3(-1.0f, -1.0f,  1.0f);
	pos[6] = distance * Vector3( 1.0f, -1.0f, -1.0f);
	pos[7] = distance * Vector3(-1.0f, -1.0f, -1.0f);

	pos[8]  = distance * Vector3( 1.0f,  1.0f, 1.0f);
	pos[9]  = distance * Vector3(-1.0f,  1.0f, 1.0f);
	pos[10] = distance * Vector3( 1.0f, -1.0f, 1.0f);
	pos[11] = distance * Vector3(-1.0f, -1.0f, 1.0f);

	pos[12] = distance * Vector3(-1.0f,  1.0f, -1.0f);
	pos[13] = distance * Vector3( 1.0f,  1.0f, -1.0f);
	pos[14] = distance * Vector3(-1.0f, -1.0f, -1.0f);
	pos[15] = distance * Vector3( 1.0f, -1.0f, -1.0f);

	pos[16] = distance * Vector3(-1.0f,  1.0f,  1.0f);
	pos[17] = distance * Vector3(-1.0f,  1.0f, -1.0f);
	pos[18] = distance * Vector3(-1.0f, -1.0f,  1.0f);
	pos[19] = distance * Vector3(-1.0f, -1.0f, -1.0f);

	pos[20] = distance * Vector3( 1.0f,  1.0f, -1.0f);
	pos[21] = distance * Vector3( 1.0f,  1.0f,  1.0f);
	pos[22] = distance * Vector3( 1.0f, -1.0f, -1.0f);
	pos[23] = distance * Vector3( 1.0f, -1.0f,  1.0f);

	Vector2 *uv = new Vector2[24];
	uv[0] = Vector2(0, 0);
	uv[1] = Vector2(1, 0);
	uv[2] = Vector2(0, 1);
	uv[3] = Vector2(1, 1);

	uv[4] = Vector2(0, 0);
	uv[5] = Vector2(1, 0);
	uv[6] = Vector2(0, 1);
	uv[7] = Vector2(1, 1);

	uv[8]  = Vector2(0, 0);
	uv[9]  = Vector2(1, 0);
	uv[10] = Vector2(0, 1);
	uv[11] = Vector2(1, 1);

	uv[12] = Vector2(0, 0);
	uv[13] = Vector2(1, 0);
	uv[14] = Vector2(0, 1);
	uv[15] = Vector2(1, 1);

	uv[16] = Vector2(0, 0);
	uv[17] = Vector2(1, 0);
	uv[18] = Vector2(0, 1);
	uv[19] = Vector2(1, 1);

	uv[20] = Vector2(0, 0);
	uv[21] = Vector2(1, 0);
	uv[22] = Vector2(0, 1);
	uv[23] = Vector2(1, 1);

	DWORD* indices = new DWORD[36];
	for(int i = 0; i < 6; i++){
		indices[6*i + 0] = 4*i;
		indices[6*i + 1] = 4*i + 2;
		indices[6*i + 2] = 4*i + 1;
			
		indices[6*i + 3] = 4*i + 2;
		indices[6*i + 4] = 4*i + 3;
		indices[6*i + 5] = 4*i + 1;
	}

	positionData = pos;
	uvData = uv;
	indexData = indices;
			
	numVertices = 24;
	numTriangles = 12;

	distance = 500.0f;

	vertexData = new VertexUV[numVertices];
	for(int i = 0; i < numVertices; ++i){
		VertexUV vert = VertexUV(positionData[i].x, positionData[i].y, positionData[i].z, 
								uvData[i].x, uvData[i].y);
		((VertexUV*)vertexData)[i] = vert;
	}

	CreateVB(D3DDevice, &vertexBuffer, vertexData, numVertices, XYZ_UV);
	CreateIB(D3DDevice, &indexBuffer, indexData, 3*numTriangles);
}


void SkyBox::SetDistance(float dist)
{
	distance = dist;
}

void SkyBox::SetCubeTex(string filenamePX, string filenameNX, string filenamePY, string filenameNY, string filenamePZ, string filenameNZ)
{
	if(cubeTex)
		SAFE_RELEASE(cubeTex);

	int cubeTexSize = 512;

	D3DDevice->CreateCubeTexture(cubeTexSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &cubeTex, NULL);

	IDirect3DSurface9 *cubeTexSurf;

	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenamePX.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenameNX.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenamePY.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenameNY.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenamePZ.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenameNZ.c_str(), NULL, D3DX_DEFAULT, 0, NULL);

	SAFE_RELEASE(cubeTexSurf);
}

//void SkyBox::UpdatePosition()
//{
//	position = scene.mainCamera.position;
//}

void SkyBox::Render()
{
	position = scene.mainCamera.position;
	rotation = Quaternion(0, 0, 0);
	
	/*D3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DDevice->SetTransform(D3DTS_WORLD, &(LocalToWorldMatrix()));
	D3DDevice->SetTransform(D3DTS_VIEW, &(scene.mainCamera.ViewMatrix()));
	D3DDevice->SetTransform(D3DTS_PROJECTION, &(scene.mainCamera.ProjMatrix()));
	D3DDevice->SetStreamSource(0, vertexBuffer, 0, 20);
	D3DDevice->SetFVF(VertexUV::FVF);
	D3DDevice->SetIndices(indexBuffer);
	D3DDevice->SetTexture(0, cubeTex);

	D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numTriangles);*/

	D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	
	D3DDevice->SetStreamSource(0, vertexBuffer, 0, 20);
	D3DDevice->SetFVF(VertexUV::FVF);
	D3DDevice->SetIndices(indexBuffer);

	cubeMappingShader.effect->SetTechnique("CubeMapping");
	cubeMappingShader.effect->SetMatrix("matWVP", &(LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.ProjMatrix()));
	cubeMappingShader.effect->SetMatrix("matWorld", &(LocalToWorldMatrix()));
	cubeMappingShader.effect->SetTexture("cubeTex", cubeTex);
	cubeMappingShader.effect->SetRawValue("centerPos", &position, 0, sizeof(Vector3));

	cubeMappingShader.effect->Begin(0, 0);
	cubeMappingShader.effect->BeginPass(0);
	
	D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numTriangles);

	cubeMappingShader.effect->EndPass();
	cubeMappingShader.effect->End();

	D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
#include"Material.h"

Material::Material()
{
	diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	spec = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	shiness = 1.0f;
	gloss = 1.0f;

	tilesU = 1.0f;
	tilesV = 1.0f;
	offsetU = 0;
	offsetV = 0;

	heightMapScale = 4.0f;
	uvOffset = 0.005f;

	parallaxHeightScale = 0.02f;
	parallaxHeightOffset = 0;

	cubeTexSize = 256;
	
	colorTex = NULL;
	normalTex = NULL;
	bumpTex = NULL;
	cubeTex = NULL;
}

void Material::SetColorTex(string filename)
{
	if(colorTex)
		SAFE_RELEASE(colorTex);
	D3DXCreateTextureFromFile(D3DDevice, filename.c_str(), &colorTex);
}

void Material::SetNormalTex(string filename)
{
	if(normalTex)
		SAFE_RELEASE(normalTex);
	D3DXCreateTextureFromFile(D3DDevice, filename.c_str(), &normalTex);
}

void Material::SetBumpTex(string filename)
{
	if(bumpTex)
		SAFE_RELEASE(bumpTex);
	D3DXCreateTextureFromFile(D3DDevice, filename.c_str(), &bumpTex);
}

void Material::SetCubeTex(string filenamePX, string filenameNX, string filenamePY, string filenameNY, string filenamePZ, string filenameNZ)
{
	if(cubeTex)
		SAFE_RELEASE(cubeTex);

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



D3DXMATRIX Material::UVTransformMatrix()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, tilesU, tilesV, 1.0f);
	D3DXMatrixTranslation(&matTrans, offsetU, offsetV, 0);

	return matScale * matTrans;
}
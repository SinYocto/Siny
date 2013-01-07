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
	
	colorTex = NULL;
	normalTex = NULL;
	bumpTex = NULL;
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

D3DXMATRIX Material::UVTransformMatrix()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, tilesU, tilesV, 1.0f);
	D3DXMatrixTranslation(&matTrans, offsetU, offsetV, 0);

	return matScale * matTrans;
}
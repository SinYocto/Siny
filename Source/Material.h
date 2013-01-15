#ifndef MATERIAL_H
#define MATERIAL_H

#include"D3DUtility.h"
#include<string>

using namespace std;

class Material
{
public:
	Material();
	void SetColorTex(string filename);
	void SetNormalTex(string filename);
	void SetBumpTex(string filename);
	void SetCubeTex(string filenamePX, string filenameNX, string filenamePY, string filenameNY, string filenamePZ, string filenameNZ);
	D3DXMATRIX UVTransformMatrix();
public:
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	float gloss;
	float shiness;
	
	float tilesU;
	float tilesV;

	float offsetU;
	float offsetV;

	float uvOffset;
	float heightMapScale;

	float parallaxHeightScale;
	float parallaxHeightOffset;

	int cubeTexSize;
	
	IDirect3DTexture9 *colorTex;
	IDirect3DTexture9 *normalTex;
	IDirect3DTexture9 *bumpTex;
	IDirect3DCubeTexture9 *cubeTex;
};

#endif
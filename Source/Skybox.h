#ifndef SKYBOX_H
#define SKYBOX_H

#include"D3DUtility.h"
#include"Object.h"

class SkyBox : Object
{
public:
	SkyBox();
	void SetDistance(float dist);
	void SetCubeTex(string filenamePX, string filenameNX, string filenamePY, string filenameNY, string filenamePZ, string filenameNZ);
	void UpdatePosition();
	void Render();
	void Build();
private:
	void *vertexData;
	Vector3	*positionData;
	Vector2 *uvData;
	DWORD *indexData;

	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DIndexBuffer9*  indexBuffer;

	IDirect3DCubeTexture9 *cubeTex;

	int numVertices;
	int numTriangles;

	float distance;
};


#endif
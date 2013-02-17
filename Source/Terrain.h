#ifndef TERRAIN_H
#define TERRAIN_H

#include"Mesh.h"
#include"QuadTree.h"
#include<vector>

struct Chunk
{
	Mesh mesh;
	QuadTreeNode *node;
};

class Terrain : Object
{
public:
	Terrain();
	void LoadFromHeightMap(std::string filename, int dataSize);
	//void BuildTerrain(float terrain_width, float terrain_length, float terrain_height);
	void BuildTerrain(float terrain_length, float terrain_height, int depth);
	void SetColorTexes(std::string filename0, std::string filename1 = "", std::string filename2 = "", std::string filename3 = "");
	void SetSplatMapTex(std::string filename);
	void SetMtlParameters(float tiles_u, float tiles_v, D3DXCOLOR diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	void Render();

	void CalculateNormals();
	void CalculateTangents();
	void CalculateBitangents();
	void BuildChunkMesh(VertexType type);

private:
	D3DXMATRIX UVTransformMatrix();
	void BuildChunks(QuadTreeNode *node, int depth);

private:
	IDirect3DTexture9* colorTexes[4];
	IDirect3DTexture9 *splatMapTex;

	//float width;
	float length;
	float height;

	WORD *heightMapData;

	QuadTreeNode *rootNode;
	vector<Chunk> chunks;
	
	int heightMapSize;
	int numTexes;

	D3DXCOLOR diffuse;
	float tilesU;
	float tilesV;
};


#endif
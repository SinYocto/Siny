#ifndef MESH_H
#define MESH_H

#include"D3DUtility.h"
#include"Object.h"

class Mesh : public Object
{
public:
	Mesh();
	Mesh(const Mesh &mesh);
	Mesh operator=(const Mesh &mesh);
	~Mesh();
	void Build(VertexType type);
	void CreateVertexBuffer(int nVertices);
	void CreateIndexBuffer(int nIndices);
	void SetVertexDeclaration();
	D3DXMATRIX LocalToWorldMatrix();
	void CaculateNormals();
	void CaculateTangents();
	void CaculateBitangents();

	void SetStream();
	void Draw();
public:
	int numVertices;
	int numTriangles;

	VertexType vertexType;
	int sizeofVertex;

	Vector3 scale;
public:
	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DIndexBuffer9*  indexBuffer;

	IDirect3DVertexDeclaration9 *vertexDecl;

	void *vertexData;
	Vector3	*positionData;
	Vector2 *uvData;
	Vector3 *normalData;
	Vector3 *tangentData;
	Vector3 *bitangentData;
	WORD *indexData;
};


#endif
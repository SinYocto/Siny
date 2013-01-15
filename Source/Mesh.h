#ifndef MESH_H
#define MESH_H

#include"D3DUtility.h"
#include"Object.h"
#include<map>
#include<vector>

using namespace std;


enum UVMappingMode { SphereUV };
enum MeshFileFormat { OBJ };

class Mesh : public Object
{
public:
	Mesh();
	Mesh(const Mesh &mesh);
	Mesh operator=(const Mesh &mesh);
	~Mesh();
	void LoadDataFromFile(string filename, MeshFileFormat format);
	void Build(VertexType type);
	void CreateVertexBuffer(int nVertices);
	void CreateIndexBuffer(int nIndices);
	void SetVertexDeclaration();
	D3DXMATRIX LocalToWorldMatrix();
	void CalculateNormals();
	void CalculateTangents();
	void CalculateBitangents();
	void CalculateUVs(UVMappingMode uvMode);

	void SetStream();
	void Draw();
private:
	void OBJParseLine(char *line, vector<Vector3> &filePosData, vector<WORD> &fileIndexData);

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

	map<int, int> normalPairs;

	void *vertexData;
	Vector3	*positionData;
	Vector2 *uvData;
	Vector3 *normalData;
	Vector3 *tangentData;
	Vector3 *bitangentData;
	WORD *indexData;
};


#endif
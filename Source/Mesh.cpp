#include"Mesh.h"


Mesh::Mesh()
{
	vertexBuffer = NULL; 
	indexBuffer = NULL; 
	vertexDecl = NULL; 
	scale = Vector3(1.0f, 1.0f, 1.0f); 
}

Mesh::Mesh(const Mesh &mesh)
{
	position = mesh.position;
	rotation = mesh.rotation;

	vertexBuffer = mesh.vertexBuffer;
	indexBuffer = mesh.indexBuffer;
	vertexData = mesh.vertexData;
	positionData = mesh.positionData;
	uvData = mesh.uvData;
	normalData = mesh.normalData;
	tangentData = mesh.tangentData;
	bitangentData = mesh.bitangentData;
	indexData = mesh.indexData;
	numTriangles = mesh.numTriangles;
	numVertices = mesh.numVertices;
	scale = mesh.scale;
	right = mesh.right;
	up = mesh.up;
	forward = mesh.forward;
	vertexDecl = mesh.vertexDecl;
	vertexType = mesh.vertexType;
	sizeofVertex = mesh.sizeofVertex;

	normalPairs = mesh.normalPairs;

}

Mesh Mesh::operator=(const Mesh &mesh)
{
	position = mesh.position;
	rotation = mesh.rotation;

	vertexBuffer = mesh.vertexBuffer;
	indexBuffer = mesh.indexBuffer;
	vertexData = mesh.vertexData;
	positionData = mesh.positionData;
	uvData = mesh.uvData;
	normalData = mesh.normalData;
	tangentData = mesh.tangentData;
	bitangentData = mesh.bitangentData;
	indexData = mesh.indexData;
	numTriangles = mesh.numTriangles;
	numVertices = mesh.numVertices;
	scale = mesh.scale;
	right = mesh.right;
	up = mesh.up;
	forward = mesh.forward;
	vertexDecl = mesh.vertexDecl;
	vertexType = mesh.vertexType;
	sizeofVertex = mesh.sizeofVertex;

	normalPairs = mesh.normalPairs;

	return *this;
}

Mesh::~Mesh()
{
	/*SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexDecl);*/
}

void Mesh::CreateVertexBuffer(int nVertices)
{
	CreateVB(D3DDevice, &vertexBuffer, vertexData, nVertices, vertexType);
}

void Mesh::CreateIndexBuffer(int nIndices)
{
	CreateIB(D3DDevice, &indexBuffer, indexData, nIndices);
}

void Mesh::Build(VertexType type)
{
	vertexType = type;

	switch(vertexType){
		case XYZ:
			sizeofVertex = 12;
			vertexData = positionData;
			break;
		case XYZ_UV:
			sizeofVertex = 20;
			vertexData = new VertexUV[numVertices];
			for(int i = 0; i < numVertices; ++i){
				VertexUV vert = VertexUV(positionData[i].x, positionData[i].y, positionData[i].z, 
										uvData[i].x, uvData[i].y);
				((VertexUV*)vertexData)[i] = vert;
			}
			break;
		case XYZ_N:
			sizeofVertex = 24;
			vertexData = new VertexN[numVertices];
			for(int i = 0; i < numVertices; ++i){
				VertexN vert = VertexN(positionData[i].x, positionData[i].y, positionData[i].z, 
										normalData[i].x, normalData[i].y, normalData[i].z);
				((VertexN*)vertexData)[i] = vert;
			}
			break;
		case XYZ_UV_N:
			sizeofVertex = 32;
			vertexData = new VertexUVN[numVertices];
			for(int i =0; i < numVertices; ++i){
				VertexUVN vert = VertexUVN(positionData[i].x, positionData[i].y, positionData[i].z, 
										uvData[i].x, uvData[i].y, 
										normalData[i].x, normalData[i].y, normalData[i].z);
				((VertexUVN*)vertexData)[i] = vert;
			}
			break;
		case XYZ_UV_TBN:
			sizeofVertex = 56;
			vertexData = new VertexUVTBN[numVertices];
			for(int i =0; i < numVertices; ++i){
				VertexUVTBN vert = VertexUVTBN(positionData[i].x, positionData[i].y, positionData[i].z, 
										uvData[i].x, uvData[i].y, 
										tangentData[i].x, tangentData[i].y, tangentData[i].z,
										bitangentData[i].x, bitangentData[i].y, bitangentData[i].z,
										normalData[i].x, normalData[i].y, normalData[i].z);
				((VertexUVTBN*)vertexData)[i] = vert;
			}
			break;
	}

	CreateVertexBuffer(numVertices);
	CreateIndexBuffer(3*numTriangles);

}

D3DXMATRIX Mesh::LocalToWorldMatrix()
{
	D3DXMATRIX matScale, matRot, matTrans;
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&matTrans, position.x, position.y, position.z);
	matRot = rotation.matrix();

	return matScale * matRot * matTrans;

}

void Mesh::SetVertexDeclaration()
{
	switch(vertexType){
	case XYZ:
		{
			D3DVERTEXELEMENT9 NMVertexElements[] =  {
				{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},      
				D3DDECL_END()
			};
			if(vertexDecl)
				SAFE_RELEASE(vertexDecl);
			D3DDevice->CreateVertexDeclaration(NMVertexElements, &vertexDecl);
			D3DDevice->SetVertexDeclaration(vertexDecl);
			break;
		}
	case XYZ_UV:
		{
			D3DVERTEXELEMENT9 NMVertexElements[] =  {
				{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},         
				{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},      
				D3DDECL_END()
			};
			if(vertexDecl)
				SAFE_RELEASE(vertexDecl);
			D3DDevice->CreateVertexDeclaration(NMVertexElements, &vertexDecl);
			D3DDevice->SetVertexDeclaration(vertexDecl);
			break;
		}
	case XYZ_N:
		{
			D3DVERTEXELEMENT9 NMVertexElements[] =  {
				{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},   
				{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},        
				D3DDECL_END()
			};
			if(vertexDecl)
				SAFE_RELEASE(vertexDecl);
			D3DDevice->CreateVertexDeclaration(NMVertexElements, &vertexDecl);
			D3DDevice->SetVertexDeclaration(vertexDecl);
			break;
		}
	case XYZ_UV_N:
		{
			D3DVERTEXELEMENT9 NMVertexElements[] =  {
				{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},         
				{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},    
				{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},      
				D3DDECL_END()
			};
			if(vertexDecl)
				SAFE_RELEASE(vertexDecl);
			D3DDevice->CreateVertexDeclaration(NMVertexElements, &vertexDecl);
			D3DDevice->SetVertexDeclaration(vertexDecl);
			break;
		}
	case XYZ_UV_TBN:
		{
			D3DVERTEXELEMENT9 NMVertexElements[] =  {
				{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},         
				{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},    
				{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},     
				{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},     
				{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},      
				D3DDECL_END()
			};
			if(vertexDecl)
				SAFE_RELEASE(vertexDecl);
			D3DDevice->CreateVertexDeclaration(NMVertexElements, &vertexDecl);
			D3DDevice->SetVertexDeclaration(vertexDecl);
			break;
		}
	}
}

void Mesh::Draw()
{
	D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numTriangles);
}

void Mesh::SetStream()
{	
	D3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeofVertex);
	D3DDevice->SetIndices(indexBuffer);
}

void Mesh::CaculateNormals()
{
	for(int i = 0; i < numVertices; ++i){
		normalData[i] = Vector3::Zero;
	}

	for(int i = 0; i < numTriangles; ++i){
		Vector3 v0 = positionData[indexData[3*i + 0]];
		Vector3 v1 = positionData[indexData[3*i + 1]];
		Vector3 v2 = positionData[indexData[3*i + 2]];

		Vector3 v0v1 = v1 - v0;
		Vector3 v0v2 = v2 - v0;

		Vector3 normal = v0v1.Cross(v0v2);
		normal.normalize();

		for(int k = 0; k < 3; ++k){
			normalData[indexData[3*i + k]] = normalData[indexData[3*i + k]] + normal;

			// using normalPairs to find the the vert with the same normal
			map<int, int>::iterator iter = normalPairs.find(indexData[3*i + k]);
			if(iter != normalPairs.end())
				normalData[iter->second] = normalData[iter->second] + normal;
		}
	}

	for(int i = 0; i < numVertices; ++i){
		normalData[i].normalize();
	}
}

void Mesh::CaculateTangents()
{
	for(int i = 0; i < numVertices; ++i){
		tangentData[i] = Vector3::Zero;
	}

	for(int i = 0; i < numTriangles; ++i){
		Vector3 v0 = positionData[indexData[3*i + 0]];
		Vector3 v1 = positionData[indexData[3*i + 1]];
		Vector3 v2 = positionData[indexData[3*i + 2]];

		Vector2 uv0 = uvData[indexData[3*i + 0]];
		Vector2 uv1 = uvData[indexData[3*i + 1]];
		Vector2 uv2 = uvData[indexData[3*i + 2]];

		Vector3 v0v1 = v1 - v0;
		Vector3 v1v2 = v2 - v1;

		Vector3 tangent;
		if(uv1.y == uv2.y)
			tangent = (uv1.x < uv2.x)?v1v2:(-1*v1v2);
		else{
			tangent = v0v1 + ((uv1.y - uv0.y)/(uv1.y - uv2.y))*v1v2;

			if(uv1.y > uv2.y)
				tangent = -1.0f * tangent;
		}

		tangent.normalize();

		tangentData[indexData[3*i + 0]] = tangent;
		tangentData[indexData[3*i + 1]] = tangent;
		tangentData[indexData[3*i + 2]] = tangent;
	}
}

void Mesh::CaculateBitangents()
{
	for(int i = 0; i < numTriangles; ++i){
		Vector3 normal = normalData[indexData[3*i + 0]];
		Vector3 tangent = tangentData[indexData[3*i + 0]];
		Vector3 bitangent = normal.Cross(tangent);

		bitangent.normalize();
		
		bitangentData[indexData[3*i + 0]] = bitangent;
		bitangentData[indexData[3*i + 1]] = bitangent;
		bitangentData[indexData[3*i + 2]] = bitangent;
	}
}
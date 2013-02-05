#ifndef CLOUD_H
#define CLOUD_H

#include"D3DUtility.h"
#include"Object.h"
#include<string>

using namespace std;

class Cloud : public Object
{
public:
	Cloud() { cloudTex = NULL; vertexBuffer = NULL; indexBuffer = NULL; }
	Cloud(string filename) { cloudTexFile = filename; }
	void Build()
	{
		float height = 100.0f;

		// cloud plane
		float size = 1000.0f;
		numVertices = 4;
		numTriangles = 2;

		vertexData = new VertexUV[numVertices];
		((VertexUV*)vertexData)[0] = VertexUV(-size, 0,  size, 0, 0);
		((VertexUV*)vertexData)[1] = VertexUV( size, 0,  size, 1, 0);
		((VertexUV*)vertexData)[2] = VertexUV(-size, 0, -size, 0, 1);
		((VertexUV*)vertexData)[3] = VertexUV( size, 0, -size, 1, 1);
		
		indexData = new DWORD[3*numTriangles];
		indexData[0] = 0;
		indexData[1] = 2;
		indexData[2] = 1;
		
		indexData[3] = 1;
		indexData[4] = 2;
		indexData[5] = 3;

		// 使用一个圆面作为cloud plane
		/*float skyRadius = 1000.0f;
		float radius = sqrt(skyRadius*skyRadius - height*height);

		int segments = 32;
		
		numVertices = 1 + segments;
		numTriangles = segments;

		float deltaTheta = 2*PI / segments;
		
		vertexData = new VertexUV[numVertices];
		((VertexUV*)vertexData)[0] = VertexUV(0, 0, 0, 0.5f, 0.5f);

		for(int i = 0; i < segments; ++i){
			float x = radius * cos(i*deltaTheta);
			float z = radius * sin(i*deltaTheta);
			float u = 0.5f + 0.5f*cos(i*deltaTheta);
			float v = 0.5f - 0.5f*sin(i*deltaTheta);
			
			((VertexUV*)vertexData)[i+1] = VertexUV(x, 0, z, u, v);
		}

		indexData = new DWORD[3*numTriangles];
		for(int i = 0; i < segments; ++i){
			if(i == segments - 1){
				indexData[3*i + 0] = 0;
				indexData[3*i + 1] = i + 1;
				indexData[3*i + 2] = 1;
			}
			else{
				indexData[3*i + 0] = 0;
				indexData[3*i + 1] = i + 1;
				indexData[3*i + 2] = i + 2;
			}
		}*/

		
		CreateVB(D3DDevice, &vertexBuffer, vertexData, numVertices, XYZ_UV);
		CreateIB(D3DDevice, &indexBuffer, indexData, 3*numTriangles);
		
		if(cloudTexFile == ""){
			UINT cloudTexSize = 256;
			D3DXCreateTexture(D3DDevice, cloudTexSize, cloudTexSize, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &cloudTex);
			GenerateNoiseMaps();
			GenerateCloudTex();
		}
		else
			D3DXCreateTextureFromFile(D3DDevice, cloudTexFile.c_str(), &cloudTex);
	}

	void Render();
	void GenerateCloudTex(float cover = 10.0f, float sharpness = 0.97f);

private:
	float NoiseGenerator(int x, int y, int randomSeed);
	void GenerateNoiseMap(float* noiseMap, UINT seed);
	void GenerateNoiseMaps();
	void GetCurrentNoiseMap(float* curNoiseMap);
	float InterpolateNoiseMap(float x, float y, float* noiseMap);
	void OverlapOctaves(float* noiseMap, float* cloudMap);
	void ExpFilter(float* cloudMap, float cover = 20.0f, float sharpness = 0.95f);
public:
	string cloudTexFile;
	IDirect3DTexture9 *cloudTex;

	void *vertexData;
	DWORD *indexData;

	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DIndexBuffer9*  indexBuffer;

	int numVertices;
	int numTriangles;
private:
	float noiseMaps[4][32*32];
};


#endif
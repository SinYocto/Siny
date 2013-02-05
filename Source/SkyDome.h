#ifndef SKYDOME_H
#define SKYDOME_H

#include"D3DUtility.h"
#include"Object.h"
#include<string>
#include"Time.h"

using namespace std;

class DayTime{
public:
	DayTime(int h = 0, int m = 0, int s = 0)
	{ hour = h; minute = m; second = s; counts = 3600*h + 60*m + s; timer = 0; scale = 1.0f; }
	static void Update()
	{
		curDayTime.timer += curDayTime.scale * Time::deltaTime;
		if(curDayTime.timer > 1.0f){
			curDayTime.counts += (int)(curDayTime.timer);
			curDayTime.timer -= (int)(curDayTime.timer);
			if(curDayTime.counts >= 3600*24)
				curDayTime.counts %= 3600*24;
			curDayTime.hour = curDayTime.counts/3600;
			curDayTime.minute = (curDayTime.counts - 3600*curDayTime.hour)/60;
			curDayTime.second = curDayTime.counts%60;
		}
		/*if(curDayTime.timer > 1.0f){
			curDayTime.counts++;
			if(++curDayTime.second == 60){
				if(++curDayTime.minute == 60){
					if(++curDayTime.hour == 24){
						curDayTime.hour = 0;
						curDayTime.counts = 0;
					}
					curDayTime.minute = 0;
				}
				curDayTime.second = 0;
			}
			curDayTime.timer -= 1.0f;
		}*/
	}

	static void SetDayTime(int h = 0, int m = 0, int s = 0)
	{
		curDayTime.hour = h;
		curDayTime.minute = m;
		curDayTime.second = s;
		curDayTime.counts = 3600*h + 60*m + s; 
		curDayTime.timer = 0; 
	}

	static void SetTimeScale(float s)
	{
		curDayTime.scale = s;
	}

public:
	int hour;
	int minute;
	int second;
	int counts;

	float timer;
	float scale;

	static DayTime curDayTime;
};

struct SkyDomeColor{
	SkyDomeColor(D3DCOLOR zColor = 0xffffffff, D3DCOLOR eColor = 0xffffffff, D3DCOLOR wColor = 0xffffffff) 
	{ zenithColor = zColor; eastColor = eColor; westColor = wColor; }
	D3DCOLOR zenithColor;
	D3DCOLOR eastColor;
	D3DCOLOR westColor;
};

class SkyDome : public Object
{
public:
	//SkyDome() { skyTex = NULL; vertexBuffer = NULL; indexBuffer = NULL; }
	SkyDome(string filename = "", float r = 1000.0f) { skyTex = NULL; vertexBuffer = NULL; indexBuffer = NULL; skyTexFile = filename; radius = r; }
	void build()
	{
		//float radius = 1000;
		int segmentsW = 24;
		int segmentsH = 16;

		numVertices = (segmentsW + 1) * (segmentsH + 1);
		numTriangles = 2 * segmentsW * segmentsH;

		float deltaTheta = 2*PI / segmentsW;
		float deltaPhi = PI/2 / segmentsH;

		/*Vector3 *pos = new Vector3[numVertices];
		Vector2 *uv = new Vector2[numVertices];*/
		vertexData = new VertexUV[numVertices];
		for(int i = 0; i <= segmentsH; ++i)
		{
			float phi = i*deltaPhi; 
			float uvSpan = 0.5f * phi / (PI/2);
			for(int j = 0; j <= segmentsW; ++j)
			{
				float theta = j*deltaTheta;
				float y = radius * cos(phi);
				float x = radius * sin(phi) * cos(theta);
				float z = radius * sin(phi) * sin(theta);

				/*float u = (float)atan2(x, y) / 2*PI + 0.5f;
				float v = (float)atan2(z, y) / 2*PI + 0.5f;*/
				float u = 0.5f + cos(theta) * uvSpan;
				float v = 0.5f - sin(theta) * uvSpan;

				/*pos[i*(segmentsW+1) + j] = Vector3(x, y, z);
				uv[i*(segmentsW+1) + j] = Vector2(u, v);*/
				((VertexUV*)vertexData)[i*(segmentsW+1) + j] = VertexUV(x, y, z, u, v);
			}
		}

		//DWORD* indices = new DWORD[3*numTriangles];
		indexData = new DWORD[3*numTriangles];
		for(int i = 0; i < segmentsH; ++i)
		{
			for(int j = 0; j < segmentsW; ++j)
			{
				indexData[6*(segmentsW*i + j) + 0] = i*(segmentsW+1) + j;
				indexData[6*(segmentsW*i + j) + 1] = (i+1)*(segmentsW+1) + j;
				indexData[6*(segmentsW*i + j) + 2] = (i+1)*(segmentsW+1) + j+1;
				
				indexData[6*(segmentsW*i + j) + 3] = i*(segmentsW+1) + j;
				indexData[6*(segmentsW*i + j) + 4] = (i+1)*(segmentsW+1) + j+1;
				indexData[6*(segmentsW*i + j) + 5] = i*(segmentsW+1) + j+1;

			}
		}
		
		CreateVB(D3DDevice, &vertexBuffer, vertexData, numVertices, XYZ_UV);
		CreateIB(D3DDevice, &indexBuffer, indexData, 3*numTriangles);

			
		/*if(skyTex)
			SAFE_RELEASE(skyTex);*/
		if(skyTexFile == ""){
			UINT skyTexSize = 32;
			D3DXCreateTexture(D3DDevice, skyTexSize, skyTexSize, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &skyTex);
			UpdateSkyTex(SkyDomeColor(0xff87ceff, 0xff8B5F65, 0xee9a49));
		}
		else
			D3DXCreateTextureFromFile(D3DDevice, skyTexFile.c_str(), &skyTex);
	}

	void UpdateSkyTex(SkyDomeColor skyColor)
	{
		D3DSURFACE_DESC surfaceDesc;
		D3DLOCKED_RECT lockedRect;

		skyTex->GetLevelDesc(0, &surfaceDesc);
		skyTex->LockRect(0, &lockedRect, NULL, 0);

		BYTE* ptr = (BYTE*)lockedRect.pBits;
		int size = surfaceDesc.Height;
		for(DWORD i = 0; i < surfaceDesc.Height; ++i){
			for(DWORD j = 0; j < surfaceDesc.Width; ++j){
				DWORD ix = i*lockedRect.Pitch + 4*j;

				float temp = (float)(size - 1) / 2;
				float factorH = ((i - temp)*(i - temp) + (j - temp)*(j - temp)) / (temp*temp);
				if(factorH > 1)
					factorH = 1;

				if(i == temp && j == temp){
					((DWORD*)(&(ptr[ix])))[0] = skyColor.zenithColor;
					continue;
				}

				float factorV = Vector3(0, 1, 0).Dot(Vector3(i - temp, j - temp, 0).normalized());
				factorV = (factorV + 1) / 2;
				factorV = pow(factorV, 4);

				D3DCOLOR altitudeColor = ColorLerp(skyColor.westColor, skyColor.eastColor, factorV);
				D3DCOLOR resultColor = ColorLerp(skyColor.zenithColor, altitudeColor, factorH);
				((DWORD*)(&(ptr[ix])))[0] = resultColor;
			}
		}

		skyTex->UnlockRect(0);
	}

	void Render(bool isCloudDome = false);
public:
	//Vector3 *positionData;
	//Vector2 *uvData;

	string skyTexFile;
	IDirect3DTexture9 *skyTex;

	void *vertexData;
	DWORD *indexData;

	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DIndexBuffer9*  indexBuffer;

	int numVertices;
	int numTriangles;

	float radius;


};



#endif
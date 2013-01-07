#include"Cloud.h"
#include"Scene.h"
#include"SkyDome.h"
#include<time.h>

void Cloud::Render()
{
	position = scene.mainCamera.position;
	position.y += 100.0f;

	D3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);


	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	/*D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);*/

	D3DDevice->SetTransform(D3DTS_WORLD, &(LocalToWorldMatrix()));
	D3DDevice->SetTransform(D3DTS_VIEW, &(scene.mainCamera.ViewMatrix()));
	D3DDevice->SetTransform(D3DTS_PROJECTION, &(scene.mainCamera.ProjMatrix()));
	D3DDevice->SetStreamSource(0, vertexBuffer, 0, 20);
	D3DDevice->SetFVF(VertexUV::FVF);
	D3DDevice->SetIndices(indexBuffer);
	D3DDevice->SetTexture(0, cloudTex);

	D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numTriangles);
	
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

float Cloud::NoiseGenerator(int x, int y, int randomSeed)
{
	int n = x + y*57 + randomSeed*131;
    n = (n<<13) ^ n;
    return (1.0f - ( (n*(n*n*15731 + 789221) +
            1376312589)&0x7fffffff) / 1073741824.0f);
}

void Cloud::GenerateCloudTex(float cover, float sharpness)
{
	static int lastCounts = DayTime::curDayTime.counts;
	int updateInterval = 3600 / 10;
	if(DayTime::curDayTime.counts < lastCounts)
		lastCounts -= 24*3600;
	if(DayTime::curDayTime.counts < lastCounts + updateInterval){
		return;
	}
	lastCounts = DayTime::curDayTime.counts;

	float noiseMap[32*32];
	float cloudMap[256*256];

	//GenerateNoiseMap(noiseMap);
	GetCurrentNoiseMap(noiseMap);
	OverlapOctaves(noiseMap, cloudMap);
	ExpFilter(cloudMap, cover, sharpness);

	
	D3DSURFACE_DESC surfaceDesc;
	D3DLOCKED_RECT lockedRect;

	cloudTex->GetLevelDesc(0, &surfaceDesc);
	cloudTex->LockRect(0, &lockedRect, NULL, 0);

	BYTE* ptr = (BYTE*)lockedRect.pBits;
		
	for(DWORD i = 0; i < surfaceDesc.Height; ++i){
		for(DWORD j = 0; j < surfaceDesc.Width; ++j){
			DWORD ix = i*lockedRect.Pitch + 4*j;

			BYTE channel = ((int)(cloudMap[256*i + j])) % 255;
			ptr[ix] = channel;
			ptr[ix+1] = channel;
			ptr[ix+2] = channel;
			ptr[ix+3] = channel;
		}
	}
	cloudTex->UnlockRect(0);

}

void Cloud::GenerateNoiseMap(float* noiseMap, UINT seed)
{
	float map[34][34];
	srand(seed); 
	int random = rand() % 5000;

	// noise
	for(int i = 1; i < 33; ++i){
		for(int j = 1; j < 33; ++j){
			map[i][j] = 128.0f + NoiseGenerator(i, j, random)*128.0f;
		}
	}

	// seamless
	for(int k = 0; k < 33; ++k){
		map[0][k] = map[32][k];
		map[33][k] = map[1][k];
		map[k][0] = map[k][32];
		map[k][33] = map[k][1];
	}
	map[0][0] = map[32][32];
	map[33][33] = map[1][1];
	map[0][33] = map[32][1];
	map[33][0] = map[1][32];

	// smooth
	for(int i = 1; i < 33; ++i){
		for(int j = 1; j < 33; ++j){
			float center = map[i][j] / 4.0f;
			float sides = (map[i-1][j] + map[i+1][j] + map[i][j-1] + map[i][j+1]) / 8.0f;
			float corners = (map[i-1][j-1] + map[i-1][j+1] + map[i+1][j-1] + map[i+1][j+1]) / 16.0f;

			noiseMap[(i-1)*32 + (j-1)] = center + sides + corners;
		}
	}
}

float Cloud::InterpolateNoiseMap(float x, float y, float *noiseMap)
{
	int xInt = (int)x;
	int yInt = (int)y;

	float xFrac = x - xInt;
	float yFrac = y - yInt;

	int x0 = xInt % 32;
	int y0 = yInt % 32;
	int x1 = (xInt + 1) % 32;
	int y1 = (yInt + 1) % 32;

	float top = noiseMap[y0*32 + x0] + xFrac*(noiseMap[y0*32 + x1] - noiseMap[y0*32 + x0]);
	float bot = noiseMap[y1*32 + x0] + xFrac*(noiseMap[y1*32 + x1] - noiseMap[y1*32 + x0]);

	return(top + yFrac*(bot - top));

}

void Cloud::OverlapOctaves(float *noiseMap, float *cloudMap)
{
	// clear cloudMap
	for(int i = 0; i < 256*256; ++i)
		cloudMap[i] = 0;

	for(int octaveIx = 0; octaveIx < 4; octaveIx++){
		for(int i = 0; i < 256; ++i){
			for(int j = 0; j < 256; ++j){
				float scale = 1.0f / pow((float)2, 3-octaveIx);
				float noise = InterpolateNoiseMap(j*scale, i*scale, noiseMap);

				cloudMap[256*i + j] += noise / pow((float)2, octaveIx);
			}
		}
	}

}

void Cloud::ExpFilter(float *cloudMap, float cover, float sharpness)
{
	for(int i = 0; i < 256*256; ++i){
		float c = cloudMap[i] - (255.0f - cover);
		if(c < 0)
			c = 0;
		cloudMap[i] = 255.0f - ((float)(pow(sharpness, c)) * 255.0f);
	}
}

void Cloud::GenerateNoiseMaps()
{
	for(int i = 0; i < 4; ++i)
		GenerateNoiseMap(noiseMaps[i], (unsigned)time(NULL) + i);
}

void Cloud::GetCurrentNoiseMap(float* curNoiseMap)
{	

	int sectionIx = DayTime::curDayTime.hour / 6;
	float factor = (float)(DayTime::curDayTime.counts - 3600*6*sectionIx) / (3600*6);

	static bool regenerated = false;

	// take special care when the time reach the last section of the day
	// we should regenerate the next day's noisemaps and interpolate the noisemap between the day and the next day
	static float saveNoiseMap[32*32];
	if(sectionIx == 3){

		if(!regenerated){
			for(int i = 0; i < 32*32; ++i)
				saveNoiseMap[i] = noiseMaps[3][i];

			GenerateNoiseMaps();
			regenerated = true;
		}

		for(int i = 0; i < 32*32; ++i)
			curNoiseMap[i] = Lerp(saveNoiseMap[i], noiseMaps[0][i], factor);
			//curNoiseMap[i] = saveNoiseMap[i];
	}
	else{
		regenerated = false;
		for(int i = 0; i < 32*32; ++i)
			curNoiseMap[i] = Lerp(noiseMaps[sectionIx][i], noiseMaps[sectionIx+1][i], factor);
	}


	//int sectionIx = DayTime::curDayTime.hour / 6;
	//float lastNoiseMap[32][32];

	//static bool regenerateFlag = true;
	//if(sectionIx == 3 && regenerateFlag){
	//	GenerateNoiseMaps();
	//	reGenerateFlag = false;
	//}
	//else 
	//	reGenerateFlag = true;

	//float factor = (float)(DayTime::curDayTime.counts - 3600*6*sectionIx) / (3600*6);
	////printf("factor:%f\n", factor);

	//for(int i = 0; i < 32*32; ++i){
	//	curNoiseMap[i] = Lerp(lastNoiseMap[i], noiseMaps[(sectionIx+1)%4][i], factor);
	//}
}
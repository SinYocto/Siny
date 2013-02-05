#include"Terrain.h"
#include<fstream>
#include"Scene.h"
#include"Shader.h"

Terrain::Terrain()
{
	splatMapTex = NULL;
	heightMapData = NULL;

	for(int i = 0; i < 4; ++i)
		colorTexes[i] = NULL;

	tilesU = 1.0f;
	tilesV = 1.0f;
}

void Terrain::LoadFromHeightMap(std::string filename, int texSize)
{
	heightMapSize = texSize;
	heightMapData = new WORD[heightMapSize*heightMapSize];
	std::ifstream inFile(filename.c_str(), std::ios_base::binary);
	inFile.read((char*)heightMapData, heightMapSize*heightMapSize*sizeof(WORD));
	inFile.close();
}

void Terrain::BuildTerrain(float terrain_width, float terrain_length, float terrain_height)
{
	numVertices = heightMapSize * heightMapSize;
	numTriangles = 2*(heightMapSize - 1)*(heightMapSize - 1);

	/*std::ofstream fout("terrainInfo.txt");
	fout<<"terrain size: "<<heightMapSize<<"*"<<heightMapSize<<"\n";
	fout<<"numVertices: "<<numVertices<<"\n";
	fout<<"numTriangles: "<<numTriangles<<"\n\n";*/

	Vector3 *pos = new Vector3[numVertices];
	Vector2 *uv = new Vector2[numVertices];
	for(int i = 0; i < numVertices; ++i){
		int row = i / heightMapSize;
		int column = i % heightMapSize;

		float x = ((float)column - (heightMapSize - 1)/2) * terrain_width / (heightMapSize - 1);
		float z = ((heightMapSize - 1)/2 - (float)row) * terrain_length / (heightMapSize - 1);
		float y = heightMapData[(heightMapSize - 1 - row)*heightMapSize + column] * terrain_height / 16383.0f;

		pos[i] = Vector3(x, y, z);
		uv[i] = Vector2((float)column/(heightMapSize - 1), (float)row/(heightMapSize - 1));

		//fout<<"vert["<<i<<"]:\t"<<x<<"\t"<<y<<"\t"<<z<<"\n";
	}

	//fout<<"\n";
	
	DWORD* indices = new DWORD[3*numTriangles];
	for(int i = 0; i < heightMapSize - 1; ++i){
		for(int j = 0; j < heightMapSize - 1; ++j){
			indices[6*((heightMapSize - 1)*i + j) + 0] = heightMapSize*i + j;
			indices[6*((heightMapSize - 1)*i + j) + 1] = heightMapSize*i + j+1;
			indices[6*((heightMapSize - 1)*i + j) + 2] = heightMapSize*(i+1) + j;

			//fout<<"triangle["<<2*((heightMapSize - 1)*i + j)<<"]:\t"<<indices[6*((heightMapSize - 1)*i + j) + 0]<<"\t"<<indices[6*((heightMapSize - 1)*i + j) + 1]<<"\t"<<indices[6*((heightMapSize - 1)*i + j) + 2]<<"\n";
			
			indices[6*((heightMapSize - 1)*i + j) + 3] = heightMapSize*(i+1) + j;
			indices[6*((heightMapSize - 1)*i + j) + 4] = heightMapSize*i + j+1;
			indices[6*((heightMapSize - 1)*i + j) + 5] = heightMapSize*(i+1) + j+1;
			
			//fout<<"triangle["<<2*((heightMapSize - 1)*i + j) + 1<<"]:\t"<<indices[6*((heightMapSize - 1)*i + j) + 3]<<"\t"<<indices[6*((heightMapSize - 1)*i + j) + 4]<<"\t"<<indices[6*((heightMapSize - 1)*i + j) + 5]<<"\n";
		}
	}
	
	//fout.close();

	positionData = pos;
	uvData = uv;
	normalData = new Vector3[numVertices];
	tangentData = new Vector3[numVertices];
	bitangentData = new Vector3[numVertices];
	indexData = indices;
}

void Terrain::SetColorTexes(std::string filename0, std::string filename1, std::string filename2, std::string filename3)
{
	if(colorTexes[0])
		SAFE_RELEASE(colorTexes[0]);
	D3DXCreateTextureFromFile(D3DDevice, filename0.c_str(), &colorTexes[0]);

	numTexes = 1;

	if(!filename1.empty()){
		if(colorTexes[1])
			SAFE_RELEASE(colorTexes[1]);
		D3DXCreateTextureFromFile(D3DDevice, filename1.c_str(), &colorTexes[1]);
		numTexes++;
	}
	
	if(!filename2.empty()){
		if(colorTexes[2])
			SAFE_RELEASE(colorTexes[2]);
		D3DXCreateTextureFromFile(D3DDevice, filename2.c_str(), &colorTexes[2]);
		numTexes++;
	}
	
	if(!filename3.empty()){
		if(colorTexes[3])
			SAFE_RELEASE(colorTexes[3]);
		D3DXCreateTextureFromFile(D3DDevice, filename3.c_str(), &colorTexes[3]);
		numTexes++;
	}
}

void Terrain::SetSplatMapTex(std::string filename)
{
	if(splatMapTex)
		SAFE_RELEASE(splatMapTex);
	D3DXCreateTextureFromFile(D3DDevice, filename.c_str(), &splatMapTex);

}

void Terrain::SetMtlParameters(float tiles_u, float tiles_v, D3DXCOLOR diffuseColor)
{
	tilesU = tiles_u;
	tilesV = tiles_v;
	diffuse = diffuseColor;
}


D3DXMATRIX Terrain::UVTransformMatrix()
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, tilesU, tilesV, 1.0f);

	return matScale;
}

void Terrain::Render()
{
	SetStream();
	SetVertexDeclaration();
	
	terrainShader.effect->SetTechnique("Terrain");
	terrainShader.effect->SetMatrix("matWVP", &(LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.ProjMatrix()));
	terrainShader.effect->SetMatrix("matWorld", &(LocalToWorldMatrix()));
	terrainShader.effect->SetMatrix("matUVTransform", &UVTransformMatrix());
	terrainShader.effect->SetRawValue("ambientLight", &(scene.ambientLight->intensity * scene.ambientLight->color), 0 , sizeof(Vector3));
	terrainShader.effect->SetRawValue("directionalLights", directionalLightsData, 0, sizeof(directionalLightsData));
	terrainShader.effect->SetRawValue("pointLights", pointLightsData, 0, sizeof(pointLightsData));
	terrainShader.effect->SetTexture("colorTex0", colorTexes[0]);
	terrainShader.effect->SetTexture("colorTex1", colorTexes[1]);
	terrainShader.effect->SetTexture("colorTex2", colorTexes[2]);
	terrainShader.effect->SetTexture("colorTex3", colorTexes[3]);
	terrainShader.effect->SetTexture("splatMapTex", splatMapTex);
	terrainShader.effect->SetInt("numTexes", numTexes);
	terrainShader.effect->SetRawValue("mtlDiffuse", &diffuse, 0, sizeof(D3DXCOLOR));

	terrainShader.effect->Begin(0, 0);
	terrainShader.effect->BeginPass(0);

	Draw();

	terrainShader.effect->EndPass();
	terrainShader.effect->End();
}

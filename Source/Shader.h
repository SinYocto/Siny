#ifndef SHADER_H
#define SHADER_H

#include"D3DUtility.h"
#include<string>

using namespace std;

enum ShadingMethod { Diffuse, Specular, BumpSpecular, BumpHSpecular, BumpPHSpecular, BumpPSpecular, BumpPOMSpecular, CubeEM };

class Shader{
public:
	Shader(string name = ""){ fileName = name; effect = NULL; }
	void CreateEffect()
	{ D3DXCreateEffectFromFile(D3DDevice, fileName.c_str(), NULL, NULL, D3DXSHADER_DEBUG, NULL, &effect, NULL); }
public:
	string fileName;
	LPD3DXEFFECT effect;
};

extern Shader diffuseShader;
extern Shader specShader;
extern Shader bumpSpecShader;
extern Shader bumpHSpecShader;
extern Shader bumpPHSpecShader;
extern Shader bumpPSpecShader;
extern Shader bumpPOMSpecShader;
extern Shader gizmoShader;
extern Shader cubeEMShader;

#endif
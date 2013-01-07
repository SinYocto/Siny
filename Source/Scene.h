#ifndef SCENE_H
#define SCENE_H

#include"D3DUtility.h"
#include"Camera.h"
#include"DirectionalLight.h"
#include"PointLight.h"
#include"RenderableObject.h"
#include"SkyDome.h"
#include"Cloud.h"
#include<list>
#include<string>

using namespace std;

class Scene
{
public:
	Scene() 
	{ numObjects = 0; numActiveDirectionalLights = 0; numActivePointLights = 0; directionalLightsDirty = true; pointLightsDirty = true; backgroundColor = 0xff1e90ff; }
	void AddObject(RenderableObject *object);
	void AddDirectionalLight(DirectionalLight *light);
	void AddPointLight(PointLight *light);
	void SetSkyDome(string skyTexFile = "", float radius = 1000.0f)
	{
		if(skyTexFile == "")
			isColorSky = true;
		skyDome = SkyDome(skyTexFile, radius);
		skyDome.build();
	}

	void SetCloud(string cloudTexFile = "")
	{
		cloud = Cloud(cloudTexFile);
		cloud.Build();
	}

	void SetCloudDome(string cloudTexFile, float radius = 500.0f)
	{
		cloudDome = SkyDome(cloudTexFile, radius);
		cloudDome.build();
	}

	void SetSkyColor(SkyDomeColor color0, SkyDomeColor color1, SkyDomeColor color2, SkyDomeColor color3)
	{
		skyColor[0] = color0;
		skyColor[1] = color1;
		skyColor[2] = color2;
		skyColor[3] = color3;
	}
	void UpdateSkyColor();

	void Update();
	void Render();
public:
	Camera mainCamera;
	list<DirectionalLight*> directionalLights;
	list<PointLight*> pointLights;
	list<RenderableObject*> objects;

	SkyDome skyDome;
	SkyDomeColor skyColor[4];
	bool isColorSky;

	SkyDome cloudDome;
	Cloud cloud;

	int numObjects;
	int numActiveDirectionalLights;
	int numActivePointLights;

	bool directionalLightsDirty;
	bool pointLightsDirty;


	D3DCOLOR backgroundColor;

};

extern Scene scene;



#endif
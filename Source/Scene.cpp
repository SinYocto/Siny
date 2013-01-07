#include"Scene.h"

void Scene::AddObject(RenderableObject *object)
{
	objects.push_back(object);
	numObjects++;

}

void Scene::AddDirectionalLight(DirectionalLight *light)
{
	directionalLights.push_back(light);
	if(light->IsEnabled()){
		if(numActiveDirectionalLights < MAX_DIRECTIONAL_LIGHTS)
			numActiveDirectionalLights++;
		else
			light->Disable();
	}
}


void Scene::AddPointLight(PointLight *light)
{
	pointLights.push_back(light);
	if(light->IsEnabled()){
		if(numActivePointLights < MAX_POINT_LIGHTS)
			numActivePointLights++;
		else
			light->Disable();
	}
}

void Scene::Update()
{
	DayTime::Update();

	if(isColorSky)
		UpdateSkyColor();

	cloud.GenerateCloudTex();

	if(directionalLightsDirty){

		// clear
		// 注意！似乎传入GPU中计算的始终为最大数量的light，只是不存在的light的color为0
		for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i){
			directionalLightsData[i] = DirectionalLightData(Vector3(0, 0, 0));
		}

		int ix = 0;
		for(list<DirectionalLight*>::iterator iter = directionalLights.begin(); iter != directionalLights.end(); ++iter)
		{
			if((*iter)->IsEnabled()){
				directionalLightsData[ix] = (*iter)->Data();
				ix++;
			}
		}

		directionalLightsDirty = false;
	}

	if(pointLightsDirty){
		
		for(int i = 0; i < MAX_POINT_LIGHTS; ++i){
			pointLightsData[i] = PointLightData(Vector3(0, 0, 0));
		}

		int ix = 0;
		for(list<PointLight*>::iterator iter = pointLights.begin(); iter != pointLights.end(); ++iter)
		{
			if((*iter)->IsEnabled()){
				pointLightsData[ix] = (*iter)->Data();
				ix++;
			}
		}

		pointLightsDirty = false;
	}
}

void Scene::Render()
{
	D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, backgroundColor, 1.0f, 0);
	D3DDevice->BeginScene();

	skyDome.Render();
	cloud.Render();
	//cloudDome.Render(true);
	
	for(list<RenderableObject*>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
		(*iter)->Draw();

	D3DDevice->EndScene();
	D3DDevice->Present(0, 0, 0, 0);
}

void Scene::UpdateSkyColor()
{
	SkyDomeColor curColor;
	DayTime curTime = DayTime::curDayTime;
	if(curTime.hour < 6){
		float factor = (float)(curTime.counts - DayTime(0, 0, 0).counts) / (6*3600);
		curColor.zenithColor = ColorLerp(skyColor[0].zenithColor, skyColor[1].zenithColor, factor);
		curColor.eastColor = ColorLerp(skyColor[0].eastColor, skyColor[1].eastColor, factor);
		curColor.westColor = ColorLerp(skyColor[0].westColor, skyColor[1].westColor, factor);
	}
	else if(curTime.hour < 12){
		float factor = (float)(curTime.counts - DayTime(6, 0, 0).counts) / (6*3600);
		curColor.zenithColor = ColorLerp(skyColor[1].zenithColor, skyColor[2].zenithColor, factor);
		curColor.eastColor = ColorLerp(skyColor[1].eastColor, skyColor[2].eastColor, factor);
		curColor.westColor = ColorLerp(skyColor[1].westColor, skyColor[2].westColor, factor);
	}
	else if(curTime.hour < 18){
		float factor = (float)(curTime.counts - DayTime(12, 0, 0).counts) / (6*3600);
		curColor.zenithColor = ColorLerp(skyColor[2].zenithColor, skyColor[3].zenithColor, factor);
		curColor.eastColor = ColorLerp(skyColor[2].eastColor, skyColor[3].eastColor, factor);
		curColor.westColor = ColorLerp(skyColor[2].westColor, skyColor[3].westColor, factor);
	}
	else{
		float factor = (float)(curTime.counts - DayTime(18, 0, 0).counts) / (6*3600);
		curColor.zenithColor = ColorLerp(skyColor[3].zenithColor, skyColor[0].zenithColor, factor);
		curColor.eastColor = ColorLerp(skyColor[3].eastColor, skyColor[0].eastColor, factor);
		curColor.westColor = ColorLerp(skyColor[3].westColor, skyColor[0].westColor, factor);
	}

	skyDome.UpdateSkyTex(curColor);
}
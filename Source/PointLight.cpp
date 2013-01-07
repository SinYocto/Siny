#include"PointLight.h"
#include"Scene.h"

PointLight::PointLight(Vector3 lightColor, Vector3 lightPos, float lightIntensity)
{ 
	color = lightColor; 
	position = lightPos; 
	intensity = lightIntensity; 
	attenuation = Vector3(1.0f, 0, 0); 

	isEnabled = true;
}


void PointLight::Enable()
{
	if(isEnabled)
		return;
	else{
		if(scene.numActivePointLights < MAX_POINT_LIGHTS){
			isEnabled = true;
			scene.pointLightsDirty = true;
		}
	}
}

void PointLight::Disable()
{
	if(!isEnabled)
		return;
	else{
		isEnabled = false;
		scene.pointLightsDirty = true;
	}
}

bool PointLight::IsEnabled()
{
	return isEnabled;
}
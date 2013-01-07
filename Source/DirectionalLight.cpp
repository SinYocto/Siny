#include"DirectionalLight.h"
#include"Scene.h"

DirectionalLight::DirectionalLight(Vector3 lightColor, Vector3 lightDir, float lightIntensity)
{ 
	color = lightColor; 
	direction = lightDir; 
	intensity = lightIntensity;
	
	isEnabled = true;
}

void DirectionalLight::Enable()
{
	if(isEnabled)
		return;
	else{   // ����scene�ڵ�light�ı仯Ҳ�ᵼ��scene��light��update�����Ż�
		if(scene.numActiveDirectionalLights < MAX_DIRECTIONAL_LIGHTS){
			isEnabled = true;
			scene.directionalLightsDirty = true;
		}
	}
}

void DirectionalLight::Disable()
{
	if(!isEnabled)
		return;
	else{
		isEnabled = false;
		scene.directionalLightsDirty = true;
	}
}

bool DirectionalLight::IsEnabled()
{
	return isEnabled;
}
#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include"Math.h"

#define MAX_DIRECTIONAL_LIGHTS 2

struct DirectionalLightData{
	DirectionalLightData(Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f), Vector3 lightDir = Vector3(0, -1.0f, 0)) 
	{ color = lightColor; direction = lightDir; pad0 = 1.0f; pad1 = 1.0f;}
	Vector3 color;
	float pad0;
	Vector3 direction;
	float pad1;
};

class DirectionalLight
{
public:
	DirectionalLight(Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f), Vector3 lightDir = Vector3(0, -1.0f, 0), float lightIntensity = 1.0f);
	DirectionalLightData Data() { return DirectionalLightData(intensity * color, direction); }
	void Enable();
	void Disable();
	bool IsEnabled();
public:
	Vector3 color;
	Vector3 direction;
	float intensity;
private:
	bool isEnabled;
};

extern DirectionalLightData directionalLightsData[MAX_DIRECTIONAL_LIGHTS];

#endif
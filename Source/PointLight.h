#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include"Math.h"

#define MAX_POINT_LIGHTS 8

struct PointLightData{
	PointLightData(Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f), Vector3 lightPos = Vector3(0, 0, 0), Vector3 atten = Vector3(1.0f, 0, 0)) 
	{ color = lightColor; position = lightPos; attenuation = atten; pad0 = 1.0f; pad1 = 1.0f; pad2 = 1.0f; }
	Vector3 color;
	float pad0;
	Vector3 position;
	float pad1;
	Vector3 attenuation;
	float pad2;
};

class PointLight
{
public:
	PointLight(Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f), Vector3 lightPos = Vector3(0, 0, 0), float lightIntensity = 1.0f);
	PointLightData Data() { return PointLightData(intensity * color, position, attenuation); }
	void Enable();
	void Disable();
	bool IsEnabled();
public:
	Vector3 color;
	Vector3 position;
	Vector3 attenuation;
	float intensity;
private:
	bool isEnabled;
}; 

extern PointLightData pointLightsData[MAX_POINT_LIGHTS];

#endif
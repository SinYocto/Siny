#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include"Math.h"

class AmbientLight{
public:
	AmbientLight(Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f), float lightIntensity = 0);
public:
	Vector3 color;
	float intensity;
};



#endif
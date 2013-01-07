// LightsDefine.fx
#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 8

struct DirectionalLight
{
	float4 color;
	float3 dir;
};

struct PointLight
{
	float4 color;
	float3 position;
	float3 atten;
};

struct AmbientLight
{
	float3 color;
};
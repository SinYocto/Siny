// Diffuse.fx

#include"LightsDefine.fx"

float4x4 matWVP;
float4x4 matWorld;
float4x4 matUVTransform;
texture colorTex;
float4 mtlDiffuse;

DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
PointLight pointLights[MAX_POINT_LIGHTS];

sampler ColorS = sampler_state
{
	Texture = <colorTex>;
	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

void DiffuseVS(float3 Pos : POSITION0, 
			float2 Tex : TEXCOORD0,
			float3 Normal : NORMAL0,
			out float4 oPos  : POSITION0, 
			out float2 oTex : TEXCOORD0,
			out float3 oNormal : TEXCOORD1,
			out float3 oPosW : TEXCOORD2)
{
	oPos = mul(float4(Pos, 1.0f), matWVP);
	oPosW = (mul(float4(Pos, 1.0f), matWorld)).xyz;
	oTex = (mul(float4(Tex, 0, 1.0f), matUVTransform)).xy;
	oNormal = (mul(float4(Normal, 0), matWorld)).xyz;
}

float4 DiffusePS(float2 Tex : TEXCOORD0, float3 Normal : TEXCOORD1, float3 posW : TEXCOORD2) : COLOR0
{
	float4 color = float4(0, 0, 0, 1);
	
	for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i){
		float diffuse = saturate(dot(-directionalLights[i].dir, Normal)); 
		color += diffuse * directionalLights[i].color;
	}
	
	for(int i = 0; i < MAX_POINT_LIGHTS; ++i){
		float3 lightDir = normalize(posW - pointLights[i].position);
		float diffuse = saturate(dot(-lightDir, Normal));
		float distance = length(posW - pointLights[i].position);
		float attenuation = 1 / (pointLights[i].atten.x + pointLights[i].atten.y * distance + pointLights[i].atten.z * distance * distance);
		color += attenuation * diffuse * pointLights[i].color;
	}
	
	color *= mtlDiffuse * tex2D(ColorS, Tex);
	return color;
}

technique Diffuse
{
	pass P0
	{
		VertexShader = compile vs_3_0 DiffuseVS();
		PixelShader = compile ps_3_0 DiffusePS();		
	}
}

// Terrain.fx

#include"LightsDefine.fx"

float4x4 matWVP;
float4x4 matWorld;
float4x4 matUVTransform;
float4 mtlDiffuse;

texture colorTex0;
texture colorTex1;
texture colorTex2;
texture colorTex3;
texture splatMapTex;
int numTexes;


AmbientLight ambientLight;
DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
PointLight pointLights[MAX_POINT_LIGHTS];

sampler ColorS0 = sampler_state
{
	Texture = <colorTex0>;
	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler ColorS1 = sampler_state
{
	Texture = <colorTex1>;
	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler ColorS2 = sampler_state
{
	Texture = <colorTex2>;
	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler ColorS3 = sampler_state
{
	Texture = <colorTex3>;
	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler SplatS = sampler_state
{
	Texture = <splatMapTex>;
	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

void TerrainVS(float3 Pos : POSITION0, 
			float2 Tex : TEXCOORD0,
			float3 Normal : NORMAL0,
			out float4 oPos  : POSITION0, 
			out float2 oTex : TEXCOORD0,
			out float2 oTexSplat : TEXCOORD1,
			out float3 oNormal : TEXCOORD2,
			out float3 oPosW : TEXCOORD3)
{
	oPos = mul(float4(Pos, 1.0f), matWVP);
	oPosW = (mul(float4(Pos, 1.0f), matWorld)).xyz;
	oTex = (mul(float4(Tex, 0, 1.0f), matUVTransform)).xy;
	oTexSplat = Tex;
	oNormal = (mul(float4(Normal, 0), matWorld)).xyz;
}

float4 TerrainPS(float2 Tex : TEXCOORD0, float2 TexSplat : TEXCOORD1, float3 Normal : TEXCOORD2, float3 posW : TEXCOORD3) : COLOR0
{
	//float4 texColor = float4(0, 0, 0, 1);
	float4 splatColor = tex2D(SplatS, TexSplat);
	float4 texColor = splatColor.r * tex2D(ColorS0, Tex) + splatColor.g * tex2D(ColorS1, Tex) + splatColor.b * tex2D(ColorS2, Tex) + splatColor.a * tex2D(ColorS3, Tex);
	
	float4 totalDiffuse = float4(0, 0, 0, 1);
	float4 color = float4(0, 0, 0, 1);
	
	Normal = normalize(Normal);
	
	for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i){
		float3 lightDir = normalize(-directionalLights[i].dir);
		float diffuse = saturate(dot(lightDir, Normal)); 
		totalDiffuse += diffuse * directionalLights[i].color;
	}
	
	for(int i = 0; i < MAX_POINT_LIGHTS; ++i){
		float3 lightDir = normalize(pointLights[i].position - posW);
		float diffuse = saturate(dot(lightDir, Normal));
		float distance = length(posW - pointLights[i].position);
		float attenuation = 1 / (pointLights[i].atten.x + pointLights[i].atten.y * distance + pointLights[i].atten.z * distance * distance);
		totalDiffuse += attenuation * diffuse * pointLights[i].color;
	}
	
	//color = splatColor;
	color = (float4(ambientLight.color, 1.0f) + mtlDiffuse * totalDiffuse) * texColor;
	//color = (float4(ambientLight.color, 1.0f) + mtlDiffuse * totalDiffuse) * tex2D(ColorS0, Tex);
	//color = (float4(ambientLight.color, 1.0f) + mtlDiffuse * totalDiffuse);
	return color;
}

technique Terrain
{
	pass P0
	{
		VertexShader = compile vs_3_0 TerrainVS();
		PixelShader = compile ps_3_0 TerrainPS();		
	}
}

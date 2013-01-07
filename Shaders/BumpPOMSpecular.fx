// BumpPOMSpecular.fx
// Parallax Occlusion Mapping

#include"LightsDefine.fx"

float4x4 matWVP;
float4x4 matWorld;
float4x4 matUVTransform;

texture colorTex;
texture heightTex;
texture normalTex;
float4 mtlDiffuse;
float4 mtlSpec;
float gloss;

float parallaxHeightScale;
float parallaxHeightOffset;    

float3 eyePos;

AmbientLight ambientLight;
DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
PointLight pointLights[MAX_POINT_LIGHTS];

static const int minNumSamples = 8;
static const int maxNumSamples = 40;
static const float occlutionShadowFactor = 0.5f;

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

sampler HeightS = sampler_state
{
	Texture = <heightTex>;
	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler NormalS = sampler_state
{
	Texture = <normalTex>;
	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

void BumpPOMSpecularVS(float3 Pos : POSITION0, 
			float2 Tex : TEXCOORD0,
			float3 Tangent : TANGENT0,
			float3 Bitangent : BINORMAL0,
			float3 Normal : NORMAL0,
			out float4 oPos  : POSITION0, 
			out float2 oTex : TEXCOORD0,
			out float3 oTangent : TEXCOORD1,
			out float3 oBitangent : TEXCOORD2,
			out float3 oNormal : TEXCOORD3,
			out float3 oPosW : TEXCOORD4)
{
	oPos = mul(float4(Pos, 1.0f), matWVP);
	oPosW = (mul(float4(Pos, 1.0f), matWorld)).xyz;
	oTex = (mul(float4(Tex, 0, 1.0f), matUVTransform)).xy;
	oTangent = (mul(float4(Tangent, 0), matWorld)).xyz;
	oBitangent = (mul(float4(Bitangent, 0), matWorld)).xyz;
	oNormal = (mul(float4(Normal, 0), matWorld)).xyz;
}

float4 BumpPOMSpecularPS(float2 Tex : TEXCOORD0, 
				float3 Tangent : TEXCOORD1, 
				float3 Bitangent : TEXCOORD2, 
				float3 Normal : TEXCOORD3, 
				float3 posW : TEXCOORD4) : COLOR0
{
	float4 totalDiffuse = float4(0, 0, 0, 1);
	float4 totalSpec = float4(0, 0, 0, 1);
	
	float3 eyeDir = normalize(eyePos - posW);
	
	float3x3 TBN;
	TBN[0] = Tangent;
	TBN[1] = Bitangent;
	TBN[2] = Normal;
	
	float3x3 worldToTangent	= transpose(TBN);
	float3 eyeDirT = mul(eyeDir, worldToTangent);
	float3 parallaxDir = normalize(float3(eyeDirT.xy, 0));
	float parallaxLength = sqrt(length(eyeDirT) * length(eyeDirT) - eyeDirT.z * eyeDirT.z) / eyeDirT.z;
	parallaxDir *= parallaxLength;
	
	
	int numSamples = (int)lerp(maxNumSamples, minNumSamples, dot(eyeDir, Normal));
	int sampleIx = 0;
	
	float2 curUV = Tex;
	float curHeight = 0;
	float preHeight = tex2D(HeightS, curUV).r;
	float curBound = 1.0f;
	float heightStep = 1.0f / numSamples;
	float2 maxUVOffset = parallaxHeightScale * tex2D(HeightS, Tex).r * parallaxDir.xy;
	float2 uvOffsetStep = heightStep * maxUVOffset;
	
	float2 curSample = 0;
	float2 preSample = 0;
	
	float2 dx = ddx(Tex);
	float2 dy = ddy(Tex);
	
	while(sampleIx < numSamples){
		curUV += uvOffsetStep;
		curHeight = tex2Dgrad(HeightS, curUV, dx, dy).r;
		curBound -= heightStep;
		
		if(curHeight > curBound){
			curSample = float2(curBound, curHeight);
			preSample = float2(curBound + heightStep, preHeight);
			
			sampleIx = numSamples;
		}
		else{
			sampleIx++;
			preHeight = curHeight;
		}
	}
	
	float parallaxAmount = 1.0f - (curSample.y * preSample.x - curSample.x * preSample.y) / (curSample.y - curSample.x + preSample.x - preSample.y);
	
	Tex += parallaxAmount * maxUVOffset;
	
	Normal = 2*tex2D(NormalS, Tex) - 1.0f;
	Normal = mul(Normal, TBN);
	Normal = normalize(Normal);
	
	float2 lightRayBaseTex = Tex;
	float temp = 0;
	for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i){
	
		float occlutionShadow = 1.0f;
		float3 lightDir = normalize(-directionalLights[i].dir);
		float3 lightDirT = mul(lightDir, worldToTangent);
		float2 lightRay = 10.0f * lightDirT.xy * parallaxHeightScale;
			
		float sh0 = tex2Dgrad(HeightS, lightRayBaseTex, dx, dy).r;
		float shA = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.88f, dx, dy).r - sh0 - 0.88f) * 1.0f * occlutionShadowFactor;
		float sh9 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.77f, dx, dy).r - sh0 - 0.77f) * 2.0f * occlutionShadowFactor;
		float sh8 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.66f, dx, dy).r - sh0 - 0.66f) * 4.0f * occlutionShadowFactor;
		float sh7 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.55f, dx, dy).r - sh0 - 0.55f) * 6.0f * occlutionShadowFactor;
		float sh6 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.44f, dx, dy).r - sh0 - 0.44f) * 8.0f * occlutionShadowFactor;
		float sh5 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.33f, dx, dy).r - sh0 - 0.33f) * 10.0f * occlutionShadowFactor;
		float sh4 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.22f, dx, dy).r - sh0 - 0.22f) * 12.0f * occlutionShadowFactor;
			
		occlutionShadow = saturate(1 - max(max(max(max(max(max(shA, sh9), sh8), sh7), sh6), sh5), sh4));
		occlutionShadow = occlutionShadow * 0.6f + 0.4f;
		
		if(i == 0){	
			temp = occlutionShadow;
		}
	
	
		float3 halfVec = normalize(lightDir + eyeDir);
		
		float diffuse = saturate(dot(lightDir, Normal)); 
		float spec;
		if(dot(lightDir, Normal) > 0)
			spec = pow(saturate(dot(halfVec, Normal)), gloss);
		else
			spec = 0;
		
		totalDiffuse += occlutionShadow * diffuse * directionalLights[i].color;
		totalSpec += occlutionShadow * spec * directionalLights[i].color;
	}
	
	for(int i = 0; i < MAX_POINT_LIGHTS; ++i){
	
		float occlutionShadow = 1.0f;
		float3 lightDir = normalize(pointLights[i].position - posW);
		float3 lightDirT = mul(lightDir, worldToTangent);
		float2 lightRay = 10.0f * lightDirT.xy * parallaxHeightScale;
			
		float sh0 = tex2Dgrad(HeightS, lightRayBaseTex, dx, dy).r;
		float shA = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.88f, dx, dy).r - sh0 - 0.88f) * 1.0f * occlutionShadowFactor;
		float sh9 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.77f, dx, dy).r - sh0 - 0.77f) * 2.0f * occlutionShadowFactor;
		float sh8 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.66f, dx, dy).r - sh0 - 0.66f) * 4.0f * occlutionShadowFactor;
		float sh7 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.55f, dx, dy).r - sh0 - 0.55f) * 6.0f * occlutionShadowFactor;
		float sh6 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.44f, dx, dy).r - sh0 - 0.44f) * 8.0f * occlutionShadowFactor;
		float sh5 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.33f, dx, dy).r - sh0 - 0.33f) * 10.0f * occlutionShadowFactor;
		float sh4 = (tex2Dgrad(HeightS, lightRayBaseTex + lightRay * 0.22f, dx, dy).r - sh0 - 0.22f) * 12.0f * occlutionShadowFactor;
			
		occlutionShadow = saturate(1 - max(max(max(max(max(max(shA, sh9), sh8), sh7), sh6), sh5), sh4));
		occlutionShadow = occlutionShadow * 0.6f + 0.4f;
		
		//float3 lightDir = normalize(pointLights[i].position - posW);
		float3 halfVec = normalize(lightDir + eyeDir);
		
		float diffuse = saturate(dot(lightDir, Normal));
		float spec;
		if(dot(lightDir, Normal) > 0)
			spec = pow(saturate(dot(halfVec, Normal)), gloss);
		else
			spec = 0;
			
		float distance = length(posW - pointLights[i].position);
		float attenuation = 1 / (pointLights[i].atten.x + pointLights[i].atten.y * distance + pointLights[i].atten.z * distance * distance);
		
		totalDiffuse += attenuation * diffuse * pointLights[i].color;
		totalSpec += attenuation * spec * pointLights[i].color;
	}
	
	float4 color = (float4(ambientLight.color, 1.0f) + mtlDiffuse * totalDiffuse) * tex2D(ColorS, Tex) + mtlSpec * totalSpec;
	return color;
	return float4(temp, temp, temp, 1);
}

technique BumpPOMSpecular
{
	pass P0
	{
		VertexShader = compile vs_3_0 BumpPOMSpecularVS();
		PixelShader = compile ps_3_0 BumpPOMSpecularPS();		
	}
}

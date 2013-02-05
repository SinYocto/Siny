// CubeRefractEM.fx

float4x4 matWVP;
float4x4 matWorld;
texture cubeTex;

float3 eyePos;

float etaRatio;

samplerCUBE CubeMapS = sampler_state
{
	Texture = <cubeTex>;

	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = Clamp;
	AddressV  = Clamp;
};


void CubeRefractEMVS(float3 Pos : POSITION0, 
			float3 Normal : NORMAL0,
			out float4 oPos  : POSITION0, 
			out float3 oNormal : TEXCOORD0,
			out float3 oPosW : TEXCOORD1)
{
	oPos = mul(float4(Pos, 1.0f), matWVP);
	oPosW = (mul(float4(Pos, 1.0f), matWorld)).xyz;
	oNormal = (mul(float4(Normal, 0), matWorld)).xyz;
}

float4 CubeRefractEMPS(float3 Normal : TEXCOORD0, float3 posW : TEXCOORD1) : COLOR0
{
	float4 color = float4(0, 0, 0, 1);
	
	Normal = normalize(Normal);
	float3 eyeDir = normalize(posW - eyePos);
	float3 refracttDir = refract(eyeDir, Normal, etaRatio);
	
	color = texCUBE(CubeMapS, refracttDir);
	
	return color;
}

technique CubeRefractEM
{
	pass P0
	{
		VertexShader = compile vs_3_0 CubeRefractEMVS();
		PixelShader = compile ps_3_0 CubeRefractEMPS();		
	}
}

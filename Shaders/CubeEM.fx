// CubeEM.fx

float4x4 matWVP;
float4x4 matWorld;
texture cubeTex;

float3 eyePos;

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


void CubeEMVS(float3 Pos : POSITION0, 
			float3 Normal : NORMAL0,
			out float4 oPos  : POSITION0, 
			out float3 oNormal : TEXCOORD0,
			out float3 oPosW : TEXCOORD1)
{
	oPos = mul(float4(Pos, 1.0f), matWVP);
	oPosW = (mul(float4(Pos, 1.0f), matWorld)).xyz;
	oNormal = (mul(float4(Normal, 0), matWorld)).xyz;
}

float4 CubeEMPS(float3 Normal : TEXCOORD0, float3 posW : TEXCOORD1) : COLOR0
{
	float4 color = float4(0, 0, 0, 1);
	
	Normal = normalize(Normal);
	float3 eyeDir = posW - eyePos;
	float3 reflectDir = reflect(eyeDir, Normal);
	
	color = texCUBE(CubeMapS, reflectDir);
	
	return color;
}

technique CubeEM
{
	pass P0
	{
		VertexShader = compile vs_3_0 CubeEMVS();
		PixelShader = compile ps_3_0 CubeEMPS();		
	}
}

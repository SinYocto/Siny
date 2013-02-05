// IrradianceEM.fx

float4x4 matWVP;
float4x4 matWorld;
texture diffCubeTex;
texture specCubeTex;

samplerCUBE DiffCubeMapS = sampler_state
{
	Texture = <diffCubeTex>;

	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = Clamp;
	AddressV  = Clamp;
};

samplerCUBE SpecCubeMapS = sampler_state
{
	Texture = <specCubeTex>;

	MinFilter = linear;
	MaxAnisotropy = 4;
	MagFilter = linear;
	MipFilter = linear;
	AddressU  = Clamp;
	AddressV  = Clamp;
};

void IrradianceEMVS(float3 Pos : POSITION0, 
			float3 Normal : NORMAL0,
			out float4 oPos  : POSITION0, 
			out float3 oNormal : TEXCOORD0)
{
	oPos = mul(float4(Pos, 1.0f), matWVP);
	oNormal = (mul(float4(Normal, 0), matWorld)).xyz;
}

float4 IrradianceEMPS(float3 Normal : TEXCOORD0) : COLOR0
{
	float4 color = float4(0, 0, 0, 1);
	
	float3 diff = texCUBE(DiffCubeMapS, Normal).rgb;
	float3 spec = texCUBE(SpecCubeMapS, Normal).rgb;
	
	color.rgb = diff + spec;
	return color;
}

technique IrradianceEM
{
	pass P0
	{
		VertexShader = compile vs_3_0 IrradianceEMVS();
		PixelShader = compile ps_3_0 IrradianceEMPS();		
	}
}

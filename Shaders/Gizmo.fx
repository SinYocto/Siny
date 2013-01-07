// Gizmo.fx

float4x4 matWVP;
float4x4 matWorld;
float4 mainColor;
float3 eyePos;

void GizmoVS(float3 Pos : POSITION0, 
			float3 Normal : NORMAL0,
			out float4 oPos  : POSITION0, 
			out float3 oNormal : TEXCOORD1,
			out float3 oPosW : TEXCOORD2)
{
	oPos = mul(float4(Pos, 1.0f), matWVP);
	oPosW = (mul(float4(Pos, 1.0f), matWorld)).xyz;
	oNormal = (mul(float4(Normal, 0), matWorld)).xyz;
}

float4 GizmoPS(float3 Normal : TEXCOORD1, float3 posW : TEXCOORD2) : COLOR0
{
	float3 dir = normalize(eyePos - posW);
	float diffuse = saturate(dot(dir, Normal));
	return diffuse*mainColor;
}

technique Gizmo
{
	pass P0
	{
		VertexShader = compile vs_2_0 GizmoVS();
		PixelShader = compile ps_2_0 GizmoPS();		
	}
}

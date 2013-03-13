// ColorPickerHue.fx

float Hue2RGB(float v1, float v2, float vh)
{
	if(vh < 0)
		vh = vh + 1;
	if(vh > 1)
		vh = vh - 1;
	
	if(6.0f*vh < 1)
		return v1 + (v2 - v1)*6.0f*vh;
	if(2.0f*vh < 1)
		return v2;
	if(3.0f*vh < 2)
		return v1 + (v2 - v1)*(2.0f/3.0f - vh)*6.0f;
		
	return v1;
}

float3 HSL2RGB(float3 hsl)
{
	float h = hsl.x;
	float s = hsl.y;
	float l = hsl.z;
	
	if(s == 0){
		return float3(l, l, l);
	}
	
	float v1, v2;
	if(l < 0.5f)
		v2 = l*(1 + s);
	else
		v2 = (l + s) - s*l;
	v1 = 2*l - v2;
	
	float r = Hue2RGB(v1, v2, h + 1.0f/3.0f);
	float g = Hue2RGB(v1, v2, h);
	float b = Hue2RGB(v1, v2, h - 1.0f/3.0f);
	
	return float3(r, g, b);
}

float4 ColorPickerHuePS(float2 Tex : TEXCOORD0) : COLOR0
{
	float4 color = float4(0, 0, 0, 1);
	color.rgb = HSL2RGB(float3(1 - Tex.y, 1.0f, 0.5f));
	return color;
}

technique ColorPickerHue
{
	pass P0
	{
		PixelShader = compile ps_3_0 ColorPickerHuePS();		
	}
}
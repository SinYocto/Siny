#include"DeviceInfo.h"
#include"stdio.h"
#include"D3DUtility.h"


float DeviceInfo::aspect = 1.0f;

void DeviceInfo::GetDisplayModeInfo()
{
	D3DDISPLAYMODE d3ddm;
	if(FAILED(D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))		
		printf("GetAdapterDisplayMode() - Failed!\n");
	else{
		aspect = (float)d3ddm.Width / (float)d3ddm.Height;
	}
}
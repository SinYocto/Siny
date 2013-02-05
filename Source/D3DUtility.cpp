#include"D3DUtility.h"
#include"DeviceInfo.h"

/**
 * 注册和创建窗口，初始化D3D
 */
int InitD3D(HINSTANCE hInstance, bool windowed, int screenWidth, int screenHeight, _D3DMULTISAMPLE_TYPE multisampleType, DWORD multisampleQuality, 
			HWND &hWnd, IDirect3D9** D3D, IDirect3DDevice9** d3dDevice)
{
	// 获取D3D接口
	*D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!(*D3D)){
		MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return 0;
	}

	// 获取显示模式
	D3DDISPLAYMODE d3ddm;
	if(FAILED((*D3D)->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return 0;

	DeviceInfo::GetDisplayModeInfo();


	// 窗口的注册和创建
	WNDCLASSEX WndClass;

	WndClass.cbSize			= sizeof(WNDCLASSEX);
	WndClass.style			= CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc	= WndProc;
	WndClass.cbClsExtra		= 0;
	WndClass.cbWndExtra		= 0;
	WndClass.hInstance		= hInstance;
	WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor		= LoadCursor(hInstance, IDC_ARROW);
	WndClass.hbrBackground  = NULL;
	WndClass.lpszMenuName	= NULL;
	WndClass.lpszClassName  = "Siny";
	WndClass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&WndClass)){
		MessageBox(0, "RegisterClassEx()- FAILED", 0, 0);
		return 0;
	}

	hWnd = CreateWindow("Siny", "Siny", WS_CAPTION | WS_SYSMENU, 0, 0, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);
	if(!hWnd){
		MessageBox(0, "CreateWindow() - FAILED", 0, 0);
		return 0;
	}
	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);


	// 设备能力检查
	// --顶点运算方式
	D3DCAPS9 d3dcaps;
	(*D3D)->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);
	int vp = 0;
	if(d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else 
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// --多重采样
	_D3DMULTISAMPLE_TYPE multisample = multisampleType;
	if( SUCCEEDED((*D3D)->CheckDeviceMultiSampleType( d3dcaps.AdapterOrdinal, d3dcaps.DeviceType, D3DFMT_A8B8G8R8, FALSE, multisampleType, NULL ) ) )
		multisample = multisampleType;
	else
		multisample = D3DMULTISAMPLE_NONE;


	// 填写PARAMETERS结构
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed					= windowed;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferWidth			= d3ddm.Width;
	d3dpp.BackBufferHeight			= d3ddm.Height;
	d3dpp.BackBufferFormat			= d3ddm.Format;
	d3dpp.MultiSampleType			= multisample;
	d3dpp.MultiSampleQuality		= multisampleQuality;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;


	// 获取设备接口
	if(FAILED((*D3D)->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, d3dDevice)))
	{
		MessageBox(0, "CreateDevice() - FAILED",0 ,0);
		return 0;
	}

	return 1;
}


/**
 * 消息处理
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 
	
	case WM_ACTIVATEAPP:
		if(DID_Keyboard) 
			DID_Keyboard->Acquire();
		if(DID_Mouse) 
			DID_Mouse->Acquire();
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/**
 * 创建顶点缓存
 */
void CreateVB(IDirect3DDevice9* d3dDevice,IDirect3DVertexBuffer9** vb, void* vertexData, int nVertices, VertexType vertexType)
{
	int sizeofVertex;
	DWORD vertexFVF;
	switch(vertexType){
		case XYZ:
			sizeofVertex = 12;
			vertexFVF = D3DFVF_XYZ;
			break;
		case XYZ_UV:
			sizeofVertex = 20;
			vertexFVF = D3DFVF_XYZ | D3DFVF_TEX1;
			break;
		case XYZ_N:
			sizeofVertex = 24;
			vertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL;
			break;
		case XYZ_UV_N:
			sizeofVertex = 32;
			vertexFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;
			break;
		case XYZ_UV_TBN:
			sizeofVertex = 56;
			vertexFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;
			break;
		default:
			break;
	}

	void *ptr;
	d3dDevice->CreateVertexBuffer(sizeofVertex*nVertices, 0,
                          vertexFVF, D3DPOOL_DEFAULT, vb, 0);
	(*vb)->Lock(0,0, (void**)&ptr, 0);
	memcpy(ptr, vertexData, sizeofVertex*nVertices);
	(*vb)->Unlock();
}

/** 
 * 创建索引缓存
 */
void CreateIB(IDirect3DDevice9* d3dDevice, IDirect3DIndexBuffer9** ib, DWORD* indexData, int nIndices)
{
	BYTE* ptr;
	d3dDevice->CreateIndexBuffer(sizeof(DWORD)*nIndices, 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, ib, 0);
	(*ib)->Lock(0, 0, (void**)&ptr, 0);
	memcpy(ptr, indexData, sizeof(DWORD)*nIndices);
	(*ib)->Unlock();
}



D3DCOLOR ColorLerp(D3DCOLOR color1, D3DCOLOR color2, float factor)
{
	if(factor > 1)
		factor = 1;
	if(factor < 0)
		factor = 0;

	BYTE a1 = (color1>>24) & 0xff;
	BYTE r1 = (color1>>16) & 0xff;
	BYTE g1 = (color1>>8) & 0xff;
	BYTE b1 = (color1) & 0xff;

	BYTE a2 = (color2>>24) & 0xff;
	BYTE r2 = (color2>>16) & 0xff;
	BYTE g2 = (color2>>8) & 0xff;
	BYTE b2 = (color2) & 0xff;

	BYTE a = (BYTE)((1-factor)*a1 + factor*a2);
	BYTE r = (BYTE)((1-factor)*r1 + factor*r2);
	BYTE g = (BYTE)((1-factor)*g1 + factor*g2);
	BYTE b = (BYTE)((1-factor)*b1 + factor*b2);

	D3DCOLOR color;

	((BYTE*)&color)[0] = b;		// B
	((BYTE*)&color)[1] = g;			// G
	((BYTE*)&color)[2] = r;			// R
	((BYTE*)&color)[3] = a;			// A

	return color;
}


float Lerp(float value1, float value2, float factor)
{
	return (1-factor)*value1 + factor*value2;
}


void CreateCubeTextureFromFile(IDirect3DCubeTexture9 *&cubeTex, int cubeTexSize, string filenamePX, string filenameNX, string filenamePY, string filenameNY, string filenamePZ, string filenameNZ)
{
	if(cubeTex)
		SAFE_RELEASE(cubeTex);

	D3DDevice->CreateCubeTexture(cubeTexSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &cubeTex, NULL);

	IDirect3DSurface9 *cubeTexSurf;

	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenamePX.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenameNX.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenamePY.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenameNY.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenamePZ.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	
	cubeTex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &cubeTexSurf);
	D3DXLoadSurfaceFromFile(cubeTexSurf, NULL, NULL, filenameNZ.c_str(), NULL, D3DX_DEFAULT, 0, NULL);

	SAFE_RELEASE(cubeTexSurf);
}
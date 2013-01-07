#ifndef D3DUTILITY_H
#define D3DUTILITY_H

#include<windows.h>
#include"d3d9.h"
#include"d3dx9.h"
#include"Input.h"


extern HWND HWnd;
extern IDirect3D9 *D3D;
extern IDirect3DDevice9 *D3DDevice;


#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z)
	{
		_x = x; _y = y; _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF = D3DFVF_XYZ;
};

struct VertexUV
{
	VertexUV(){}
	VertexUV(float x, float y, float z, float u, float v)
	{
		_x = x; _y = y; _z = z; _u = u; _v = v;
	}
	float _x, _y, _z, _u, _v;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};

struct VertexUVN
{	
	VertexUVN(){}
	VertexUVN(float x, float y, float z, float u, float v, float nx, float ny, float nz)
	{
		_x = x; _y = y; _z = z; _u = u; _v = v; _nx = nx; _ny = ny; _nz = nz;
	}
	float _x, _y, _z, _u, _v, _nx, _ny, _nz;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;

};

struct VertexUVTBN
{
	VertexUVTBN() {}
	VertexUVTBN(float x, float y, float z, float u, float v, float tx, float ty, float tz, float bx, float by, float bz, float nx, float ny, float nz)
	{
		_x = x; _y = y; _z = z; _u = u; _v = v; _tx = tx; _ty = ty; _tz = tz; _bx = bx; _by = by; _bz = bz; _nx = nx; _ny = ny; _nz = nz;
	}
	float _x, _y, _z, _u, _v, _tx, _ty, _tz, _bx, _by, _bz, _nx, _ny, _nz;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;
	
};

struct VertexN
{
	VertexN(){}
	VertexN(float x, float y, float z, float nx, float ny, float nz)
	{
		_x = x; _y = y; _z = z; _nx = nx; _ny = ny; _nz = nz;
	}
	float _x, _y, _z, _nx, _ny, _nz;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
};

enum VertexType { XYZ, XYZ_UV, XYZ_N, XYZ_UV_N, XYZ_UV_TBN };

int	InitD3D(HINSTANCE hInstance, bool windowed, int screenWidth, int screenHeight, _D3DMULTISAMPLE_TYPE multisampleType, DWORD multisampleQuality, 
			HWND &hWnd, IDirect3D9** d3d, IDirect3DDevice9** d3dDevice);
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void CreateVB(IDirect3DDevice9* d3dDevice, IDirect3DVertexBuffer9** vb, void* VertexData, int nVertexs, VertexType vertexType);
void CreateIB(IDirect3DDevice9* d3dDevice, IDirect3DIndexBuffer9** ib, WORD* IndexData, int nIndices);

D3DCOLOR ColorLerp(D3DCOLOR color1, D3DCOLOR color2, float factor);
float Lerp(float value1, float value2, float factor);


#endif
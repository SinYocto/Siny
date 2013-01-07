#include"SkyDome.h"
#include"Scene.h"

DayTime DayTime::curDayTime;

void SkyDome::Render(bool isCloudDome)
{
	position = scene.mainCamera.position;
	rotation = Quaternion(0, 0, 0);

	D3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	if(isCloudDome){
		D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	}

	D3DDevice->SetTransform(D3DTS_WORLD, &(LocalToWorldMatrix()));
	D3DDevice->SetTransform(D3DTS_VIEW, &(scene.mainCamera.ViewMatrix()));
	D3DDevice->SetTransform(D3DTS_PROJECTION, &(scene.mainCamera.ProjMatrix()));
	D3DDevice->SetStreamSource(0, vertexBuffer, 0, 20);
	D3DDevice->SetFVF(VertexUV::FVF);
	D3DDevice->SetIndices(indexBuffer);
	D3DDevice->SetTexture(0, skyTex);

	D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numTriangles);

	if(isCloudDome){
		D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		return;
	}

	rotation = Quaternion(PI, 0, 0);
	D3DDevice->SetTransform(D3DTS_WORLD, &(LocalToWorldMatrix()));

	D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numTriangles);

}
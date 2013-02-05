#ifndef IRRADIANCEEM_H
#define IRRANDIANCEEM_H

#include"D3DUtility.h"

#define N_ORDER 3
#define N_ORDER_P2 4
#define NUM_PARA_SAMPLES 64
#define NUM_RESULT_SAMPLES 32
#define NUM_RADIANCE_SAMPLES 32

enum IrradianceTexType { IrradianceLambert, IrradiancePhong };

class IrradianceEM
{
public:
	IrradianceEM();
	void Init();
	void BuildParaboloidWeightTexs();
	void BuildLambertIrradianceTexs();
	void BuildPhongIrradianceTexs();
	void RenderParaboloidEnvMap();
	void ProjectParaboloidToSH();
	void EvaluateConvolvedSH(IrradianceTexType type);

private:
	bool ParaVecFromCoord(D3DXVECTOR3 *vec, int face, float u, float v);
	void CubeVecFromCoord(D3DXVECTOR3 *vec, int face, float u, float v);
	void BuildIrradianceTexs(IrradianceTexType type);
	float lamberAlEvaluator(int l);
	float phongAlEvaluator(int l, float specExponent);
public:
	IDirect3DCubeTexture9 *baseCubeTex;
	IDirect3DCubeTexture9 *diffCubeTex;
	IDirect3DCubeTexture9 *specCubeTex;
	IDirect3DTexture9*	paraboloidTex[2];

	IDirect3DTexture9*	paraboloidSHWeights[2];
	IDirect3DTexture9*	lambertSHEval[6];
	IDirect3DTexture9*	phongSHEval[6];
	IDirect3DTexture9*	irradianceSHCoef;

	LPD3DXEFFECT paraboloidEffect;
	LPD3DXEFFECT convolveEffect;

	float specExponent;
};

extern IrradianceEM irrEM;


#endif
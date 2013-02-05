#include"IrradianceEM.h"
#include"Math.h"
#include<stdio.h>

IrradianceEM::IrradianceEM()
{
	specExponent = 30.0f;
}

void IrradianceEM::Init()
{
	CreateCubeTextureFromFile(baseCubeTex, 64, "./Textures/indoor_right.jpg", "./Textures/indoor_left.jpg", "./Textures/indoor_up.jpg", 
		"./Textures/indoor_down.jpg", "./Textures/indoor_back.jpg", "./Textures/indoor_front.jpg");
	D3DDevice->CreateTexture(NUM_PARA_SAMPLES, NUM_PARA_SAMPLES, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &paraboloidTex[0], NULL);
	D3DDevice->CreateTexture(NUM_PARA_SAMPLES, NUM_PARA_SAMPLES, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &paraboloidTex[1], NULL);
	D3DDevice->CreateCubeTexture(NUM_RESULT_SAMPLES, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &diffCubeTex, NULL);
	D3DDevice->CreateCubeTexture(NUM_RESULT_SAMPLES, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &specCubeTex, NULL);
	D3DDevice->CreateTexture(N_ORDER_P2, N_ORDER_P2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &irradianceSHCoef, NULL);

	D3DXCreateEffectFromFile(D3DDevice, "./Shaders/RenderParaboloid.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL, &paraboloidEffect, NULL);
	D3DXCreateEffectFromFile(D3DDevice, "./Shaders/ConvolveIrradiance.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL, &convolveEffect, NULL);
}

void IrradianceEM::BuildParaboloidWeightTexs()
{
	int nOrder = N_ORDER;
	int nOrderP2 = N_ORDER_P2;
	int size = NUM_RADIANCE_SAMPLES;

	float *dOmega = new float[size*size];
	float dOmegaSum = 0;

	for(int s = 0; s < size; ++s){
		for(int t = 0; t < size; ++t){
			float x = ((s + 0.5f) / size)*2.0f - 1.0f;
			float y = ((t + 0.5f) / size)*2.0f - 1.0f;
			float rSqr = x*x + y*y;

			int ix = t*size + s;
			if(rSqr > 1.0f){
				dOmega[ix] = 0;
				continue;
			}

			float z = 0.5f*(1 - rSqr);
			float mag = sqrt(rSqr + z*z);

			float cosTheta = 1.0f;
			dOmega[ix] = cosTheta / (mag*mag);
			dOmegaSum += dOmega[ix];
		}
	}

	float dOmegaScale = 4*PI / (2*dOmegaSum);

	// SH basis
	float *basisProj = new float[nOrder*nOrder];		 

	for(int face = 0; face < 2; ++face){
		D3DDevice->CreateTexture(nOrderP2*size, nOrderP2*size, 1, 0, D3DFMT_R32F, D3DPOOL_MANAGED, &paraboloidSHWeights[face], NULL);

		float *coefs = new float[nOrderP2*size * nOrderP2*size];
		ZeroMemory(coefs, nOrderP2*nOrderP2*size*size*sizeof(float));

		for(int s = 0; s < size; ++s){
			for(int t = 0; t < size; ++t){
				D3DXVECTOR3	paraVec;
				float x = ((s + 0.5f) / size)*2.0f - 1.0f;
				float y = ((t + 0.5f) / size)*2.0f - 1.0f;

				if(!ParaVecFromCoord(&paraVec, face, x, y))
					continue;

				// evaluate SH basis from a given direction and nOrder(nOrder*nOrder basis)
				D3DXSHEvalDirection(basisProj, nOrder, &paraVec);

				int basisIx = 0;
				int sampleIx = t*size + s;
				for(int l = 0; l < nOrder; ++l){
					for(int m = -l; m <= l; ++m, ++basisIx){

						// first tiles layer(nOrderP2*nOrderP2 tiles)
						int tileY = basisIx / nOrder;
						int tileX = basisIx % nOrder;

						float Ylm = basisProj[l*(l+1) + m];
						int coefIx = ((tileY*size + t)*size*nOrderP2) + tileX*size + s;
						float weight = Ylm * dOmega[sampleIx] * dOmegaScale;
						coefs[coefIx] = weight;
					}
				}
			}
		}

		D3DLOCKED_RECT lockRect;
		paraboloidSHWeights[face]->LockRect(0, &lockRect, NULL, 0);

		unsigned char* dst = (unsigned char*)lockRect.pBits;
		unsigned char* src = (unsigned char*)coefs;
		unsigned int srcPitch = size * nOrderP2 * sizeof(float);

		for(int i = 0; i < size * nOrderP2; ++i, dst += lockRect.Pitch, src += srcPitch)
			memcpy(dst, src, srcPitch);

		paraboloidSHWeights[face]->UnlockRect(0);
		delete [] coefs;
	}

	delete [] basisProj;
	delete [] dOmega;
}

void IrradianceEM::BuildLambertIrradianceTexs()
{
	BuildIrradianceTexs(IrradianceLambert);
}

void IrradianceEM::BuildPhongIrradianceTexs()
{
	BuildIrradianceTexs(IrradiancePhong);
}

void IrradianceEM::BuildIrradianceTexs(IrradianceTexType type)
{
	LPDIRECT3DTEXTURE9 *weightTexs;
	if(type == IrradianceLambert)
		weightTexs = lambertSHEval;
	else
		weightTexs = phongSHEval;

	int nOrder = N_ORDER;
	int nOrderP2 = N_ORDER_P2;
	int size = NUM_RESULT_SAMPLES;
	
	// SH basis
	float *basisProj = new float[nOrder*nOrder];	

	for(int face = 0; face < 6; ++face){
		printf("face %d\n", face);
		if(FAILED(D3DDevice->CreateTexture(nOrderP2*size, nOrderP2*size, 1, 0, D3DFMT_R32F, D3DPOOL_MANAGED, &weightTexs[face], NULL))) printf("failed!\n");

		float *coefs = new float[nOrderP2*size * nOrderP2*size];
		ZeroMemory(coefs, nOrderP2*nOrderP2*size*size*sizeof(float));

		for(int s = 0; s < size; ++s){
			for(int t = 0; t < size; ++t){
				printf("face(%d) s(%d) t(%d)\n", face, s, t);
				D3DXVECTOR3	cubeVec;
				float x = ((s + 0.5f) / size)*2.0f - 1.0f;
				float y = ((t + 0.5f) / size)*2.0f - 1.0f;

				CubeVecFromCoord(&cubeVec, face, x, y);

				// evaluate SH basis from a given direction and nOrder(nOrder*nOrder basis)
				D3DXSHEvalDirection(basisProj, nOrder, &cubeVec);

				int basisIx = 0;
				for(int l = 0; l < nOrder; ++l){
					float Al;
					if(type == IrradianceLambert)
						Al = lamberAlEvaluator(l);
					else
						Al = phongAlEvaluator(l, specExponent);

					for(int m = -l; m <= l; ++m, ++basisIx){
						// first tiles layer(nOrderP2*nOrderP2 tiles)
						int tileY = basisIx / nOrder;
						int tileX = basisIx % nOrder;

						float Ylm = basisProj[l*(l+1) + m];
						int coefIx = ((tileY*size + t)*size*nOrderP2) + tileX*size + s;
						coefs[coefIx] = Ylm * Al;
					}
				}
			}
		}

		D3DLOCKED_RECT lockRect;
		weightTexs[face]->LockRect(0, &lockRect, NULL, 0);

		unsigned char* dst = (unsigned char*)lockRect.pBits;
		unsigned char* src = (unsigned char*)coefs;
		unsigned int srcPitch = size * nOrderP2 * sizeof(float);

		for(int i = 0; i < size * nOrderP2; ++i, dst += lockRect.Pitch, src += srcPitch)
			memcpy(dst, src, srcPitch);

		weightTexs[face]->UnlockRect(0);
		delete [] coefs;
	}
    delete [] basisProj;
}


bool IrradianceEM::ParaVecFromCoord(D3DXVECTOR3 *vec, int face, float x, float y)
{
	float rSqr = x*x + y*y;

	if(rSqr > 1.0f)
		return false;

	D3DXVECTOR3 axis;
	if(face == 0)
		axis = D3DXVECTOR3(0, 0, -1);
	else 
		axis = D3DXVECTOR3(0, 0, 1);

	D3DXVECTOR3 normal(x, y, 1);
	D3DXVec3Normalize(&normal, &normal);
	D3DXVECTOR3 reflect = axis - 2*D3DXVec3Dot(&axis, &normal)*normal;

	*vec = reflect;
	return true;
}

void IrradianceEM::CubeVecFromCoord(D3DXVECTOR3 *vec, int face, float x, float y)
{
    D3DXVECTOR3 tmp;
    switch (face){
    case 0: 
		tmp = D3DXVECTOR3(1.0f, -y, -x); 
		break;
    case 1: 
		tmp = D3DXVECTOR3(-1.0f, -y, x); 
		break;
    case 2: 
		tmp = D3DXVECTOR3(x, 1.0f, y); 
		break;
    case 3: 
		tmp = D3DXVECTOR3(x, -1.0f, -y); 
		break;
    case 4: 
		tmp = D3DXVECTOR3(x, -y, 1.0f); 
		break;
    case 5: 
		tmp = D3DXVECTOR3(-x, -y, -1.0f); 
		break;
    }
    D3DXVec3Normalize(&tmp, &tmp);
    vec->x =  tmp.z;
    vec->y = -tmp.x;
    vec->z =  tmp.y;
}

float IrradianceEM::lamberAlEvaluator(int l)
{   
	if (l<0)       // bogus case
        return 0;

	if(l == 0)
		return PI;

    if ( (l&1)==1 )
    {
        if (l==1) 
            return 2.0f*PI / 3.0f;
        else 
            return 0;
    }
    else  // l is even
    {
        float l_fac = (float)factorial((unsigned int)l);
        float l_over2_fac = (float)factorial((unsigned int)(l>>1));
        float denominator = (float) (l+2)*(l-1);
        float sign = ( (l>>1) & 1 )?1.f : -1.f;  // -1^(l/2 - 1) = -1 when l is a multiple of 4, 1 for other multiples of 2
        float exp2_l = (float)(1 << (unsigned int)l);
        return (sign*2.0f*PI*l_fac) / (denominator*exp2_l*l_over2_fac);
    }
}

float IrradianceEM::phongAlEvaluator(int l, float specExponent)
{
    if ( l<0 ) return 0;

    float lambda = sqrt(4.0f*PI/(2.0f*float(l)+1.0f));
    float x = -float(l*l) / (2.0f*specExponent);
    return exp(x) / lambda;
}

void IrradianceEM::RenderParaboloidEnvMap()
{
	
	LPDIRECT3DSURFACE9 pOldRT = NULL;
	LPDIRECT3DSURFACE9 pOldDS = NULL;
	D3DDevice->GetRenderTarget(0, &pOldRT);
	D3DDevice->GetDepthStencilSurface(&pOldDS);
	
	D3DDevice->BeginScene();
    for (int face = 0; face < 2; ++face){ 
		LPDIRECT3DSURFACE9 pRTSurf = NULL;
		paraboloidTex[face]->GetSurfaceLevel( 0, &pRTSurf );
		D3DDevice->SetRenderTarget(0, pRTSurf);
		D3DDevice->SetDepthStencilSurface(NULL);
        
        D3DDevice->Clear(0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0xff,0x7f,0x7f,0xff), 1.0f, 0L);
		paraboloidEffect->SetTechnique("ConvertHemisphere");
		paraboloidEffect->SetTexture("CubeMap", baseCubeTex);

        D3DXVECTOR4 vDirectionVec(0.f, 0.f, (face)?1.f : -1.f, 1.f);
		paraboloidEffect->SetVector("DirectionVec", &vDirectionVec);

        paraboloidEffect->Begin(0, 0);
		paraboloidEffect->BeginPass(0);

		D3DSURFACE_DESC surfDesc;
		pRTSurf->GetDesc( &surfDesc );
	
		D3DXVECTOR4 fsQuad[4];
		fsQuad[0] = D3DXVECTOR4( 0, 0, 0.5f, 1.f );
		fsQuad[1] = D3DXVECTOR4(  (float)surfDesc.Width, 0, 0.5f, 1.f );
		fsQuad[2] = D3DXVECTOR4( 0,  (float)surfDesc.Height, 0.5f, 1.f );
		fsQuad[3] = D3DXVECTOR4( (float)surfDesc.Width,  (float)surfDesc.Height, 0.5f, 1.f );
		D3DDevice->SetFVF(D3DFVF_XYZRHW);
		D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, fsQuad, sizeof(D3DXVECTOR4));
		
		paraboloidEffect->EndPass();
		paraboloidEffect->End();
        pRTSurf->Release();
    }
	D3DDevice->EndScene();
	
	D3DDevice->SetDepthStencilSurface( pOldDS );
	D3DDevice->SetRenderTarget(0, pOldRT);
}

void IrradianceEM::ProjectParaboloidToSH()
{
	
	LPDIRECT3DSURFACE9 pOldRT = NULL;
	LPDIRECT3DSURFACE9 pOldDS = NULL;
	D3DDevice->GetRenderTarget(0, &pOldRT);
	D3DDevice->GetDepthStencilSurface(&pOldDS);

	UINT uPasses;

    LPDIRECT3DSURFACE9 pRTSurf = NULL;
	irradianceSHCoef->GetSurfaceLevel(0, &pRTSurf);
	D3DDevice->SetRenderTarget(0, pRTSurf);
	D3DDevice->SetDepthStencilSurface(NULL);
	
	D3DDevice->BeginScene();
    D3DDevice->Clear(0L, NULL, D3DCLEAR_TARGET, 0x00000000, 1.f, 0L);
	
	convolveEffect->SetTechnique("ProjectDualParaboloidToSH");
	convolveEffect->SetTexture("SH_Convolve_dE_0", paraboloidTex[0]);
	convolveEffect->SetTexture("SH_Convolve_dE_1", paraboloidTex[1]);
	convolveEffect->SetTexture("SH_Convolve_Ylm_dW_0", paraboloidSHWeights[0]);
	convolveEffect->SetTexture("SH_Convolve_Ylm_dW_1", paraboloidSHWeights[1]);

    convolveEffect->Begin(&uPasses, 0);
	for(UINT i=0; i<uPasses; i++ ){
		convolveEffect->BeginPass(i);

		D3DSURFACE_DESC surfDesc;
		pRTSurf->GetDesc( &surfDesc );
	
		D3DXVECTOR4 fsQuad[4];
		fsQuad[0] = D3DXVECTOR4( 0, 0, 0.5f, 1.f );
		fsQuad[1] = D3DXVECTOR4(  (float)surfDesc.Width, 0, 0.5f, 1.f );
		fsQuad[2] = D3DXVECTOR4( 0,  (float)surfDesc.Height, 0.5f, 1.f );
		fsQuad[3] = D3DXVECTOR4( (float)surfDesc.Width,  (float)surfDesc.Height, 0.5f, 1.f );
		D3DDevice->SetFVF(D3DFVF_XYZRHW);
		D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, fsQuad, sizeof(D3DXVECTOR4));

		convolveEffect->EndPass();
    }
    convolveEffect->End();
    pRTSurf->Release();

	D3DDevice->EndScene();
	D3DDevice->SetDepthStencilSurface( pOldDS );
	D3DDevice->SetRenderTarget(0, pOldRT);
}

void IrradianceEM::EvaluateConvolvedSH(IrradianceTexType type)
{
	
	LPDIRECT3DSURFACE9 pOldRT = NULL;
	LPDIRECT3DSURFACE9 pOldDS = NULL;
	D3DDevice->GetRenderTarget(0, &pOldRT);
	D3DDevice->GetDepthStencilSurface(&pOldDS);

	UINT uPasses;
	LPDIRECT3DCUBETEXTURE9 pResultCube;
	LPDIRECT3DTEXTURE9* pEvalSHFunction;
	if(type == IrradianceLambert){
		pResultCube = diffCubeTex;
		pEvalSHFunction = lambertSHEval;
	}
	else{
		pResultCube = specCubeTex;
		pEvalSHFunction = phongSHEval;
	}
	
	D3DDevice->BeginScene();
    for(UINT face = 0; face < 6; ++face){
        LPDIRECT3DSURFACE9 pRTSurf = NULL;
		pResultCube->GetCubeMapSurface((D3DCUBEMAP_FACES)face, 0, &pRTSurf);
		D3DDevice->SetRenderTarget(0, pRTSurf);
		D3DDevice->SetDepthStencilSurface(NULL);
        D3DDevice->Clear(0L, NULL, D3DCLEAR_TARGET, 0x00000000, 1.f, 0L);

        const D3DXHANDLE EvalTextureNames[6] = { "SH_Integrate_Ylm_Al_xpos",
                                                 "SH_Integrate_Ylm_Al_xneg",
                                                 "SH_Integrate_Ylm_Al_ypos",
                                                 "SH_Integrate_Ylm_Al_yneg",
                                                 "SH_Integrate_Ylm_Al_zpos",
                                                 "SH_Integrate_Ylm_Al_zneg" };
        
		convolveEffect->SetTechnique("EvaluateSHCubemap");
		convolveEffect->SetTexture(EvalTextureNames[face], pEvalSHFunction[face]);
		convolveEffect->SetTexture("SH_Coefficients", irradianceSHCoef);
        
        convolveEffect->Begin(&uPasses,0);

        convolveEffect->BeginPass(face);
		
		D3DSURFACE_DESC surfDesc;
		pRTSurf->GetDesc( &surfDesc );
	
		D3DXVECTOR4 fsQuad[4];
		fsQuad[0] = D3DXVECTOR4( 0, 0, 0.5f, 1.f );
		fsQuad[1] = D3DXVECTOR4(  (float)surfDesc.Width, 0, 0.5f, 1.f );
		fsQuad[2] = D3DXVECTOR4( 0,  (float)surfDesc.Height, 0.5f, 1.f );
		fsQuad[3] = D3DXVECTOR4( (float)surfDesc.Width,  (float)surfDesc.Height, 0.5f, 1.f );
		D3DDevice->SetFVF(D3DFVF_XYZRHW);
		D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, fsQuad, sizeof(D3DXVECTOR4));

		convolveEffect->EndPass();
        convolveEffect->End();

		static int count = 0;
		if(count < 12){
			string filename("env");
			if(type == IrradianceLambert)
				filename += "_diff_";
			else
				filename += "_spec_";
			char t[256];
			sprintf(t, "%d", face);
			filename += t;
			filename += ".bmp";

			D3DXSaveSurfaceToFile(filename.c_str(), D3DXIFF_BMP, pRTSurf, NULL, NULL);
			count++;
		}

        pRTSurf->Release();
    }

	D3DDevice->EndScene();
	D3DDevice->SetDepthStencilSurface( pOldDS );
	D3DDevice->SetRenderTarget(0, pOldRT);
}
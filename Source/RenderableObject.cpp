#include"RenderableObject.h"
#include"Scene.h"
//#include"Camera.h"
#include"DirectionalLight.h"
#include"PointLight.h"

void RenderableObject::Draw()
{
	mesh.SetStream();
	mesh.SetVertexDeclaration();

	switch(shader){
		case Diffuse:{
			diffuseShader.effect->SetTechnique("Diffuse");
			diffuseShader.effect->SetMatrix("matWVP", &(mesh.LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.ProjMatrix()));
			diffuseShader.effect->SetMatrix("matWorld", &(mesh.LocalToWorldMatrix()));
			diffuseShader.effect->SetMatrix("matUVTransform", &(material.UVTransformMatrix()));
			diffuseShader.effect->SetRawValue("directionalLights", directionalLightsData, 0, sizeof(directionalLightsData));
			diffuseShader.effect->SetRawValue("pointLights", pointLightsData, 0, sizeof(pointLightsData));
			diffuseShader.effect->SetTexture("colorTex", material.colorTex);
			diffuseShader.effect->SetRawValue("mtlDiffuse", &(material.diffuse), 0, sizeof(D3DXCOLOR));

			diffuseShader.effect->Begin(0, 0);
			diffuseShader.effect->BeginPass(0);

			mesh.Draw();

			diffuseShader.effect->EndPass();
			diffuseShader.effect->End();

			break;
		}

		case Specular:{
			specShader.effect->SetTechnique("Specular");
			specShader.effect->SetMatrix("matWVP", &(mesh.LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.ProjMatrix()));
			specShader.effect->SetMatrix("matWorld", &(mesh.LocalToWorldMatrix()));
			specShader.effect->SetMatrix("matUVTransform", &(material.UVTransformMatrix()));
			specShader.effect->SetRawValue("directionalLights", directionalLightsData, 0, sizeof(directionalLightsData));
			specShader.effect->SetRawValue("pointLights", pointLightsData, 0, sizeof(pointLightsData));
			specShader.effect->SetTexture("colorTex", material.colorTex);
			specShader.effect->SetRawValue("mtlDiffuse", &(material.diffuse), 0, sizeof(D3DXCOLOR));
			specShader.effect->SetRawValue("mtlSpec", &(material.shiness * material.spec), 0, sizeof(D3DXCOLOR));
			specShader.effect->SetFloat("gloss", material.gloss);
			specShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

			specShader.effect->Begin(0, 0);
			specShader.effect->BeginPass(0);

			mesh.Draw();

			specShader.effect->EndPass();
			specShader.effect->End();

			break;
		}

		case BumpSpecular:{
			bumpSpecShader.effect->SetTechnique("BumpSpecular");
			bumpSpecShader.effect->SetMatrix("matWVP", &(mesh.LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.ProjMatrix()));
			bumpSpecShader.effect->SetMatrix("matWorld", &(mesh.LocalToWorldMatrix()));
			bumpSpecShader.effect->SetMatrix("matUVTransform", &(material.UVTransformMatrix()));
			bumpSpecShader.effect->SetRawValue("directionalLights", directionalLightsData, 0, sizeof(directionalLightsData));
			bumpSpecShader.effect->SetRawValue("pointLights", pointLightsData, 0, sizeof(pointLightsData));
			bumpSpecShader.effect->SetTexture("colorTex", material.colorTex);
			bumpSpecShader.effect->SetTexture("normalTex", material.normalTex);
			bumpSpecShader.effect->SetRawValue("mtlDiffuse", &(material.diffuse), 0, sizeof(D3DXCOLOR));
			bumpSpecShader.effect->SetRawValue("mtlSpec", &(material.shiness * material.spec), 0, sizeof(D3DXCOLOR));
			bumpSpecShader.effect->SetFloat("gloss", material.gloss);
			bumpSpecShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

			bumpSpecShader.effect->Begin(0, 0);
			bumpSpecShader.effect->BeginPass(0);

			mesh.Draw();

			bumpSpecShader.effect->EndPass();
			bumpSpecShader.effect->End();

			break;
		}
						  		
		case BumpHSpecular:{
			bumpHSpecShader.effect->SetTechnique("BumpHSpecular");
			bumpHSpecShader.effect->SetMatrix("matWVP", &(mesh.LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.ProjMatrix()));
			bumpHSpecShader.effect->SetMatrix("matWorld", &(mesh.LocalToWorldMatrix()));
			bumpHSpecShader.effect->SetMatrix("matUVTransform", &(material.UVTransformMatrix()));
			bumpHSpecShader.effect->SetRawValue("directionalLights", directionalLightsData, 0, sizeof(directionalLightsData));
			bumpHSpecShader.effect->SetRawValue("pointLights", pointLightsData, 0, sizeof(pointLightsData));
			bumpHSpecShader.effect->SetTexture("colorTex", material.colorTex);
			bumpHSpecShader.effect->SetTexture("bumpTex", material.bumpTex);
			bumpHSpecShader.effect->SetRawValue("mtlDiffuse", &(material.diffuse), 0, sizeof(D3DXCOLOR));
			bumpHSpecShader.effect->SetRawValue("mtlSpec", &(material.shiness * material.spec), 0, sizeof(D3DXCOLOR));
			bumpHSpecShader.effect->SetFloat("gloss", material.gloss);
			bumpHSpecShader.effect->SetFloat("uvOffset", material.uvOffset);   
			bumpHSpecShader.effect->SetFloat("heightMapScale", material.heightMapScale); 
			bumpHSpecShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

			bumpHSpecShader.effect->Begin(0, 0);
			bumpHSpecShader.effect->BeginPass(0);

			mesh.Draw();

			bumpHSpecShader.effect->EndPass();
			bumpHSpecShader.effect->End();

			break;
		}
						  		
		case BumpPHSpecular:{
			bumpPHSpecShader.effect->SetTechnique("BumpPHSpecular");
			bumpPHSpecShader.effect->SetMatrix("matWVP", &(mesh.LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.ProjMatrix()));
			bumpPHSpecShader.effect->SetMatrix("matWorld", &(mesh.LocalToWorldMatrix()));
			bumpPHSpecShader.effect->SetMatrix("matUVTransform", &(material.UVTransformMatrix()));
			bumpPHSpecShader.effect->SetRawValue("directionalLights", directionalLightsData, 0, sizeof(directionalLightsData));
			bumpPHSpecShader.effect->SetRawValue("pointLights", pointLightsData, 0, sizeof(pointLightsData));
			bumpPHSpecShader.effect->SetTexture("colorTex", material.colorTex);
			bumpPHSpecShader.effect->SetTexture("bumpTex", material.bumpTex);
			bumpPHSpecShader.effect->SetRawValue("mtlDiffuse", &(material.diffuse), 0, sizeof(D3DXCOLOR));
			bumpPHSpecShader.effect->SetRawValue("mtlSpec", &(material.shiness * material.spec), 0, sizeof(D3DXCOLOR));
			bumpPHSpecShader.effect->SetFloat("gloss", material.gloss);
			bumpPHSpecShader.effect->SetFloat("uvOffset", material.uvOffset);   
			bumpPHSpecShader.effect->SetFloat("heightMapScale", material.heightMapScale);  
			bumpPHSpecShader.effect->SetFloat("parallaxHeightScale", material.parallaxHeightScale);
			bumpPHSpecShader.effect->SetFloat("parallaxHeightOffset", material.parallaxHeightOffset);    
			bumpPHSpecShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

			bumpPHSpecShader.effect->Begin(0, 0);
			bumpPHSpecShader.effect->BeginPass(0);

			mesh.Draw();

			bumpPHSpecShader.effect->EndPass();
			bumpPHSpecShader.effect->End();

			break;
		}
						  		
		case BumpPSpecular:{
			bumpPSpecShader.effect->SetTechnique("BumpPSpecular");
			bumpPSpecShader.effect->SetMatrix("matWVP", &(mesh.LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.ProjMatrix()));
			bumpPSpecShader.effect->SetMatrix("matWorld", &(mesh.LocalToWorldMatrix()));
			bumpPSpecShader.effect->SetMatrix("matUVTransform", &(material.UVTransformMatrix()));
			bumpPSpecShader.effect->SetRawValue("directionalLights", directionalLightsData, 0, sizeof(directionalLightsData));
			bumpPSpecShader.effect->SetRawValue("pointLights", pointLightsData, 0, sizeof(pointLightsData));
			bumpPSpecShader.effect->SetTexture("colorTex", material.colorTex);
			bumpPSpecShader.effect->SetTexture("heightTex", material.bumpTex);
			bumpPSpecShader.effect->SetTexture("normalTex", material.normalTex);
			bumpPSpecShader.effect->SetRawValue("mtlDiffuse", &(material.diffuse), 0, sizeof(D3DXCOLOR));
			bumpPSpecShader.effect->SetRawValue("mtlSpec", &(material.shiness * material.spec), 0, sizeof(D3DXCOLOR));
			bumpPSpecShader.effect->SetFloat("gloss", material.gloss);
			bumpPSpecShader.effect->SetFloat("parallaxHeightScale", material.parallaxHeightScale);
			bumpPSpecShader.effect->SetFloat("parallaxHeightOffset", material.parallaxHeightOffset);    
			bumpPSpecShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

			bumpPSpecShader.effect->Begin(0, 0);
			bumpPSpecShader.effect->BeginPass(0);

			mesh.Draw();

			bumpPSpecShader.effect->EndPass();
			bumpPSpecShader.effect->End();

			break;
		}		   
						  		
		case BumpPOMSpecular:{
			bumpPOMSpecShader.effect->SetTechnique("BumpPOMSpecular");
			bumpPOMSpecShader.effect->SetMatrix("matWVP", &(mesh.LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.ProjMatrix()));
			bumpPOMSpecShader.effect->SetMatrix("matWorld", &(mesh.LocalToWorldMatrix()));
			bumpPOMSpecShader.effect->SetMatrix("matUVTransform", &(material.UVTransformMatrix()));
			bumpPOMSpecShader.effect->SetRawValue("directionalLights", directionalLightsData, 0, sizeof(directionalLightsData));
			bumpPOMSpecShader.effect->SetRawValue("pointLights", pointLightsData, 0, sizeof(pointLightsData));
			bumpPOMSpecShader.effect->SetTexture("colorTex", material.colorTex);
			bumpPOMSpecShader.effect->SetTexture("heightTex", material.bumpTex);
			bumpPOMSpecShader.effect->SetTexture("normalTex", material.normalTex);
			bumpPOMSpecShader.effect->SetRawValue("mtlDiffuse", &(material.diffuse), 0, sizeof(D3DXCOLOR));
			bumpPOMSpecShader.effect->SetRawValue("mtlSpec", &(material.shiness * material.spec), 0, sizeof(D3DXCOLOR));
			bumpPOMSpecShader.effect->SetFloat("gloss", material.gloss);
			bumpPOMSpecShader.effect->SetFloat("parallaxHeightScale", material.parallaxHeightScale);
			bumpPOMSpecShader.effect->SetFloat("parallaxHeightOffset", material.parallaxHeightOffset);    
			bumpPOMSpecShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

			bumpPOMSpecShader.effect->Begin(0, 0);
			bumpPOMSpecShader.effect->BeginPass(0);

			mesh.Draw();

			bumpPOMSpecShader.effect->EndPass();
			bumpPOMSpecShader.effect->End();

			break;
		}
	}

	gizmo.rotation = mesh.rotation;
	if(showGizmo)
		gizmo.Draw(mesh.position);
	
}
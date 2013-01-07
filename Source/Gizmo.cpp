#include"Gizmo.h"
#include"Scene.h"

void Gizmo::Build()
{
	/*axisX.Build(Vector3::Zero, Vector3(1, 0, 0), D3DXCOLOR(1, 0, 0, 1), 4.0f);
	axisY.Build(Vector3::Zero, Vector3(0, 1, 0), D3DXCOLOR(0, 1, 0, 1), 4.0f);
	axisZ.Build(Vector3::Zero, Vector3(0, 0, 1), D3DXCOLOR(0, 0, 1, 1), 4.0f);*/

	axisX = Cylinder(0.02f, 0.02f, 1.0f, 12, 8, D3DXCOLOR(1, 0, 0, 1));
	axisX.CaculateNormals();
	axisX.Build(XYZ_N);
	axisX.rotation = Quaternion(0, 0, -PI/2);
	
	axisY = Cylinder(0.02f, 0.02f, 1.0f, 12, 8, D3DXCOLOR(0, 1, 0, 1));
	axisY.CaculateNormals();
	axisY.Build(XYZ_N);
	
	axisZ = Cylinder(0.02f, 0.02f, 1.0f, 12, 8, D3DXCOLOR(0, 0, 1, 1));
	axisZ.CaculateNormals();
	axisZ.Build(XYZ_N);
	axisZ.rotation = Quaternion(PI/2, 0, 0);

	coneX = Cylinder(0.0f, 0.08f, 0.2f, 12, 8, D3DXCOLOR(1, 0, 0, 1));
	coneX.CaculateNormals();
	coneX.Build(XYZ_N);
	coneX.position = Vector3(1, 0, 0);
	coneX.rotation = Quaternion(0, 0, -PI/2);
		
	coneY = Cylinder(0.0f, 0.08f, 0.2f, 12, 8, D3DXCOLOR(0, 1, 0, 1));
	coneY.CaculateNormals();
	coneY.Build(XYZ_N);
	coneY.position = Vector3(0, 1, 0);
		
	coneZ = Cylinder(0.0f, 0.08f, 0.2f, 12, 8, D3DXCOLOR(0, 0, 1, 0));
	coneZ.CaculateNormals();
	coneZ.Build(XYZ_N);
	coneZ.position = Vector3(0, 0, 1);
	coneZ.rotation = Quaternion(PI/2, 0, 0);
}

void Gizmo::CaculatePosition(Vector3 sourcePos, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXMATRIX orthoProjMatrix)
{
	D3DXVECTOR3 orthoPos, clipPos;
	D3DXMATRIX invTempMatrix;
	D3DXVec3TransformCoord(&clipPos, &(D3DXVECTOR3(sourcePos.x, sourcePos.y, sourcePos.z)), &(viewMatrix*projMatrix));
	D3DXMatrixInverse(&invTempMatrix, 0, &(viewMatrix*orthoProjMatrix));
	D3DXVec3TransformCoord(&orthoPos, &clipPos, &invTempMatrix);
	position = Vector3(orthoPos.x, orthoPos.y, orthoPos.z);
}

void Gizmo::Draw(Vector3 perspectivePos)
{
	CaculatePosition(perspectivePos, scene.mainCamera.ViewMatrix(), scene.mainCamera.ProjMatrix(), scene.mainCamera.OrthoProjMatrix());
	
	// TODO：存在一个问题，gizmo本身的z顺序。
	D3DDevice->SetRenderState(D3DRS_ZENABLE, false);

	// axis - line
	/*axisX.Draw(LocalToWorldMatrix() * scene.mainCamera.ViewMatrix() * scene.mainCamera.OrthoProjMatrix());
	axisZ.Draw(LocalToWorldMatrix() * scene.mainCamera.ViewMatrix() * scene.mainCamera.OrthoProjMatrix());
	axisY.Draw(LocalToWorldMatrix() * scene.mainCamera.ViewMatrix() * scene.mainCamera.OrthoProjMatrix());*/

	// axisX
	axisX.SetStream();
	axisX.SetVertexDeclaration();

	gizmoShader.effect->SetTechnique("Gizmo");
	gizmoShader.effect->SetMatrix("matWVP", &(axisX.LocalToWorldMatrix()*LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.OrthoProjMatrix()));
	gizmoShader.effect->SetMatrix("matWorld", &(axisX.LocalToWorldMatrix()*LocalToWorldMatrix()));
	gizmoShader.effect->SetRawValue("mainColor", &(axisX.color), 0, sizeof(D3DXCOLOR));
	gizmoShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

	gizmoShader.effect->Begin(0, 0);
	gizmoShader.effect->BeginPass(0);

	axisX.Draw();

	gizmoShader.effect->EndPass();
	gizmoShader.effect->End();
	
	// axisY
	axisY.SetStream();
	axisY.SetVertexDeclaration();

	gizmoShader.effect->SetTechnique("Gizmo");
	gizmoShader.effect->SetMatrix("matWVP", &(axisY.LocalToWorldMatrix()*LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.OrthoProjMatrix()));
	gizmoShader.effect->SetMatrix("matWorld", &(axisY.LocalToWorldMatrix()*LocalToWorldMatrix()));
	gizmoShader.effect->SetRawValue("mainColor", &(axisY.color), 0, sizeof(D3DXCOLOR));
	gizmoShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

	gizmoShader.effect->Begin(0, 0);
	gizmoShader.effect->BeginPass(0);

	axisY.Draw();

	gizmoShader.effect->EndPass();
	gizmoShader.effect->End();

	// axisZ
	axisZ.SetStream();
	axisZ.SetVertexDeclaration();

	gizmoShader.effect->SetTechnique("Gizmo");
	gizmoShader.effect->SetMatrix("matWVP", &(axisZ.LocalToWorldMatrix()*LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.OrthoProjMatrix()));
	gizmoShader.effect->SetMatrix("matWorld", &(axisZ.LocalToWorldMatrix()*LocalToWorldMatrix()));
	gizmoShader.effect->SetRawValue("mainColor", &(axisZ.color), 0, sizeof(D3DXCOLOR));
	gizmoShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

	gizmoShader.effect->Begin(0, 0);
	gizmoShader.effect->BeginPass(0);

	axisZ.Draw();

	gizmoShader.effect->EndPass();
	gizmoShader.effect->End();
		
	// coneX
	coneX.SetStream();
	coneX.SetVertexDeclaration();

	gizmoShader.effect->SetTechnique("Gizmo");
	gizmoShader.effect->SetMatrix("matWVP", &(coneX.LocalToWorldMatrix()*LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.OrthoProjMatrix()));
	gizmoShader.effect->SetMatrix("matWorld", &(coneX.LocalToWorldMatrix()*LocalToWorldMatrix()));
	gizmoShader.effect->SetRawValue("mainColor", &(coneX.color), 0, sizeof(D3DXCOLOR));
	gizmoShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

	gizmoShader.effect->Begin(0, 0);
	gizmoShader.effect->BeginPass(0);

	coneX.Draw();

	gizmoShader.effect->EndPass();
	gizmoShader.effect->End();
		
	// coneY
	coneY.SetStream();
	coneY.SetVertexDeclaration();

	gizmoShader.effect->SetTechnique("Gizmo");
	gizmoShader.effect->SetMatrix("matWVP", &(coneY.LocalToWorldMatrix()*LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.OrthoProjMatrix()));
	gizmoShader.effect->SetMatrix("matWorld", &(coneY.LocalToWorldMatrix()*LocalToWorldMatrix()));
	gizmoShader.effect->SetRawValue("mainColor", &(coneY.color), 0, sizeof(D3DXCOLOR));
	gizmoShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

	gizmoShader.effect->Begin(0, 0);
	gizmoShader.effect->BeginPass(0);

	coneY.Draw();

	gizmoShader.effect->EndPass();
	gizmoShader.effect->End();
		
	// coneZ
	coneZ.SetStream();
	coneZ.SetVertexDeclaration();

	gizmoShader.effect->SetTechnique("Gizmo");
	gizmoShader.effect->SetMatrix("matWVP", &(coneZ.LocalToWorldMatrix()*LocalToWorldMatrix()*scene.mainCamera.ViewMatrix()*scene.mainCamera.OrthoProjMatrix()));
	gizmoShader.effect->SetMatrix("matWorld", &(coneZ.LocalToWorldMatrix()*LocalToWorldMatrix()));
	gizmoShader.effect->SetRawValue("mainColor", &(coneZ.color), 0, sizeof(D3DXCOLOR));
	gizmoShader.effect->SetRawValue("eyePos", &(scene.mainCamera.position), 0, sizeof(Vector3));

	gizmoShader.effect->Begin(0, 0);
	gizmoShader.effect->BeginPass(0);

	coneZ.Draw();

	gizmoShader.effect->EndPass();
	gizmoShader.effect->End();

	D3DDevice->SetRenderState(D3DRS_ZENABLE, true);
}
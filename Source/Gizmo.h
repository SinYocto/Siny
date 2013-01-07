#ifndef GIZMO_H
#define GIZMO_H

#include"Math.h"
#include"D3DUtility.h"
#include"Utility.h"
#include"Primitive.h"
#include"Shader.h"


class Line : public Object
{
public:
	Line() { line = NULL; }
	void Build(Vector3 pos0, Vector3 pos1, D3DXCOLOR lineColor, float lineWidth = 1.0f)
	{  
		D3DXCreateLine(D3DDevice, &line);
		line->SetWidth(lineWidth);

		points[0] = D3DXVECTOR3(pos0.x, pos0.y, pos0.z);
		points[1] = D3DXVECTOR3(pos1.x, pos1.y, pos1.z);

		color = lineColor;
		width = lineWidth;
	}
	void Draw(D3DXMATRIX matrix)
	{
		D3DDevice->SetRenderState(D3DRS_ZENABLE, false);
		line->DrawTransform(points, 2, &matrix, color);
		D3DDevice->SetRenderState(D3DRS_ZENABLE, true);
	}
public:
	ID3DXLine *line;
	D3DXVECTOR3 points[2];
	D3DXCOLOR color;
	float width;
};

class Gizmo : public Object
{
public:
	Gizmo() {}
	void Build();

	void CaculatePosition(Vector3 sourcePos, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXMATRIX orthoProjMatrix);
	void Draw(Vector3 perspectivePos);
public:
	//Line axisX;
	//Line axisY;
	//Line axisZ;

	Cylinder axisX;
	Cylinder axisY;
	Cylinder axisZ;

	Cylinder coneX;
	Cylinder coneY;
	Cylinder coneZ;
};


#endif
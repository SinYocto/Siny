#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

#include"Mesh.h"
#include"Shader.h"
#include"Material.h"
#include"Gizmo.h"

enum DisplayMode { Textured, WireFrame };

class RenderableObject
{
public:
	RenderableObject() {}
	RenderableObject(Mesh _mesh, ShadingMethod _shader, Material _material)
	{ mesh = _mesh; shader = _shader; material = _material; showGizmo = false; gizmo.Build(); displayMode = Textured; }
	void Draw();
public:
	Mesh mesh;
	ShadingMethod shader;
	Material material; 

	bool showGizmo;
	Gizmo gizmo;

	DisplayMode displayMode;
};

#endif
#include"D3DUtility.h"
#include"Shader.h"
#include"Scene.h"
#include"DirectionalLight.h"
#include"PointLight.h"
#include"Input.h"
#include"IrradianceEM.h"

HWND HWnd = NULL;
IDirect3D9 *D3D = NULL;
IDirect3DDevice9 *D3DDevice = NULL;

IDirectInput8 *DI = NULL;					
IDirectInputDevice8 *DID_Keyboard = NULL;	
IDirectInputDevice8 *DID_Mouse = NULL;

Scene scene;

Shader diffuseShader("./Shaders/Diffuse.fx");
Shader specShader("./Shaders/Specular.fx");
Shader bumpSpecShader("./Shaders/BumpSpecular.fx");
Shader bumpHSpecShader("./Shaders/BumpHSpecular.fx");
Shader bumpPHSpecShader("./Shaders/BumpPHSpecular.fx");
Shader bumpPSpecShader("./Shaders/BumpPSpecular.fx");
Shader bumpPOMSpecShader("./Shaders/BumpPOMSpecular.fx");
Shader gizmoShader("./Shaders/Gizmo.fx");
Shader cubeEMShader("./Shaders/CubeEM.fx");
Shader cubeRefractEMShader("./Shaders/CubeRefractEM.fx");
Shader irradianceEMShader("./Shaders/IrradianceEM.fx");
Shader cubeMappingShader("./Shaders/CubeMapping.fx");

Shader terrainShader("./Shaders/Terrain.fx");


DirectionalLightData directionalLightsData[MAX_DIRECTIONAL_LIGHTS];
PointLightData pointLightsData[MAX_POINT_LIGHTS];


IrradianceEM irrEM;
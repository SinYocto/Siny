#include"D3DUtility.h"
#include"Shader.h"
#include"Scene.h"
#include"DirectionalLight.h"
#include"PointLight.h"
#include"Input.h"
#include"IrradianceEM.h"
#include"GUI.h"

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

Shader colorPickerSLShader("./Shaders/ColorPickerSL.fx");
Shader colorPickerHueShader("./Shaders/ColorPickerHue.fx");


DirectionalLightData directionalLightsData[MAX_DIRECTIONAL_LIGHTS];
PointLightData pointLightsData[MAX_POINT_LIGHTS];

LabelStyle defaultLabelStyle("ºÚÌå", 8, 17, 100, WHITE, DT_CENTER | DT_VCENTER);
ButtonStyle defaultButtonStyle(&defaultLabelStyle, "./Textures/GUI/buttonNormal.tga", "./Textures/GUI/buttonHover.tga", "./Textures/GUI/buttonActive.tga");
SliderStyle defaultSliderStyle("./Textures/GUI/sliderBar.tga", "./Textures/GUI/sliderThumbNormal.tga", "./Textures/GUI/sliderThumbHover.tga", "./Textures/GUI/sliderThumbActive.tga", 0.5f);
ToggleStyle defaultToggleStyle("./Textures/GUI/toggleOffNormal.tga", "./Textures/GUI/toggleOffHover.tga", "./Textures/GUI/toggleOnNormal.tga", "./Textures/GUI/toggleOnHover.tga");
ListBoxStyle defaultListBoxStyle(&defaultLabelStyle, "./Textures/GUI/listBoxButtonNormal.tga", "./Textures/GUI/listBoxButtonHover.tga", "./Textures/GUI/listBoxItemNormal.tga", "./Textures/GUI/listBoxItemHover.tga");
GUI GUISystem;


IrradianceEM irrEM;
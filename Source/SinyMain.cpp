
//-------------
// 包含文件
//-------------

#include<fstream>

#include"D3DUtility.h"
#include"Utility.h"

#include"Time.h"
#include"Object.h"
#include"Camera.h"
#include"Mesh.h"
#include"Primitive.h"
#include"Material.h"
#include"Shader.h"
#include"DirectionalLight.h"
#include"PointLight.h"
#include"RenderableObject.h"
#include"Scene.h"
#include"Input.h"
#include"Gizmo.h"
#include"IrradianceEM.h"
#include"SkyBox.h"
#include"Terrain.h"
#include"QuadTree.h"
#include"GUI.h"


//--------------
// 全局变量
//--------------

// Lights
DirectionalLight dirLight0;
DirectionalLight dirLight1;

PointLight pointLight0;
PointLight pointLight1;

// Terrain
Terrain terrain;

// Renderable Object
RenderableObject cubie;
RenderableObject sphereRO;
RenderableObject teapotRO;
RenderableObject irrTeapotRO;

RenderableObject terrainRO;

LabelStyle* leftAlignStyle;

// Material
Material mtlBrickP;     // use parallax mapping
Material mtlBrickPOM;   // use parallax occlusion mapping
Material mtlBrickPH;    // use parallax mapping(normal from heightMap)
Material mtlBrickB;     // use bump mapping
Material mtlBrickN;     // use normal mapping
Material mtlBrickS;     // use smiple Specular
Material mtlBrickD;     // diffuse
Material mtlCubeMap;	

Material mtlTerrain;

//IrradianceEM irrEM;


bool windowed = true;
int screenWidth = 1280;
int screenHeight = 720;
				
_D3DMULTISAMPLE_TYPE	multisampleType = D3DMULTISAMPLE_4_SAMPLES;			
DWORD					multisampleQuality = 0;


//------------
// 函数声明
//------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd);
int AppInit(HINSTANCE hInst);
int AppSetup();
int AppLoop();
int AppDestory();

void FPCameraControl(float deltaTime);
void HoverCameraControl(Vector3 targetPos, float deltaTime);

void GUIUpdate();

void SetCamera();
void SetShaders();
void SetMaterials();
void SetTerrain();
void SetMeshes();
void SetLights();
void SetSky();
void SetGUIStyle();

//------------
// 函数定义
//------------

/**
 * AppInit()
 */
int AppInit(HINSTANCE hInst)
{
	CreateConsole();

	if(!InitD3D(hInst, windowed, screenWidth, screenHeight, multisampleType, multisampleQuality, HWnd, &D3D, &D3DDevice))
		return 0;

	if(!InitInput(hInst, HWnd, &DI, &DID_Keyboard, &DID_Mouse))
		return 0;

	return 1;
}

/**
 * AppSetup()
 */
int AppSetup()
{
	SetGUIStyle();
	SetCamera();
	SetShaders();
	SetMaterials();
	SetTerrain();
	SetMeshes();
	SetLights();
	SetSky();
	scene.backgroundColor = 0xff1e90ff;

	return 1;
}

/**
 * AppLoop()
 */
int AppLoop()
{	
	// monitor input
	Input::GetDIDState();

	// CameraController
	FPCameraControl(Time::deltaTime);
	//HoverCameraControl(cubie.mesh.position, Time::deltaTime);


	if(Input::GetKey(DIK_UP))
		cubie.material.uvOffset += 0.0001f;
	if(Input::GetKey(DIK_DOWN))
		cubie.material.uvOffset -= 0.0001f;

	if(cubie.material.uvOffset < 0)
		cubie.material.uvOffset = 0;

	if(Input::GetKeyDown(DIK_NUMPAD0)){
		cubie.shader = Specular;
		cubie.material = mtlBrickS;
	}
	if(Input::GetKeyDown(DIK_NUMPAD1)){
		cubie.shader = BumpSpecular;
		cubie.material = mtlBrickN;
	}
	if(Input::GetKeyDown(DIK_NUMPAD2)){
		cubie.shader = BumpHSpecular;
		cubie.material = mtlBrickB;
	}
	if(Input::GetKeyDown(DIK_NUMPAD3)){
		cubie.shader = BumpPHSpecular;
		cubie.material = mtlBrickPH;
	}
	if(Input::GetKeyDown(DIK_NUMPAD4)){
		cubie.shader = BumpPSpecular;
		cubie.material = mtlBrickP;
	}
	if(Input::GetKeyDown(DIK_NUMPAD5)){
		cubie.shader = BumpPOMSpecular;
		cubie.material = mtlBrickPOM;
	}

	GUIUpdate();
	scene.Update();
	scene.Render();

	return true;
}

/**
 * AppDestory()
 */
int AppDestory()
{
	FreeConsole();  
	return true;
}

/**
 * 入口函数
 */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	if(!AppInit(hInst)){
		MessageBox(0, "AppInit() - FAILED", 0, 0);
		return 0;
	}

	if(!AppSetup()){
		MessageBox(0, "AppSetup() - FAILED", 0, 0);
		return 0;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	LARGE_INTEGER lpf;
	QueryPerformanceFrequency(&lpf);
	LONGLONG nFreq = lpf.QuadPart;
	QueryPerformanceCounter(&lpf);
	static LONGLONG lastFreq = lpf.QuadPart;

	while(msg.message != WM_QUIT){
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		QueryPerformanceCounter(&lpf);
		LONGLONG curFreq = lpf.QuadPart;
		float deltaTime = (curFreq - lastFreq) / (float)nFreq;
		Time::deltaTime = deltaTime;
		lastFreq = curFreq;

		if(!AppLoop()){
			MessageBox(0, "AppLoop() - FAILED", 0, 0);
		}
	}

	AppDestory();

	return msg.wParam;
}

void FPCameraControl(float deltaTime)
{
	Vector3 moveVector;
	float moveSpeed = 5.0f;
	float rotateSpeed = 2.0f;

	if(Input::GetKey(DIK_LSHIFT))
		moveSpeed *= 4;

	if(Input::GetKey(DIK_W))
		moveVector = moveVector + moveSpeed * deltaTime * Vector3(0, 0, 1);
	if(Input::GetKey(DIK_S))
		moveVector = moveVector + moveSpeed * deltaTime * Vector3(0, 0, -1);

	if(Input::GetKey(DIK_A))
		moveVector = moveVector + moveSpeed * deltaTime * Vector3(-1, 0, 0);
	if(Input::GetKey(DIK_D))
		moveVector = moveVector + moveSpeed * deltaTime * Vector3(1, 0, 0);

	moveVector = scene.mainCamera.LocalVectorToWorld(moveVector);

	if(Input::GetKey(DIK_Q))
		moveVector = moveVector + moveSpeed * deltaTime * Vector3(0, -1, 0);
	if(Input::GetKey(DIK_E))
		moveVector = moveVector + moveSpeed * deltaTime * Vector3(0, 1, 0);

	
	if(moveVector != Vector3::Zero){
		scene.mainCamera.isTranformDirty = true;
		scene.mainCamera.Translate(moveVector);
	}
	
	if(Input::MouseState.rgbButtons[1] & 0x80){
		float rotationY = rotateSpeed * Input::MouseState.lX / 1000.0f;
		float rotationX = rotateSpeed * Input::MouseState.lY / 1000.0f;

		if(rotationY != 0 || rotationX != 0){
			scene.mainCamera.isTranformDirty = true;
			scene.mainCamera.Rotate(0, rotationY, 0);
			scene.mainCamera.RotateLocal(rotationX, 0, 0);
		}
	}

}

void HoverCameraControl(Vector3 targetPos, float deltaTime)
{
	float rotateSpeed = 5.0f;
	float zoomSpeed = 20.0f;
	float minTiltAngle = -4*PI/9;
	float maxTiltAngle = 4*PI/9;
	float minDistance = 2.0f;
	float maxDistance = 100.0f;

	static float panAngle = 0;
	static float tiltAngle = PI/4;
	static float distance = 20.0f;

	if(Input::MouseState.rgbButtons[1] & 0x80){
		float deltaPanAngle = - rotateSpeed * Input::MouseState.lX / 1000.0f;
		float deltaTiltAngle = rotateSpeed * Input::MouseState.lY / 1000.0f;

		if(deltaPanAngle != 0 || deltaTiltAngle != 0)
			scene.mainCamera.isTranformDirty = true;

		panAngle += deltaPanAngle;
		tiltAngle += deltaTiltAngle;

		if(tiltAngle > maxTiltAngle)
			tiltAngle = maxTiltAngle;
		if(tiltAngle < minTiltAngle)
			tiltAngle = minTiltAngle;
	}

	if(Input::MouseState.rgbButtons[0] & 0x80){
		float deltaDistance = zoomSpeed * ( -Input::MouseState.lX + Input::MouseState.lY) / 1000.0f;
		
		if(deltaDistance != 0)
			scene.mainCamera.isTranformDirty = true;

		distance += deltaDistance;
		if(distance > maxDistance)
			distance = maxDistance;
		if(distance < minDistance)
			distance = minDistance;
	}

	float y = distance * sin(tiltAngle);
	float x = distance * cos(tiltAngle) * cos(panAngle);
	float z = distance * cos(tiltAngle) * sin(panAngle);

	scene.mainCamera.position = targetPos + Vector3(x, y, z);
	scene.mainCamera.LookAt(targetPos);

}

void SetGUIStyle()
{
	defaultLabelStyle.CreateFont();
	leftAlignStyle = new LabelStyle("黑体", 8, 17, 0, WHITE, DT_LEFT | DT_VCENTER);
	leftAlignStyle->CreateFont();

	defaultButtonStyle.CreateTextures();
	defaultSliderStyle.CreateTextures();
	defaultToggleStyle.CreateTextures();
	defaultListBoxStyle.CreateTextures();
}

void GUIUpdate()
{
	GUISystem.clear();
	
	char text[50];
	sprintf(text, "fps:%d", scene.fps);
	GUISystem.GUILabel(text, Rect(10, 10, 100, 25), leftAlignStyle);
	
	sprintf(text, "culledChunks:%d", (*(scene.terrains.begin()))->culledChunks);
	GUISystem.GUILabel(text, Rect(110, 10, 200, 25), leftAlignStyle);


	static bool showSettings = false;
	if(GUISystem.GUIButton("Settings", Rect(10, 40, 100, 25)))
		showSettings = !showSettings;
	
	GUISystem.GUILabel("操作：按住鼠标右键旋转摄像机，WASDQE移动", Rect(10, 70, 400, 25), leftAlignStyle);
	if(showSettings){
		GUISystem.GUILabel("Time:", Rect(10, 100, 100, 25), leftAlignStyle); 

		static int cubieMaterialIx = 0;
		vector<LPWSTR> listItems;
		listItems.push_back(str2wstr("BasicSpecular"));
		listItems.push_back(str2wstr("BumpMapping"));
		listItems.push_back(str2wstr("NormalMapping"));
		listItems.push_back(str2wstr("POM"));
		cubieMaterialIx = GUISystem.GUIListBox(listItems, Rect(400, 70, 200, 25), cubieMaterialIx, "cubieMaterialIx");

		switch(cubieMaterialIx){
			case 0:
				cubie.shader = Specular;
				cubie.material = mtlBrickS;
				break;
			case 1:
				cubie.shader = BumpHSpecular;
				cubie.material = mtlBrickB;
				break;
			case 2:
				cubie.shader = BumpSpecular;
				cubie.material = mtlBrickN;
				break;
			case 3:
				cubie.shader = BumpPOMSpecular;
				cubie.material = mtlBrickPOM;
				break;
		}

		static int timeCount = 0;
		timeCount = GUISystem.GUISlider(Rect(110, 100, 200, 25), 0, 3600*24 - 1, timeCount, "sliderHour");
		int h = timeCount/3600;
		int m = (timeCount - 3600*h)/60;
		int s = timeCount%60;
		sprintf(text, "%d:%d:%d", h, m, s);
		GUISystem.GUILabel(text, Rect(310, 100, 100, 25));
		DayTime::SetDayTime(h, m, s);

		/*static bool flag = false;
		flag = GUISystem.GUIToggle(Rect(10, 100, 25, 25), flag);*/

		static D3DXCOLOR color0h_z = scene.skyColor[0].zenithColor;
		static D3DXCOLOR color0h_w = scene.skyColor[0].westColor;
		static D3DXCOLOR color0h_e = scene.skyColor[0].eastColor;
		
		static D3DXCOLOR color6h_z = scene.skyColor[1].zenithColor;
		static D3DXCOLOR color6h_w = scene.skyColor[1].westColor;
		static D3DXCOLOR color6h_e = scene.skyColor[1].eastColor;
		
		static D3DXCOLOR color12h_z = scene.skyColor[2].zenithColor;
		static D3DXCOLOR color12h_w = scene.skyColor[2].westColor;
		static D3DXCOLOR color12h_e = scene.skyColor[2].eastColor;
		
		static D3DXCOLOR color18h_z = scene.skyColor[3].zenithColor;
		static D3DXCOLOR color18h_w = scene.skyColor[3].westColor;
		static D3DXCOLOR color18h_e = scene.skyColor[3].eastColor;

		color0h_z = GUISystem.GUIColor(Rect(10, 130, 25, 25), color0h_z, "color0h_z");
		color0h_w = GUISystem.GUIColor(Rect(170, 130, 25, 25), color0h_w, "color0h_w");
		color0h_e = GUISystem.GUIColor(Rect(330, 130, 25, 25), color0h_e, "color0h_e");
		
		color6h_z = GUISystem.GUIColor(Rect(10, 240, 25, 25), color6h_z, "color6h_z");
		color6h_w = GUISystem.GUIColor(Rect(170, 240, 25, 25), color6h_w, "color6h_w");
		color6h_e = GUISystem.GUIColor(Rect(330, 240, 25, 25), color6h_e, "color6h_e");
		
		color12h_z = GUISystem.GUIColor(Rect(10, 350, 25, 25), color12h_z, "color12h_z");
		color12h_w = GUISystem.GUIColor(Rect(170, 350, 25, 25), color12h_w, "color12h_w");
		color12h_e = GUISystem.GUIColor(Rect(330, 350, 25, 25), color12h_e, "color12h_e");
		
		color18h_z = GUISystem.GUIColor(Rect(10, 460, 25, 25), color18h_z, "color18h_z");
		color18h_w = GUISystem.GUIColor(Rect(170, 460, 25, 25), color18h_w, "color18h_w");
		color18h_e = GUISystem.GUIColor(Rect(330, 460, 25, 25), color18h_e, "color18h_e");

		scene.skyColor[0].zenithColor = D3DCOLOR_XRGB((int)(color0h_z.r*255), (int)(color0h_z.g*255), (int)(color0h_z.b*255));
		scene.skyColor[0].westColor = D3DCOLOR_XRGB((int)(color0h_w.r*255), (int)(color0h_w.g*255), (int)(color0h_w.b*255));
		scene.skyColor[0].eastColor = D3DCOLOR_XRGB((int)(color0h_e.r*255), (int)(color0h_e.g*255), (int)(color0h_e.b*255));
		
		scene.skyColor[1].zenithColor = D3DCOLOR_XRGB((int)(color6h_z.r*255), (int)(color6h_z.g*255), (int)(color6h_z.b*255));
		scene.skyColor[1].westColor = D3DCOLOR_XRGB((int)(color6h_w.r*255), (int)(color6h_w.g*255), (int)(color6h_w.b*255));
		scene.skyColor[1].eastColor = D3DCOLOR_XRGB((int)(color6h_e.r*255), (int)(color6h_e.g*255), (int)(color6h_e.b*255));
		
		scene.skyColor[2].zenithColor = D3DCOLOR_XRGB((int)(color12h_z.r*255), (int)(color12h_z.g*255), (int)(color12h_z.b*255));
		scene.skyColor[2].westColor = D3DCOLOR_XRGB((int)(color12h_w.r*255), (int)(color12h_w.g*255), (int)(color12h_w.b*255));
		scene.skyColor[2].eastColor = D3DCOLOR_XRGB((int)(color12h_e.r*255), (int)(color12h_e.g*255), (int)(color12h_e.b*255));
		
		scene.skyColor[3].zenithColor = D3DCOLOR_XRGB((int)(color18h_z.r*255), (int)(color18h_z.g*255), (int)(color18h_z.b*255));
		scene.skyColor[3].westColor = D3DCOLOR_XRGB((int)(color18h_w.r*255), (int)(color18h_w.g*255), (int)(color18h_w.b*255));
		scene.skyColor[3].eastColor = D3DCOLOR_XRGB((int)(color18h_e.r*255), (int)(color18h_e.g*255), (int)(color18h_e.b*255));
	}
} 

void SetCamera()
{
	scene.mainCamera = Camera(PI/3, (float)screenWidth / screenHeight, 0.1f, 1000.0f);
	scene.mainCamera.position = Vector3(-10.0f, 30.0f, -30.0f);
	scene.mainCamera.LookAt(Vector3(-10.0f, 30.0f, 0.0f));
	scene.mainCamera.ExtractFrustumPlanes();
}

void SetShaders()
{
	diffuseShader.CreateEffect();
	specShader.CreateEffect();
	bumpSpecShader.CreateEffect();
	bumpHSpecShader.CreateEffect();
	bumpPSpecShader.CreateEffect();
	bumpPHSpecShader.CreateEffect();
	bumpPOMSpecShader.CreateEffect();
	gizmoShader.CreateEffect();
	cubeEMShader.CreateEffect();
	cubeRefractEMShader.CreateEffect();
	irradianceEMShader.CreateEffect();
	cubeMappingShader.CreateEffect();

	terrainShader.CreateEffect();

	colorPickerSLShader.CreateEffect();
	colorPickerHueShader.CreateEffect();
}

void SetMaterials()
{	
	mtlBrickPH.SetColorTex("./Textures/2711.jpg");
	mtlBrickPH.SetBumpTex("./Textures/2711Bump.jpg");
	mtlBrickPH.shiness = 0.3f;
	mtlBrickPH.gloss = 50.0f;
	mtlBrickPH.parallaxHeightScale = 0.02f;
	mtlBrickPH.parallaxHeightOffset = 0;
	mtlBrickPH.heightMapScale = 4.0f;
	mtlBrickPH.uvOffset = 1.0f/128.0f;
	mtlBrickPH.tilesU = 1.0f;
	mtlBrickPH.tilesV = 1.0f;
	
	mtlBrickP.SetColorTex("./Textures/2711.jpg");
	mtlBrickP.SetNormalTex("./Textures/2711Normal.jpg");
	mtlBrickP.SetBumpTex("./Textures/2711Bump.jpg");
	mtlBrickP.shiness = 0.3f;
	mtlBrickP.gloss = 50.0f;
	mtlBrickP.parallaxHeightScale = 0.02f;
	mtlBrickP.parallaxHeightOffset = 0;
	mtlBrickP.heightMapScale = 4.0f;
	mtlBrickP.uvOffset = 1.0f/128.0f;
	mtlBrickP.tilesU = 1.0f;
	mtlBrickP.tilesV = 1.0f;
	
	mtlBrickPOM.SetColorTex("./Textures/2711.jpg");
	mtlBrickPOM.SetNormalTex("./Textures/2711Normal.jpg");
	mtlBrickPOM.SetBumpTex("./Textures/2711Bump.jpg");
	mtlBrickPOM.shiness = 0.3f;
	mtlBrickPOM.gloss = 50.0f;
	mtlBrickPOM.parallaxHeightScale = 0.02f;
	mtlBrickPOM.parallaxHeightOffset = 0;
	mtlBrickPOM.heightMapScale = 4.0f;
	mtlBrickPOM.uvOffset = 1.0f/128.0f;
	mtlBrickPOM.tilesU = 1.0f;
	mtlBrickPOM.tilesV = 1.0f;

	mtlBrickB.SetColorTex("./Textures/2711.jpg");
	mtlBrickB.SetNormalTex("./Textures/2711Normal.jpg");
	mtlBrickB.SetBumpTex("./Textures/2711Bump.jpg");
	mtlBrickB.shiness = 0.3f;
	mtlBrickB.gloss = 50.0f;
	mtlBrickB.heightMapScale = 4.0f;
	mtlBrickB.uvOffset = 1.0f/128.0f;
	mtlBrickB.tilesU = 1.0f;
	mtlBrickB.tilesV = 1.0f;

	mtlBrickN.SetColorTex("./Textures/2711.jpg");
	mtlBrickN.SetNormalTex("./Textures/2711Normal.jpg");
	mtlBrickN.shiness = 0.3f;
	mtlBrickN.gloss = 50.0f;
	mtlBrickN.tilesU = 1.0f;
	mtlBrickN.tilesV = 1.0f;
	
	mtlBrickS.SetColorTex("./Textures/2711.jpg");
	mtlBrickS.shiness = 0.3f;
	mtlBrickS.gloss = 50.0f;
	mtlBrickS.tilesU = 1.0f;
	mtlBrickS.tilesV = 1.0f;

	mtlBrickD.SetColorTex("./Textures/2711.jpg");
	mtlBrickD.tilesU = 1.0f;
	mtlBrickD.tilesV = 1.0f;
	
	mtlCubeMap.SetCubeTex("./Textures/indoor_right.jpg", "./Textures/indoor_left.jpg", "./Textures/indoor_up.jpg", "./Textures/indoor_down.jpg", "./Textures/indoor_front.jpg", "./Textures/indoor_back.jpg");
	//mtlCubeMap.SetCubeTex("./Textures/Sunny1_left.jpg", "./Textures/Sunny1_right.jpg", "./Textures/Sunny1_up.jpg", "./Textures/Sunny1_down.jpg", "./Textures/Sunny1_front.jpg", "./Textures/Sunny1_back.jpg");

	mtlTerrain.SetColorTex("./Textures/Grass_Hill.jpg");
	mtlTerrain.tilesU = 30.0f;
	mtlTerrain.tilesV = 30.0f;
}

void SetTerrain()
{	
	terrain.LoadFromHeightMap("./Textures/heightMap257_bit16.raw", 257);
	terrain.BuildTerrain(200, 20, 4);
	terrain.CalculateNormals();
	//terrain.CalculateTangents();
	//terrain.CalculateBitangents();
	terrain.BuildChunkMesh(XYZ_UV_N);
	terrain.SetColorTexes("./Textures/Cliff.jpg", "./Textures/Grass_Hill.jpg", "./Textures/DirtGrass.jpg", "./Textures/Pebbles.jpg");
	terrain.SetSplatMapTex("./Textures/splat.tga");
	terrain.SetMtlParameters(30.0f, 30.0f);

	scene.AddTerrain(&terrain);
}

void SetMeshes()
{
	Cube cube;
	cube.CalculateNormals();
	cube.CalculateTangents();
	cube.CalculateBitangents();
	cube.Build(XYZ_UV_TBN);
	cube.position = Vector3(-10, 20, 0);

	cubie = RenderableObject(cube, Diffuse, mtlBrickD);
	scene.AddObject(&cubie);

	Sphere sphere(1, 24, 16);
	sphere.CalculateNormals();
	sphere.CalculateTangents();
	sphere.CalculateBitangents();
	sphere.Build(XYZ_UV_TBN);
	sphere.position = Vector3(-14, 20, 0);

	sphereRO = RenderableObject(sphere, BumpPOMSpecular, mtlBrickPOM);
	scene.AddObject(&sphereRO);

	/*Sphere sphere(1, 24, 16);
	sphere.CalculateNormals();
	sphere.Build(XYZ_N);
	sphere.position = Vector3(4, 0, 0);

	sphereRO = RenderableObject(sphere, CubeEM, mtlCubeMap);
	scene.AddObject(&sphereRO);*/
	
	Mesh teapot;
	teapot.LoadDataFromFile("./Models/teapot.obj", OBJ);
	teapot.CalculateUVs(SphereUV);
	teapot.CalculateNormals();
	teapot.Build(XYZ_N);
	teapot.scale = Vector3(0.01f, 0.01f, 0.01f);
	teapot.position = Vector3(-18, 20, 0);

	teapotRO = RenderableObject(teapot, CubeEM, mtlCubeMap);
	scene.AddObject(&teapotRO);
}

void SetLights()
{
	dirLight0.color = Vector3(1.0f, 1.0f, 1.0f);
	dirLight0.direction = Vector3(0, -0.4f, 1.0f);
	dirLight1.color = Vector3(0, 0, 1.0f);
	dirLight1.direction = Vector3(0, -0.5f, 1.0f);

	pointLight0.color = Vector3(1.0f, 0, 0);
	pointLight0.position = Vector3(4.0f, 0, 0);
	pointLight1.color = Vector3(0, 1.0f, 0);
	pointLight1.position = Vector3(0, 0, 4.0f);
	

	scene.AddDirectionalLight(&dirLight0);
	scene.AddDirectionalLight(&dirLight1);
	scene.AddPointLight(&pointLight0);
	scene.AddPointLight(&pointLight1);
	
	//dirLight0.Disable();
	dirLight1.Disable();
	pointLight0.Disable();
	pointLight1.Disable();
}

void SetSky()
{
	scene.SetAmbientLight(Vector3(1.0f, 1.0f, 1.0f), 0.2f);

	
	//scene.SetSkyDome("./Textures/Skydome4.png");
	scene.SetSkyDome();
	scene.SetSkyColor(SkyDomeColor(0xff292A46, 0xff111217, 0xff111217), 
		SkyDomeColor(0xff908E7E, 0xffF47413, 0xffE88877), 
		SkyDomeColor(0xff6088BC, 0xffb4cdcd, 0xff8AB0D7),
		SkyDomeColor(0xff9F9D8D, 0xffAE677B, 0xffF7711A));
	scene.SetCloud();
	//scene.SetCloudDome("./Textures/cloud.png", 200.0f);

	DayTime::SetTimeScale(1);
	//DayTime::SetDayTime(0, 0, 0);
	
	
	//scene.SetSkyBox("./Textures/Sunny1_right.jpg", "./Textures/Sunny1_left.jpg", "./Textures/Sunny1_up.jpg", "./Textures/Sunny1_down.jpg", "./Textures/Sunny1_front.jpg", "./Textures/Sunny1_back.jpg", 500.0f);
}









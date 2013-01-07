
//-------------
// 包含文件
//-------------

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


//--------------
// 全局变量
//--------------

// Lights
DirectionalLight dirLight0;
DirectionalLight dirLight1;

PointLight pointLight0;
PointLight pointLight1;

// Renderable Object
RenderableObject cubie;

// Material
Material mtlBrickP;     // use parallax mapping
Material mtlBrickPOM;   // use parallax occlusion mapping
Material mtlBrickPH;    // use parallax mapping(normal from heightMap)
Material mtlBrickB;     // use bump mapping
Material mtlBrickN;     // use normal mapping
Material mtlBrickS;     // use smiple Specular


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
	// camera
	scene.mainCamera = Camera(PI/3, (float)screenWidth / screenHeight, 0.1f, 1000.0f);
	scene.mainCamera.position = Vector3(0.0f, 2.0f, -10.0f);
	scene.mainCamera.LookAt(Vector3::Zero);

	// shader & material
	diffuseShader.CreateEffect();
	specShader.CreateEffect();
	bumpSpecShader.CreateEffect();
	bumpHSpecShader.CreateEffect();
	bumpPSpecShader.CreateEffect();
	bumpPHSpecShader.CreateEffect();
	bumpPOMSpecShader.CreateEffect();
	gizmoShader.CreateEffect();

	mtlBrickPH.SetColorTex("./Textures/6133.jpg");
	mtlBrickPH.SetBumpTex("./Textures/6133Bump.jpg");
	mtlBrickPH.shiness = 0.3f;
	mtlBrickPH.gloss = 50.0f;
	mtlBrickPH.parallaxHeightScale = 0.02f;
	mtlBrickPH.parallaxHeightOffset = 0;
	mtlBrickPH.heightMapScale = 4.0f;
	mtlBrickPH.uvOffset = 1.0f/128.0f;
	mtlBrickPH.tilesU = 1.0f;
	mtlBrickPH.tilesV = 1.0f;
	
	mtlBrickP.SetColorTex("./Textures/6133.jpg");
	mtlBrickP.SetNormalTex("./Textures/6133Normal.jpg");
	mtlBrickP.SetBumpTex("./Textures/6133Bump.jpg");
	mtlBrickP.shiness = 0.3f;
	mtlBrickP.gloss = 50.0f;
	mtlBrickP.parallaxHeightScale = 0.02f;
	mtlBrickP.parallaxHeightOffset = 0;
	mtlBrickP.heightMapScale = 4.0f;
	mtlBrickP.uvOffset = 1.0f/128.0f;
	mtlBrickP.tilesU = 1.0f;
	mtlBrickP.tilesV = 1.0f;
	
	mtlBrickPOM.SetColorTex("./Textures/6133.jpg");
	mtlBrickPOM.SetNormalTex("./Textures/6133Normal.jpg");
	mtlBrickPOM.SetBumpTex("./Textures/6133Bump.jpg");
	mtlBrickPOM.shiness = 0.3f;
	mtlBrickPOM.gloss = 50.0f;
	mtlBrickPOM.parallaxHeightScale = 0.02f;
	mtlBrickPOM.parallaxHeightOffset = 0;
	mtlBrickPOM.heightMapScale = 4.0f;
	mtlBrickPOM.uvOffset = 1.0f/128.0f;
	mtlBrickPOM.tilesU = 1.0f;
	mtlBrickPOM.tilesV = 1.0f;

	mtlBrickB.SetColorTex("./Textures/6133.jpg");
	mtlBrickB.SetNormalTex("./Textures/6133Normal.jpg");
	mtlBrickB.SetBumpTex("./Textures/6133Bump.jpg");
	mtlBrickB.shiness = 0.3f;
	mtlBrickB.gloss = 50.0f;
	mtlBrickB.heightMapScale = 4.0f;
	mtlBrickB.uvOffset = 1.0f/128.0f;
	mtlBrickB.tilesU = 1.0f;
	mtlBrickB.tilesV = 1.0f;

	mtlBrickN.SetColorTex("./Textures/6133.jpg");
	mtlBrickN.SetNormalTex("./Textures/6133Normal.jpg");
	mtlBrickN.shiness = 0.3f;
	mtlBrickN.gloss = 50.0f;
	mtlBrickN.tilesU = 1.0f;
	mtlBrickN.tilesV = 1.0f;
	
	mtlBrickS.SetColorTex("./Textures/6133.jpg");
	mtlBrickS.shiness = 0.3f;
	mtlBrickS.gloss = 50.0f;
	mtlBrickS.tilesU = 1.0f;
	mtlBrickS.tilesV = 1.0f;

	// mesh
	Cube cube;
	cube.CaculateNormals();
	cube.CaculateTangents();
	cube.CaculateBitangents();
	cube.Build(XYZ_UV_TBN);
	cube.position = Vector3(0, 0, 0);

	// Renderable
	cubie = RenderableObject(cube, BumpPHSpecular, mtlBrickP);


	// lights
	dirLight0.color = Vector3(1.0f, 1.0f, 1.0f);
	dirLight0.direction = Vector3(0, -0.5f, 1.0f);
	dirLight1.color = Vector3(0, 0, 1.0f);
	dirLight1.direction = Vector3(0, -0.5f, 1.0f);

	pointLight0.color = Vector3(1.0f, 0, 0);
	pointLight0.position = Vector3(4.0f, 0, 0);
	pointLight1.color = Vector3(0, 1.0f, 0);
	pointLight1.position = Vector3(0, 0, 4.0f);

	// scene
	scene.backgroundColor = 0xff1e90ff;
	scene.AddObject(&cubie);

	scene.AddDirectionalLight(&dirLight0);
	scene.AddDirectionalLight(&dirLight1);
	scene.AddPointLight(&pointLight0);
	scene.AddPointLight(&pointLight1);

	dirLight1.Disable();
	pointLight0.Disable();
	pointLight1.Disable();

	// sky
	//scene.SetSkyDome("./Textures/Skydome4.png");
	scene.SetSkyDome();
	scene.SetSkyColor(SkyDomeColor(0xff292A46, 0xff111217, 0xff111217), 
		SkyDomeColor(0xff908E7E, 0xffF47413, 0xffE88877), 
		SkyDomeColor(0xff6088BC, 0xffb4cdcd, 0xff8AB0D7),
		SkyDomeColor(0xff9F9D8D, 0xffAE677B, 0xffF7711A));
	scene.SetCloud();
	//scene.SetCloudDome("./Textures/cloud.png", 200.0f);

	DayTime::SetTimeScale(3600);
	//DayTime::SetDayTime(0, 0, 0);

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
	//FPCameraControl(Time::deltaTime);
	HoverCameraControl(cubie.mesh.position, Time::deltaTime);

	// cube - update
	static float angleY = 0;
	angleY += Time::deltaTime * 0.2f * 3.14f;

	//cubie.mesh.rotation = Quaternion(0, angleY, 0);

	/*if(Input::GetKeyDown(DIK_UP))
		DayTime::SetTimeScale(DayTime::curDayTime.scale * 2);
	if(Input::GetKeyDown(DIK_DOWN))
		DayTime::SetTimeScale(DayTime::curDayTime.scale / 2);*/

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



	//printf("cur Time: %d:%d:%d\n", DayTime::curDayTime.hour, DayTime::curDayTime.minute, DayTime::curDayTime.second);
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

	float timer = 0;
	int fps = 0;

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
		
		fps++;
		timer += deltaTime;
		if(timer > 1.0f){
			printf("fps:%d\n", fps);
			timer = 0;
			fps = 0;
		}
		//printf("fps:%f\n", 1 / deltaTime);

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

	scene.mainCamera.Translate(moveVector);
	
	if(Input::MouseState.rgbButtons[1] & 0x80){
		float rotationY = rotateSpeed * Input::MouseState.lX / 1000.0f;
		float rotationX = rotateSpeed * Input::MouseState.lY / 1000.0f;

		scene.mainCamera.Rotate(0, rotationY, 0);
		scene.mainCamera.RotateLocal(rotationX, 0, 0);
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

		panAngle += deltaPanAngle;
		tiltAngle += deltaTiltAngle;

		if(tiltAngle > maxTiltAngle)
			tiltAngle = maxTiltAngle;
		if(tiltAngle < minTiltAngle)
			tiltAngle = minTiltAngle;
	}

	if(Input::MouseState.rgbButtons[0] & 0x80){
		float deltaDistance = zoomSpeed * ( -Input::MouseState.lX + Input::MouseState.lY) / 1000.0f;

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









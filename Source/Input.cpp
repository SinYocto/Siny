#include"Input.h"
#include"D3DUtility.h"
#include<stdio.h>

char Input::lastKeyState[256];
char Input::KeyState[256];
DIMOUSESTATE Input::lastMouseState;
DIMOUSESTATE Input::MouseState;
POINT Input::lastCursorPos;
POINT Input::cursorPos;

int InitInput(HINSTANCE hInst, HWND hWnd, IDirectInput8** DI, IDirectInputDevice8** DID_Keyboard, IDirectInputDevice8** DID_Mouse)
{
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)DI,NULL)))
		return 0;
	if(FAILED((*DI)->CreateDevice(GUID_SysKeyboard, DID_Keyboard,NULL)))
		return 0;
	if(FAILED((*DI)->CreateDevice(GUID_SysMouse, DID_Mouse, NULL)))
		return 0;

	(*DID_Keyboard)->SetDataFormat(&c_dfDIKeyboard);
	(*DID_Keyboard)->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED((*DID_Keyboard)->Acquire()))
		printf("(*DID_Keyboard)->Acquire() - failed\n");

	(*DID_Mouse)->SetDataFormat(&c_dfDIMouse);
	(*DID_Mouse)->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED((*DID_Mouse)->Acquire()))
		printf("(*DID_Mouse)->Acquire() - failed\n");

	return 1;
}

void Input::GetDIDState()
{
	for(int i = 0; i < 256; ++i)
		lastKeyState[i] = KeyState[i];

	lastMouseState = MouseState;

	lastCursorPos = cursorPos;

	HRESULT hr;
	hr = DID_Keyboard->GetDeviceState(sizeof(KeyState), (void*)&KeyState);
	if(hr == DIERR_INPUTLOST){
		if(FAILED(DID_Keyboard->Acquire()))
			printf("DID_Keyboard->Acquire() - failed\n");
	}

	hr = DID_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&MouseState);
	if(hr == DIERR_INPUTLOST){
		if(FAILED(DID_Mouse->Acquire()))
			printf("DID_Mouse->Acquire() - failed\n");
	}

	GetCursorPos(&cursorPos);
	ScreenToClient(HWnd, &cursorPos);
}

bool Input::GetKeyDown(BYTE keyCode)
{
	if((KeyState[keyCode]&0x80) && !(lastKeyState[keyCode]&0x80))
		return true;
	return false;
}


bool Input::GetKeyUp(BYTE keyCode)
{
	if(!(KeyState[keyCode]&0x80) && (lastKeyState[keyCode]&0x80))
		return true;
	return false;
}

bool Input::GetKey(BYTE keyCode)
{
	if(KeyState[keyCode] & 0x80)
		return true;
	return false;
}

bool Input::GetLeftButton()
{
	if(MouseState.rgbButtons[0] & 0x80)
		return true;
	return false;
}

bool Input::GetLeftButtonDown()
{
	if((MouseState.rgbButtons[0] & 0x80) && !(lastMouseState.rgbButtons[0] & 0x80))
		return true;
	return false;
}

bool Input::GetLeftButtonUp()
{
	if(!(MouseState.rgbButtons[0] & 0x80) && (lastMouseState.rgbButtons[0] & 0x80))
		return true;
	return false;
}

bool Input::GetRightButton()
{
	if(MouseState.rgbButtons[1] & 0x80)
		return true;
	return false;
}

bool Input::GetRightButtonDown()
{
	if((MouseState.rgbButtons[1] & 0x80) && !(lastMouseState.rgbButtons[1] & 0x80))
		return true;
	return false;
}

bool Input::GetRightButtonUp()
{
	if(!(MouseState.rgbButtons[1] & 0x80) && (lastMouseState.rgbButtons[1] & 0x80))
		return true;
	return false;
}
#ifndef INPUT_H
#define INPUT_H

#define DIRECTINPUT_VERSION 0x0800
#include"dinput.h"

extern IDirectInput8 *DI;					
extern IDirectInputDevice8 *DID_Keyboard;	
extern IDirectInputDevice8 *DID_Mouse;		

int InitInput(HINSTANCE hInst, HWND hWnd, IDirectInput8** DI, IDirectInputDevice8** DID_Keyboard, IDirectInputDevice8** DID_Mouse);

class Input
{
public:
	static void GetDIDState();
	static bool GetKey(BYTE keyCode);
	static bool GetKeyDown(BYTE keyCode);

public:
	static char KeyState[256];
	static DIMOUSESTATE MouseState;
private:
	static char lastKeyState[256];
};

#endif
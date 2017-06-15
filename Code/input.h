//====================================================================================================================
//	
//	ì¸óÕèàóùÅ@[input.h]
//	Author:AKIRA TANAKA
//	
//====================================================================================================================
#ifndef  _INPUT_H_
#define	_INPUT_H_
//#define DIRECTINPUT_VERSION 0x0800
//#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib") 
#include <Windows.h>
#include <dinput.h>
//***********************************************************************************************************************
//		PrototypeêÈåæ
//***********************************************************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

bool GetKeyboardPress(int key);
bool GetKeyboardTrigger(int key);
bool GetKeyboardRepeat(int key);
bool GetKeyboardRelease(int key);

//bool GetMouseLeftPress(void);
//bool GetMouseLeftTrigger(void);
//bool GetMouseRightPress(void);
//bool GetMouseRightTrigger(void);
//bool GetMouseCenterPress(void);
//bool GetMouseCenterTrigger(void);
//long GetMouseAxisX(void);
//long GetMouseAxisY(void);
//long GetMouseAxisZ(void);
//long GetMouseX(void);
//long GetMouseY(void);



#endif // ! _INPUT_H_

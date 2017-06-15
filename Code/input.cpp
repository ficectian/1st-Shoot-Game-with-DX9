//====================================================================================================================
//	
//	入力処理　[input.cpp]
//	Author:AKIRA TANAKA
//	
//====================================================================================================================
#include "input.h"


//***********************************************************************************************************************
//	Macro定義
//***********************************************************************************************************************
#define	NUM_KEY_MAX					(256)		//	keyの最大数
#define	LIMIT_COUNT_REPEAT	(20)			//	repeat count LIMIT

//***********************************************************************************************************************
//		Prototype宣言
//***********************************************************************************************************************
HRESULT	InitInput(HINSTANCE hInstance, HWND hWnd);
//void				UninitInput(void);
//void				UpdateInput(void);
//HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
//void UninitKeyboard(void);
//HRESULT UpdateKeyboard(void);
//HRESULT InitMouse(HINSTANCE hInst, HWND hWnd);
//void UninitMouse(void);
//HRESULT UpdateMouse(void);

//***********************************************************************************************************************
//		Global変数
//***********************************************************************************************************************
LPDIRECTINPUT8					g_pDInput = NULL;												//	DirectInput interfaceへのpointer
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;								//	入力Device（keyboard）へのpointer
LPDIRECTINPUTDEVICE8	g_pDIDevMouse = NULL;										//	入力Device（mouse）へのpointer
BYTE											g_aKeyState[NUM_KEY_MAX];							//	keyboardの状態を受け取るWork
BYTE											g_aKeyStateTrigger[NUM_KEY_MAX];				//	keyboardの状態を受け取るWork
BYTE											g_aKeyStateRepeat[NUM_KEY_MAX];				//	keyboardの状態を受け取るWork
BYTE											g_aKeyStateRelease[NUM_KEY_MAX];				//	keyboardの状態を受け取るWork
int												g_aKeyStateRepeatCnt[NUM_KEY_MAX];		//	keyboardのrepeat counter
//DIMOUSESTATE2			g_mouseState;						// マウスの状態を受け取るワーク
//DIMOUSESTATE2			g_mouseStateTrigger;				// マウスの状態を受け取るワーク
//POINT					g_MousePoint;						// マウスの現在座標
//HWND					g_hwnd;
//====================================================================================================================
//		入力処理初期化
//====================================================================================================================
HRESULT	InitInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (g_pDInput == NULL)
	{
		//		DirectInput Objectの作成
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL);
		return hr;
	}

	return S_OK;
}

//====================================================================================================================
//		入力処理の終了処理
//====================================================================================================================
void		UninitInput(void)
{
	if (g_pDInput != NULL)
	{//		DirectInput Objectの開放
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//====================================================================================================================
//		入力処理の更新処理
//====================================================================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	//// マウスの更新
	//UpdateMouse();
}

//====================================================================================================================
//		keyboardの初期化
//====================================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	//		入力処理の初期化
	hr = InitInput(hInstance, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("DirectInput objectが作れ無い！"), TEXT("警告"), MB_ICONWARNING);
		return hr;
	}

	//		Device objectを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("keyboardが無い！"), TEXT("警告"), MB_ICONWARNING);
		return hr;
	}

	//		data formatを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("keyboardのdata formatを設定出来ません出下。"), TEXT("警告"), MB_ICONWARNING);
		return hr;
	}

	//		協調modeを設定（foreground＆非排他mode）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("keyboardの協調modeを設定出来ません出下。"), TEXT("警告"), MB_ICONWARNING);
		return hr;
	}

	//		keyboardのaccess権を獲得（入力制御開始）
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//====================================================================================================================
//		keyboardの終了処理
//====================================================================================================================
void		UninitKeyboard(void)
{
	if (g_pDIDevKeyboard != NULL)
	{//		入力Device（keyboard）の開放
		//		keyboardへのaccess権を開放（入力制御終了）
		g_pDIDevKeyboard->Unacquire();

		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}

	//		入力処理の終了処理
	UninitInput();
}


//====================================================================================================================
//		keyboardの更新処理
//====================================================================================================================

//HRESULT UpdateKeyboard(void)
//{
//	HRESULT hr;
//	BYTE keyStateOld[256];
//
//	// 前回のデータを保存
//	memcpy(keyStateOld, g_aKeyState, NUM_KEY_MAX);
//
//	// デバイスからデータを取得
//	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_aKeyState), g_aKeyState);
//	if (SUCCEEDED(hr))
//	{
//		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
//		{
//			// トリガーとリリースを取得
//			g_aKeyStateTrigger[cnt] = (g_aKeyState[cnt] ^ keyStateOld[cnt]) & g_aKeyState[cnt];
//			g_aKeyStateRelease[cnt] = (g_aKeyState[cnt] ^ keyStateOld[cnt]) & keyStateOld[cnt];
//
//			// キーが押されているならリピートの判定処理
//			if (g_aKeyState[cnt])
//			{
//				if (g_aKeyStateRepeatCnt[cnt] < 20)
//				{
//					g_aKeyStateRepeatCnt[cnt]++;
//					// 「初回入力」もしくは「ボタンを押してから20フレーム経過」
//					// した場合、リピート用配列のデータを有効にする
//					// 上記条件以外はリピート用配列のデータを無効にする
//					if (g_aKeyStateRepeatCnt[cnt] == 1 || g_aKeyStateRepeatCnt[cnt] >= 20) {
//						g_aKeyStateRepeat[cnt] = 0x80;
//					}
//					else {
//						g_aKeyStateRepeat[cnt] = 0;
//					}
//				}
//			}
//			else
//			{
//				g_aKeyStateRepeatCnt[cnt] = 0;
//				g_aKeyStateRepeat[cnt] = 0;
//			}
//		}
//	}
//	else
//	{
//		// キーボードへのアクセス権を取得
//		hr = g_pDIDevKeyboard->Acquire();
//	}
//
//	return hr;
//}

void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	
	//		Deviceからdataを取得
	if (SUCCEEDED(g_pDIDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			//		key trigger・release情報を生成
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & ~aKeyState[nCnKey];

			//		key repeat情報を生成
			if (aKeyState[nCnKey])
			{
				if (g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					if (g_aKeyStateRepeatCnt[nCnKey] == 1
						|| g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{//		keyを押し始めた最初のFrame、又は一定時間経過したらkey repeat情報ON
						g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
					}
					else
					{
						g_aKeyStateRepeat[nCnKey] = 0;
					}
				}
			}
			else
			{
				g_aKeyStateRepeatCnt[nCnKey] = 0;
				g_aKeyStateRepeat[nCnKey] = 0;
			}

			//		key press情報を保存
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		//		keyboardへのaccess権を取得
		g_pDIDevKeyboard->Acquire();
	}
}

//====================================================================================================================
//		keyboardのpress状態を取得
//====================================================================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//====================================================================================================================
//		keyboardのtrigger状態を取得
//====================================================================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//====================================================================================================================
//		keyboardのrepeat状態を取得
//====================================================================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}


//====================================================================================================================
//		keyboardのrelease状態を取得
//====================================================================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}
//
//
////=============================================================================
//// マウスの初期化
////=============================================================================
//HRESULT InitMouse(HINSTANCE hInst, HWND hWnd)
//{
//	HRESULT hr;
//
//	// デバイスオブジェクトを作成
//	hr = g_pDInput->CreateDevice(GUID_SysMouse, &g_pDIDevMouse, NULL);
//	if (FAILED(hr) || g_pDIDevMouse == NULL)
//	{
//		MessageBox(hWnd, TEXT("マウスがねぇ！"), TEXT("警告！"), MB_ICONWARNING);
//		return hr;
//	}
//
//	// データフォーマットを設定
//	hr = g_pDIDevMouse->SetDataFormat(&c_dfDIMouse2);
//	if (FAILED(hr))
//	{
//		MessageBox(hWnd, TEXT("マウスのデータフォーマットを設定できませんでした。"), TEXT("警告！"), MB_ICONWARNING);
//		return hr;
//	}
//
//	// 協調モードを設定（フォアグラウンド＆非排他モード）
//	hr = g_pDIDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
//	if (FAILED(hr))
//	{
//		MessageBox(hWnd, TEXT("マウスの協調モードを設定できませんでした。"), TEXT("警告！"), MB_ICONWARNING);
//		return hr;
//	}
//
//	// デバイスの設定
//	{
//		DIPROPDWORD dipdw;
//
//		dipdw.diph.dwSize = sizeof(dipdw);
//		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
//		dipdw.diph.dwObj = 0;
//		dipdw.diph.dwHow = DIPH_DEVICE;
//		dipdw.dwData = DIPROPAXISMODE_REL;
//
//		hr = g_pDIDevMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
//	}
//
//	// キーボードへのアクセス権を獲得(入力制御開始)
//	g_pDIDevMouse->Acquire();
//
//	return S_OK;
//}
//
////=============================================================================
//// マウスの終了処理
////=============================================================================
//void UninitMouse(void)
//{
//	if (g_pDIDevMouse)
//	{
//		g_pDIDevMouse->Unacquire();
//
//		g_pDIDevMouse->Release();
//		g_pDIDevMouse = NULL;
//	}
//}
//
////=============================================================================
//// マウスの更新処理
////=============================================================================
//HRESULT UpdateMouse(void)
//{
//	HRESULT hr;
//	DIMOUSESTATE2 mouseStateOld;
//
//	// 前回のデータを保存
//	mouseStateOld = g_mouseState;
//
//	GetCursorPos(&g_MousePoint);
//	ScreenToClient(g_hwnd, &g_MousePoint);
//
//
//	// デバイスからデータを取得
//	hr = g_pDIDevMouse->GetDeviceState(sizeof(g_mouseState), &g_mouseState);
//	if (SUCCEEDED(hr))
//	{
//		// トリガーの取得
//		//g_mouseStateTrigger.lX = 
//		//g_mouseStateTrigger.lY = 
//		//g_mouseStateTrigger.lZ = 
//		for (int cnt = 0; cnt < 8; cnt++)
//		{
//			g_mouseStateTrigger.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ g_mouseState.rgbButtons[cnt]) & g_mouseState.rgbButtons[cnt]);
//		}
//	}
//	else
//	{
//		g_pDIDevMouse->Acquire();
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// マウスデータ取得(左プレス)
////=============================================================================
//bool GetMouseLeftPress(void)
//{
//	return ((g_mouseState.rgbButtons[0] & 0x80) != 0);
//}
//
////=============================================================================
//// マウスデータ取得(左トリガー)
////=============================================================================
//bool GetMouseLeftTrigger(void)
//{
//	return ((g_mouseStateTrigger.rgbButtons[0] & 0x80) != 0);
//}
//
////=============================================================================
//// マウスデータ取得(右プレス)
////=============================================================================
//bool GetMouseRightPress(void)
//{
//	return ((g_mouseState.rgbButtons[1] & 0x8) != 00);
//}
//
////=============================================================================
//// マウスデータ取得(右トリガー)
////=============================================================================
//bool GetMouseRightTrigger(void)
//{
//	return ((g_mouseStateTrigger.rgbButtons[1] & 0x80) != 0);
//}
//
////=============================================================================
//// マウスデータ取得(中央プレス)
////=============================================================================
//bool GetMouseCenterPress(void)
//{
//	return ((g_mouseState.rgbButtons[2] & 0x80) != 0);
//}
//
////=============================================================================
//// マウスデータ取得(中央トリガー)
////=============================================================================
//bool GetMouseCenterTrigger(void)
//{
//	return ((g_mouseState.rgbButtons[2] & 0x80) != 0);
//}
//
////=============================================================================
//// マウスデータ取得(Ｘ軸移動)
////=============================================================================
//long GetMouseAxisX(void)
//{
//	return g_mouseState.lX;
//}
//
////=============================================================================
//// マウスデータ取得(Ｙ軸移動)
////=============================================================================
//long GetMouseAxisY(void)
//{
//	return g_mouseState.lY;
//}
//
////=============================================================================
//// マウスデータ取得(Ｚ軸移動)
////=============================================================================
//long GetMouseAxisZ(void)
//{
//	return g_mouseState.lZ;
//}
////=============================================================================
//// マウス座標取得(X)
////=============================================================================
//long GetMouseX(void)
//{
//	return g_MousePoint.x;
//}
//
////=============================================================================
//// マウス座標取得(Y)
////=============================================================================
//long GetMouseY(void)
//{
//	return g_MousePoint.y;
//}
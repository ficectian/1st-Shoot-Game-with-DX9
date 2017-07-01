#include "main.h"
//=========================================
//		Global変数
//=========================================
extern LPDIRECT3D9					g_pD3D;				//		Direct3D Object
extern LPDIRECT3DDEVICE9	g_pD3DDevice;	//		Device Object（描画に必要）


//=========================================
//		初期化処理
//		hInstance	：	InstanceのHandle
//		hWnd			：	WindowのHandle
//		bWindow	：	Window Modeにするかどうか
//=========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE	d3ddm;

	//		Direct3D Objectの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//		
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//		DeviceのPresentation Parameterの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));									//		Workを0 Clear
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//		Game画面Size（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;					//		Game画面Size（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;						//		Color Modeの指定
	d3dpp.BackBufferCount = 1;													//		Back Bufferの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			//		映像信号に同期してClipする
	d3dpp.Windowed = bWindow;												//		Window Mode
	d3dpp.EnableAutoDepthStencil;											//		デプスBuffer（ZBuffer）とStencil Buffer作成
	d3dpp.AutoDepthStencilFormat;											//		デプスBufferとして16bitを使う

	if (bWindow)
	{//		Window Mode
		d3dpp.FullScreen_RefreshRateInHz = 0;																//		Refresh Late（指定できないので０固定）
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		//		Interval（VSyncを待たずに描画）
	}
	else
	{//		Full Screen Mode
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//		Refresh Late（現在の速度に合わせる）
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			//		Interval（VSyncを待たずに描画）
	}

	//		Device Objectの生成
	//		「Device作成制御」＜描画＞と＜頂点処理＞をHardwareで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		//		Display Adapter
		D3DDEVTYPE_HAL,																				//		Device Type
		hWnd,																										//		FocusするWindowへのHandle
		D3DCREATE_HARDWARE_VERTEXPROCESSING,						//		Device作成制御の組み合わせ
		&d3dpp,																									//		DeviceのPresentation Parameter
		&g_pD3DDevice)))																				//		Device InterfaceへのPointer
	{
		//		上記の設定が失敗したら
		//		「Device作成制御」＜描画＞をHardwareで行い、＜頂点処理＞をCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		//		
			D3DDEVTYPE_HAL,																				//		
			hWnd,																										//		
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,						//		
			&d3dpp,																									//		
			&g_pD3DDevice)))
		{
			//		上記の設定が失敗したら
			//		「Device作成制御」＜描画＞と＜頂点処理＞をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		//		
				D3DDEVTYPE_REF,																				//		
				hWnd,																										//		
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,						//		
				&d3dpp,																									//		
				&g_pD3DDevice)))
			{
				//		初期化失敗
				return	E_FAIL;
			}
		}
	}

	return S_OK;
}


//=========================================
//		終了処理
//=========================================
void Uninit(void)
{
	if (g_pD3DDevice != NULL)
	{//		Deviceの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{//		Direct3D Objectの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}
#include	"main.h"
#include "DXpolygon.h"
#include "Player.h"
#include "input.h"

#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")//multimedia library
//=========================================
//		
//=========================================
#define	CLASS_NAME		TEXT("DogShootQuin")				//		WindowのClass名
#define	WINDOW_NAME	TEXT("逆襲の犬頭人")	//		WindowのCaption名



//=========================================
//		Prototype宣言
//=========================================
LRESULT	CALLBACK	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void	Uninit(void);
void	Update(int);
void	Draw(int);


LPDIRECT3DTEXTURE9	WinTex;
LPDIRECT3DTEXTURE9	LoseTex;

extern PLAYER Player;
extern ENEMY	Enemy[ENEMYNUM];
extern SHOOT Shoot[SHOOTNUM];
extern ENEMYSHOOT EnemyShoot[ENEMYSHOOTMAX];
extern IMAGE *Image;

int Status;
//=========================================
//		Global変数
//=========================================
LPDIRECT3D9					g_pD3D = NULL;				//		Direct3D Object
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	//		Device Object（描画に必要）

//=========================================
//		Main関数
//=========================================
int	APIENTRY		WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX	wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		(HICON)LoadImage(NULL,WINDOWICO,IMAGE_ICON,0,0,LR_LOADFROMFILE),
		//NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND	hWnd;
	MSG	msg;
	
	//		Window Classの登錄
	RegisterClassEx(&wcex);

	//		Windowの作成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL);
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IMAGE_ICON));

	//		初期化处理（Window作成してから行う）
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}
	//InitInput(hInstance, hWnd);
	Status = TITLE;
	Image->Init();
	//		Window表示（初期化处理の後に行う）
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	InitKeyboard(hInstance, hWnd);

	bool 
	bret = DXLoadTexture(WINTEX, &WinTex);
	bret = DXLoadTexture(LOSETEX, &LoseTex);

	DWORD StartTime = timeGetTime();
	DWORD NowTime;
	int fcnt = 0;
	msg.message = NULL;
	//		Message Loop
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)		//Messageを取得しなかった場合「０」を返す
		{	//		Windowsの処理
			if (msg.message == WM_QUIT)
			{//		PostQuitMessage()がよばれて、WM_QUITMessageが来たらLoop終了
				break;
			}
			else
			{
				//		Messageの編訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//		DirectXの处理
			NowTime=timeGetTime();
			
			DWORD deltaTime = NowTime - StartTime;

			if (deltaTime > (float)1000 / 60) {
				fcnt += 1;
				if (fcnt > 60) { fcnt = 0; }
				deltaTime = 0;
				StartTime = timeGetTime();



				//		更新处理
				Update(fcnt);

				//		描画处理
				Draw(fcnt);
			}
		}
	}

	//		Window Class
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//		终了处理
	UninitInput();
	Uninit();
	
	return (int)msg.wParam;
}

//=========================================
//		Window Procedure
//=========================================
LRESULT	CALLBACK	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
	//unsigned char KeyState[256];
	switch (uMsg)
	{
	//case	WM_CREATE:
	//	SetTimer(hWnd, NULL, 50, FALSE);
	//	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	//case WM_TIMER:
	//	InvalidateRect(hWnd, NULL, FALSE);
	//	break;

	//case WM_LBUTTONDOWN:
	//	for (int i = 0; i < SHOOTNUM; i++) {
	//		if (Shoot[i].Flag == HITOFF) {
	//			Shoot[i].X = Player.X + Player.Width / 2;
	//			Shoot[i].Y = Player.Y + 15;
	//			Shoot[i].Flag = HITON;
	//			Shoot[i].Disp = true;
	//			break;
	//		}
	//	}
	//	break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case	VK_ESCAPE:
			nID = MessageBox(hWnd, TEXT("終了しますか？"), TEXT("終了"), MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(hWnd, TEXT("終了しますか？"), TEXT("終了"), MB_YESNO);
		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=========================================
//		初期化処理
//		hInstance	：	InstanceのHandle
//		hWnd			：	WindowのHandle
//		bWindow	：	Window Modeにするかどうか
//=========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd,BOOL bWindow)
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

//=========================================
//		更新処理
//=========================================
void Update(int fcnt)
{
	UpdateInput();

	switch (Status)
	{
	case TITLE:
		if (GetKeyboardPress(DIK_TAB)) { Status = GAME_PLAY; }
		break;
	case GAME_PLAY:
		Image->Update();
		//==================================================
		//	Move& Shoot
		//==================================================
		Player.Update(); //Player Move
		for (int i = 0; i < ENEMYNUM; i++) { Enemy[i].Update(i); }//敵移動	
		Shoot->Update(fcnt);
		EnemyShoot->Update();
		EnemyShoot->Hit();
		for (int i = 0; i < SHOOTNUM; i++) { Shoot[i].Hit(i); }

		for (int i = 0; i< ENEMYNUM; i++) {
			if (HitTest(Player.X, Player.Y, Player.Width, Player.Height, Enemy[i].X, Enemy[i].Y, Enemy[i].Width, Enemy[i].Height) && Enemy[i].nHp >0 && Player.InvincibleState <= 0) {
				Player.X -= 30;
				Player.nHp -= 1;
				Player.InvincibleState = 60;
			}
		}
		if (Player.nHp <= -10) { Status = GAME_OVER; }
		if (Enemy[0].nHp <= -5 && Enemy[1].nHp <= -5 && Enemy[2].nHp <= -5) { Status = GAME_WIN; }
		break;
	case GAME_OVER:
		if (GetKeyboardPress(DIK_RETURN)) { Status = TITLE; }
		break;
	case GAME_WIN:
		if (GetKeyboardPress(DIK_RETURN)) { Status = TITLE; }
		break;
	default:
		break;
	}
	




	
}

//=========================================
//		描画処理
//=========================================
void	Draw(int fcnt)
{
	//		BackBuffer＆ZBufferのClear
	//		Count	：	pRects配列にある矩形の数
	//		pRects	：	Clearする矩形の配列（NULL指定でView Port矩形全体をClear）
	//		Flags		：	ClearするSurfaceを示すFlag（少なくとも1つを使用しなければならない）
	//						：	「D3DCLEAR_TARGET	ー	Rendering TargetをClearしてColor Parameterの値にする」
	//						：	「D3DCLEAR_ZBUFFER	ー	深度（Z）BufferをClearしてZ Parameterの値にする」
	//						：	「D3DCLEAR_STENCIL	ー	Stencil BufferをClearしてStencil Parameterの値にする」
	//		Color		：	SurfaceをClearする色
	//		Z				：	デプスBufferに保存する値
	//		Stencil	：	Stencil Bufferに保存する値（整数）
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//		Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (Status)
		{
		case TITLE:
			Image->TitleDraw(fcnt);
			Player.Init(); //player初期化
			for (int i = 0; i < ENEMYNUM; i++) { Enemy[i].Init(i); }//敵初期化
			for (int i = 0; i < SHOOTNUM; i++) { Shoot[i].Init(i); }
			EnemyShoot->Init();
			break;
		case GAME_PLAY:
			Image->BackDraw();
			Player.Draw();
			for (int i = 0; i < ENEMYNUM; i++) { Enemy[i].Draw(); }
			Shoot->Draw();
			EnemyShoot->Draw();
			Image->UpDraw(Player.nHp);
			break;
		case GAME_OVER:
			DXDrawPolygon(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_RGBA(255, 255, 255, 255), LoseTex);
			break;
		case GAME_WIN:
			DXDrawPolygon(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_RGBA(255, 255, 255, 255), WinTex);
			break;
		default:
			break;
		}


		

		

		
		//		Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}
	
	//		Back BufferとFront Bufferの入れ替え
	//		pSourceRect						：	転送元矩形
	//		pDestRect							：	転送先矩形
	//		hDestWindowOverride	：	転送先WindowへのPointer
	//		pDirtyRegion						：	通常はNULLで
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//α blend可能
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	//α値＝0.5
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//背景のα値　＝　1-α =0.5

}




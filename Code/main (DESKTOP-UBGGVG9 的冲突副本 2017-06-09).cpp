#include	"main.h"
#include "DXpolygon.h"
#include "Player.h"
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
void	Update(void);
void	Draw(void);
LPDIRECT3DTEXTURE9 PlayerTex;
LPDIRECT3DTEXTURE9 ScrenTex;
LPDIRECT3DTEXTURE9 CloudTex;
LPDIRECT3DTEXTURE9 ShootTex;
LPDIRECT3DTEXTURE9 EnemyTex;
LPDIRECT3DTEXTURE9	BoowTex;
LPDIRECT3DTEXTURE9	EnemyBoowTex;
LPDIRECT3DTEXTURE9	Heat1Tex;
LPDIRECT3DTEXTURE9	Heat2Tex;
LPDIRECT3DTEXTURE9	EnemyShootTex;
LPDIRECT3DTEXTURE9	InvincibleTex;
LPDIRECT3DTEXTURE9	WinTex;
LPDIRECT3DTEXTURE9	LoseTex;
DATA Player;
DATA	Enemy[ENEMYNUM];
SHOOT Shoot[SHOOTNUM];
SHOOT EnemyShoot[ENEMYSHOOTMAX];
int EnemyShootNum = 0;
int ShootLoop = 0;
float * DogReturn;
int InvincibleState = 0;
float Cloud_X = SCREEN_WIDTH;
//int	ShootFlag[SHOOTNUM];// = { HITOFF,HITOFF,HITOFF,HITOFF,HITOFF };
//float ShootX[SHOOTNUM];	// = {5.0f,10.0f,15.0f,20.0f,25.0f};
//float	DefShotX[SHOOTNUM];
//float ShootY[SHOOTNUM];// = {DEFSHOTY,DEFSHOTY,DEFSHOTY,DEFSHOTY,DEFSHOTY};
//bool ShootDisp[SHOOTNUM];
//float PlayerX = SCREEN_WIDTH / 2;
//float PlayerY = SCREEN_HEIGHT / 2;
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
	Player.X =  100;
	Player.Y = SCREEN_HEIGHT / 2;
	Player.Wdith = PLAYER_WIDTH;
	Player.Height = PLAYER_HEIGHT;
	Player.nHp = 6;
	for (int i = 0; i < ENEMYNUM; i++) {
		Enemy[i].Height = ENEMY_HEIGHT;
		Enemy[i].Wdith = ENEMY_WIDTH;
		Enemy[i].Flg = 0.0f;
		Enemy[i].nHp = 3;
	}
	Enemy[0].X = 550;
	Enemy[0].Y = ENEMY0DEFY;
	Enemy[1].X = 400;
	Enemy[1].Y = ENEMY1DEFY;
	Enemy[2].X = 550;
	Enemy[2].Y = ENEMY2DEFY;
	for (int i = 0; i < SHOOTNUM; i++) {
		Shoot[i].DefX = SHOOTGAP*i + 20.0f;
		Shoot[i].X = Shoot[i].DefX;
		Shoot[i].Y = DEFSHOTY;
		Shoot[i].Flag = HITOFF;
		Shoot[i].Disp = false;
	}

	//		Window表示（初期化处理の後に行う）
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	bool bret = DXLoadTexture(PLAYERTEX, &PlayerTex);
	bret = DXLoadTexture(SCRENTEX, &ScrenTex);
	bret = DXLoadTexture(ENEMYTEX, &EnemyTex);
	bret = DXLoadTexture(ENEMYTEX, &EnemyTex);
	bret = DXLoadTexture(BOOWTEX, &BoowTex);
	bret = DXLoadTexture(CLOUDTEX, &CloudTex);
	bret = DXLoadTexture(ENEMYBOOWTEX, &EnemyBoowTex);
	bret = DXLoadTexture(SHOOTTEX, &ShootTex);
	bret = DXLoadTexture(ENEMYSHOOTTEX, &EnemyShootTex);
	bret = DXLoadTexture(HEAT1TEXT, &Heat1Tex);
	bret = DXLoadTexture(HEAT2TEXT, &Heat2Tex);
	bret = DXLoadTexture(INVINCIBLETEX, &InvincibleTex);
	bret = DXLoadTexture(WINTEX, &WinTex);
	bret = DXLoadTexture(LOSETEX, &LoseTex);
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
	//	else
	//	{//		DirectXの处理
	//		//		更新处理
	//		Update();

	//		//		描画处理
	//		Draw();
	//	}
	}

	//		Window Class
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//		终了处理
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
	case	WM_CREATE:
		SetTimer(hWnd, NULL, 50, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, FALSE);
		//PlayerOpreations(Player.X, Player.Y, Player.Wdith, Player.Height, Enemy[0].X, Enemy[0].Y, Enemy[0].Wdith, Enemy[0].Height, wParam);
		//GetKeyboardState(KeyState);
		if (InvincibleState > 0) {
			InvincibleState -= 1;
		}
		//==================================================
		//	Move& Shoot
		//==================================================
		if (Player.nHp > 0) {
			if ((::GetAsyncKeyState(VK_UP) | ::GetAsyncKeyState(87)) & 0x8000) {
				Player.Y = UpMove(Player.X, Player.Y, Player.Wdith, Player.Height);					//上移動
			}
			if ((::GetAsyncKeyState(VK_DOWN) | ::GetAsyncKeyState(83)) & 0x8000) {
				Player.Y = DownMove(Player.X, Player.Y, Player.Wdith, Player.Height);					//移動
			}
			if ((::GetAsyncKeyState(VK_LEFT) | ::GetAsyncKeyState(65)) & 0x8000) {
				Player.X = LeftMove(Player.X, Player.Y, Player.Wdith, Player.Height);				//上移動
			}
			if ((::GetAsyncKeyState(VK_RIGHT) | ::GetAsyncKeyState(68)) & 0x8000) {
				Player.X = RightMove(Player.X, Player.Y, Player.Wdith, Player.Height);					//上移動
			}
			if (::GetAsyncKeyState(VK_SPACE) & 0x8000) {
				for (int i = 0; i < SHOOTNUM; i++) {
					if (Shoot[i].Flag == HITOFF) {
						Shoot[i].X = Player.X + Player.Wdith / 2;
						Shoot[i].Y = Player.Y + 15;
						Shoot[i].Flag = HITON;
						Shoot[i].Disp = true;
						break;
					}
				}
			}
		}
		
		//Shooting Move
		for (int i = 0; i < SHOOTNUM; i++)
		{
			if (Shoot[i].Flag == HITON) {
				Shoot[i].X += 15.0f;
			}
			if (Shoot[i].X > SCREEN_WIDTH) {
				Shoot[i].Flag = HITOFF;
				Shoot[i].X = Shoot[i].DefX;
				Shoot[i].Y = DEFSHOTY;
				Shoot[i].Disp = false;
			}
		}
		for (int i = 0; i < EnemyShootNum; i++)
		{
			if (EnemyShoot[i].Flag == HITON) {
				EnemyShoot[i].X -= 10.0f;
			}
			if (EnemyShoot[i].X <0) {
				EnemyShoot[i].Flag = HITOFF;
				EnemyShoot[i].X = 0;
				EnemyShoot[i].Y = 0;
				EnemyShoot[i].Disp = false;
			}
		}

		//Dog
		for (int i = 0; i < ENEMYNUM; i++) {
			if (Enemy[i].nHp > 0) {
				DogReturn = DogMove(Enemy[i].X, Enemy[i].Y, i, Enemy[i].Flg);
				Enemy[i].X = *DogReturn;
				Enemy[i].Y = *(DogReturn + 1);
				Enemy[i].Flg = *(DogReturn + 2);
			}
		}
		if (ShootLoop == 60) {
			ShootLoop = 0;
			EnemyShootNum = 0;
			for (int i = 0; i < ENEMYNUM; i++) {
				EnemyShootNum += 2;
				if (Enemy[i].nHp > 0) {
					EnemyShoot[2 * i].X = Enemy[i].X - Enemy[i].Wdith / 2;
					EnemyShoot[2 * i].Y = Enemy[i].Y - 6;
					EnemyShoot[2 * i].Flag = HITON;
					EnemyShoot[2 * i].Disp = true;
					EnemyShoot[2 * i + 1].X = Enemy[i].X - Enemy[i].Wdith / 2;
					EnemyShoot[2 * i + 1].Y = Enemy[i].Y +10;
					EnemyShoot[2 * i + 1].Flag = HITON;
					EnemyShoot[2 * i + 1].Disp = true;
				}
			}
		}
		else
		{
			ShootLoop += 1;
		}

		//Hit
		//Shoot
		for (int i = 0; i < SHOOTNUM; i++) {
			if (Shoot[i].Disp) {
				for (int n = 0; n < ENEMYNUM; n++) {
					if (HitTest(Shoot[i].X, Shoot[i].Y, SHOOTSIZE, SHOOTSIZE, Enemy[n].X, Enemy[n].Y, Enemy[n].Wdith, Enemy[n].Height) && Enemy[n].nHp >0) {
						Shoot[i].Disp = false;
						Enemy[n].nHp -= 1;
					}
				}
			}
		}

		for (int i = 0; i< ENEMYNUM; i++) {
			if (HitTest(Player.X, Player.Y, Player.Wdith, Player.Height, Enemy[i].X, Enemy[i].Y, Enemy[i].Wdith, Enemy[i].Height) && Enemy[i].nHp >0 && InvincibleState <=0) {
				Player.X -= 30;
				Player.nHp -= 1;
				InvincibleState = 10;
			}
		}

		for (int i = 0; i < EnemyShootNum; i++) {
			if (EnemyShoot[i].Disp) {
				for (int n = 0; n < ENEMYNUM; n++) {
					if (HitTest(EnemyShoot[i].X, EnemyShoot[i].Y, ENEMYSHOOTSIZE, ENEMYSHOOTSIZE, Player.X, Player.Y, Player.Wdith, Player.Height) && Player.nHp >0 && InvincibleState <= 0) {
						Shoot[i].Disp = false;
						Player.X -= 30;
						Player.nHp -= 1;
						InvincibleState = 10;
					}
				}
			}
		}


		//CLOUD
		Cloud_X -= 1.0f;
		if (Cloud_X + CLOUDWDITH / 2 < 0) {
			Cloud_X = SCREEN_WIDTH + CLOUDWDITH/2;
		}
		break;

	case WM_LBUTTONDOWN:
		for (int i = 0; i < SHOOTNUM; i++) {
			if (Shoot[i].Flag == HITOFF) {
				Shoot[i].X = Player.X + Player.Wdith / 2;
				Shoot[i].Y = Player.Y + 15;
				Shoot[i].Flag = HITON;
				Shoot[i].Disp = true;
				break;
			}
		}
		break;
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
		//case 87:
		//case VK_UP:
		//	//MoveMod = MOVEMODDEF;
		//	for (int i = 0; i < ENEMYNUM; i++) {
		//		Player.Y = UpMove(Player.X, Player.Y, Player.Wdith, Player.Height, Enemy[i].X, Enemy[i].Y, Enemy[i].Wdith, Enemy[i].Height);					//上移動
		//	}
		//	break;
		//case 83:
		//case VK_DOWN:
		//	//MoveMod = MOVEMODDEF;
		//	for (int i = 0; i < ENEMYNUM; i++) {
		//		Player.Y = DownMove(Player.X, Player.Y, Player.Wdith, Player.Height, Enemy[i].X, Enemy[i].Y, Enemy[i].Wdith, Enemy[i].Height);					//上移動
		//	}
		//	break;
		//case 65:
		//case VK_LEFT:
		//	//MoveMod = MOVEMODLEFT;
		//	for (int i = 0; i < ENEMYNUM; i++) {
		//		Player.X = LeftMove(Player.X, Player.Y, Player.Wdith, Player.Height, Enemy[i].X, Enemy[i].Y, Enemy[i].Wdith, Enemy[i].Height);					//上移動
		//	}
		//	break;
		//case 68:
		//case VK_RIGHT:
		//	//MoveMod = MOVEMODRIGHT;
		//	for (int i = 0; i < ENEMYNUM; i++) {
		//		Player.X = RightMove(Player.X, Player.Y, Player.Wdith, Player.Height, Enemy[i].X, Enemy[i].Y, Enemy[i].Wdith, Enemy[i].Height);					//上移動
		//	}
		//	break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		Update();
		Draw();
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
void Update(void)
{
}

//=========================================
//		描画処理
//=========================================
void	Draw(void)
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
		DXDrawPolygon(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_RGBA(255, 255, 255, 255), ScrenTex);
		DXDrawPolygon(Cloud_X, CLOUD_Y , 0, CLOUDWDITH, CLOUDHEIGHT, D3DCOLOR_RGBA(255, 255, 255, 255), CloudTex);

		if (InvincibleState > 0 && Player.nHp > 0) {
			DXDrawPolygon(Player.X, Player.Y, 0, INVINCIBLE_SIZE, INVINCIBLE_SIZE, D3DCOLOR_RGBA(255, 255, 255, 100), InvincibleTex);
		}
		if (Player.nHp > 0) {
			DXDrawPolygon(Player.X, Player.Y, 0, Player.Wdith, Player.Height, D3DCOLOR_RGBA(255, 255, 255, 255), PlayerTex);//player
		}
		else if (Player.nHp > -10) {
			DXDrawPolygon(Player.X, Player.Y, 0, Player.Wdith, Player.Height, D3DCOLOR_RGBA(255, 255, 255, 255), BoowTex);//player
			Player.nHp -= 1;
		}
		
		//DXDrawPolygon(Player.X, Player.Y, 0, Player.Wdith, Player.Height, D3DCOLOR_RGBA(255, 255, 255, 50), ShadowTex);
	
		for (int i = 0; i < ENEMYNUM; i++) {
			if (Enemy[i].nHp > 0) {
				DXDrawPolygon(Enemy[i].X, Enemy[i].Y, 0, Enemy[i].Wdith, Enemy[i].Height, D3DCOLOR_RGBA(255, 255, 255, 255), EnemyTex);
			}
			else if(Enemy[i].nHp > -5)
			{
				DXDrawPolygon(Enemy[i].X, Enemy[i].Y, 0, Enemy[i].Wdith, Enemy[i].Height, D3DCOLOR_RGBA(255, 255, 255, 255), EnemyBoowTex);
				Enemy[i].nHp -= 1;
			}
			else
			{
				Enemy[i].X = -100.0f;
				Enemy[i].Y = -100.0f;
			}
			
		}
		for (int i = 0; i < SHOOTNUM; i++)
		{
			if (Shoot[i].Disp || Shoot[i].Flag == HITOFF) {
				DXDrawPolygon(Shoot[i].X, Shoot[i].Y, 0, SHOOTSIZE, SHOOTSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), ShootTex);
			}
		}
		for (int i = 0; i < EnemyShootNum; i++)
		{
			if (EnemyShoot[i].Disp && EnemyShoot[i].Flag == HITON) {
				DXDrawPolygon(EnemyShoot[i].X, EnemyShoot[i].Y, 0, ENEMYSHOOTSIZE, ENEMYSHOOTSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), EnemyShootTex);
			}
		}
		for (int i = 0; i < Player.nHp; i++) {
			if (Player.nHp >= 3) {
				for (int n = 0; n < 3; n++) {
					DXDrawPolygon(500 + n*HEATGAP,15, 0, HEATSIZE, HEATSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), Heat1Tex);
				}
				for (int n = 0; n < Player.nHp - 3; n++) {
					DXDrawPolygon(500 + n*HEATGAP, 15, 0, HEATSIZE, HEATSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), Heat2Tex);
				}
			}else{
				for (int n = 0; n < Player.nHp; n++) {
					DXDrawPolygon(500 + n*HEATGAP, 15, 0, HEATSIZE, HEATSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), Heat1Tex);
				}
			}
		}
		//DXDrawPolygon(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_RGBA(255, 255, 255, 255), ScrenupTex);
		if (Player.nHp <= -10) {
			DXDrawPolygon(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_RGBA(255, 255, 255, 255), LoseTex);
		}
		if (Enemy[0].nHp <= -5 && Enemy[1].nHp <= -5 && Enemy[2].nHp <= -5) {
			DXDrawPolygon(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_RGBA(255, 255, 255, 255), WinTex);
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
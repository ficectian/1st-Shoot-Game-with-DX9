#include	"main.h"
#include "DXpolygon.h"
#include "Player.h"
#include "input.h"
//==========================================================================================================
//		関数定義
//==========================================================================================================
int	HitSwitch = HITOFF;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		上移動関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float	UpMove(float X1, float Y1, float W1, float H1) {
	//各頂点の移動
	if (Y1 - H1 / 2 > 0)
	{
		Y1 -= PLAYERSPEED;
	}
	return Y1;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		左移動関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float	LeftMove(float X1, float Y1, float W1, float H1) {
	//各頂点の移動
	if (X1 - W1 / 2 > 0)
	{
		X1 -= PLAYERSPEED;
	}
	return X1;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		右移動関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float	RightMove(float X1, float Y1, float W1, float H1) {
	//各頂点の移動
	if (X1 + W1 / 2 < SCREEN_WIDTH)
	{
		X1 += PLAYERSPEED;
	}
	return X1;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		下移動関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float	DownMove(float X1, float Y1, float W1, float H1) {
	//各頂点の移動
	if (Y1 + H1 / 2 <SCREEN_HEIGHT)
	{
		Y1 += PLAYERSPEED;
	}
	return Y1;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		当たり判定処理関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool			HitTest(float X1, float Y1, float W1, float H1, float X2, float Y2, float W2, float H2)
{
	float XX1 = X1 - W1 / 2;
	float	XX2 = X2 - W2 / 2;
	float YY1 = Y1 - H1 / 2;
	float YY2 = Y2 - H2 / 2;

	if (((YY1 + H1 >= YY2) && (YY1 - H2 <= YY2)) && ((XX1 + W1 >= XX2) && (XX1 - W2 <= XX2)))
	{
		return true;
	}
	else
	{
		return	false;
	}

}



//==========================================================================================================
//		定義
//==========================================================================================================
int EnemyShootNum = 0;

PLAYER Player;
ENEMY Enemy[ENEMYNUM];
SHOOT Shoot[SHOOTNUM];
ENEMYSHOOT EnemyShoot[ENEMYSHOOTMAX];
void PLAYER::Init() {
	bool bret = DXLoadTexture(PLAYERTEX, &Tex);
	bret = DXLoadTexture(BOOWTEX, &DeadTex);
	bret = DXLoadTexture(INVINCIBLETEX, &InvincibleTex);
	X = 100;
	Y = SCREEN_HEIGHT / 2;
	Width = 80;
	Height = 64;
	nHp = 6;
	InvincibleState = 0;
	Ustart = 0.0f;
	Uwidth = 0.25f;
	Vstart = 0.0f;
	Vheight = 1.0f;
	
	//const int PlayerAnime1[16] = { 0,0,0,1,1,1,2,2,2,3,3,3,0xff };//0xff：終了コード
	Anime_style = 0;
	cnt = 0;
}

void PLAYER::Update() {
	if (InvincibleState > 0) {
		InvincibleState -= 1;
	}
	if (nHp > 0) {
		if (GetKeyboardPress(DIK_W) | GetKeyboardPress(DIK_UP)) {
			Y = UpMove(X, Y, Width, Height);					//上移動
		}
		if (GetKeyboardPress(DIK_S) | GetKeyboardPress(DIK_DOWN)) {
			Y = DownMove(X, Y, Width, Height);					//移動
		}
		if (GetKeyboardPress(DIK_A) | GetKeyboardPress(DIK_LEFT)) {
			X = LeftMove(X, Y, Width, Height);				//上移動
		}
		if (GetKeyboardPress(DIK_D)| GetKeyboardPress(DIK_RIGHT))
		{
			X = RightMove(X, Y, Width, Height);
		}

	}
	Ustart = ((*(Animept + cnt))%int(1/Uwidth))*Uwidth;
	cnt += 1;
	if (*(Animept + cnt) == 0xff) { cnt = 0; }
	//cnt += 1;
	//if (cnt == 5) {
	//	Player.Ustart += Player.Uwidth;
	//	cnt = 0;
	//}
}

void PLAYER::Draw() {
	if (InvincibleState > 0 && Player.nHp > 0) {
		DXDrawPolygon(Player.X, Player.Y, 0, INVINCIBLE_SIZE, INVINCIBLE_SIZE, D3DCOLOR_RGBA(255, 255, 255, 100), InvincibleTex);
	}
	if (Player.nHp > 0) {
		DXDrawAnimePolygon(X,Y, 0, Width, Height, Ustart, Uwidth, Vstart, Vheight, D3DCOLOR_RGBA(255, 255, 255, 255), Tex);//player
	}
	else if (Player.nHp > -10) {
		DXDrawPolygon(X, Y, 0, Width, Height, D3DCOLOR_RGBA(255, 255, 255, 255), DeadTex);//player
		Player.nHp -= 1;
	}
}

void ENEMY::Init(int i) {
	bool bret = DXLoadTexture(ENEMYTEX, &Tex);
	bret = DXLoadTexture(ENEMYBOOWTEX, &DeadTex);
	Height = 50;
	Width = 70;
	Flg = 0.0f;
	nHp = 3;
	Ustart = 0.0f;
	Uwidth = 1.0f;
	Vstart = 0.0f;
	Vheight = 1.0f;
	cnt = 0;
	Hitting = 0;
	switch (i)
	{
	case 0:
		X = SCREEN_WIDTH - 100;
		Y = SCREEN_HEIGHT/2 - 150;
		break;
	case 1:
		X = SCREEN_WIDTH - 250;
		Y = SCREEN_HEIGHT / 2;
		break;
	case 2:
		X = SCREEN_WIDTH - 100;
		Y = SCREEN_HEIGHT / 2 + 150;
		break;
	}
}

void ENEMY::Update(int i) {
	if (nHp > 0) {
		switch (i + int(Flg))
		{
		case 0:
			X -= 2.0f;
			Y += 2.0f;
			if (Y == ENEMY1DEFY) { Flg = 1.0f - i; }
			break;
		case 1:
			X += 2.0f;
			Y += 2.0f;
			if (Y == ENEMY2DEFY) { Flg = 2.0f - i;; }
			break;
		case 2:
			Y -= 4.0f;
			if (Y == ENEMY0DEFY) { Flg = 0.0f - i;; }
			break;
		default:
			break;
		}
	}
	if (Hitting > 0) { Hitting -= 1; }
}
void ENEMY::Draw() {
	if (nHp > 0) {
		if(Hitting <= 0){ DXDrawPolygon(X, Y, 0, Width, Height, D3DCOLOR_RGBA(255, 255, 255, 255), Tex); }
	}
	else if (nHp > -5)
	{
		DXDrawPolygon(X, Y, 0, Width, Height, D3DCOLOR_RGBA(255, 255, 255, 255), DeadTex);
		nHp -= 1;
	}
	else
	{
		X = -100.0f;
		Y = -100.0f;
	}
}

void SHOOT::Init(int i){
	bool bret = DXLoadTexture(SHOOTTEX, &Tex);
	Shoot[i].DefX = SHOOTGAP*i + 20.0f;
	Shoot[i].X = Shoot[i].DefX;
	Shoot[i].Y = DEFSHOTY;
	Shoot[i].Flag = HITOFF;
	Shoot[i].Disp = false;
}

void SHOOT::Update(int fcnt) {
	if (Player.nHp > 0) {
		if (GetKeyboardPress(DIK_SPACE)) {
			for (int i = 0; i < SHOOTNUM; i++) {
				if (Shoot[i].Flag == HITOFF & notshooting) {
					Shoot[i].X = Player.X + Player.Width / 2;
					Shoot[i].Y = Player.Y + 15;
					Shoot[i].Flag = HITON;
					Shoot[i].Disp = true;
					notshooting = false;
					break;
				}
			}
		}
	}
	if (fcnt % 10 == 0) { notshooting = true; }
	for (int i = 0; i < SHOOTNUM; i++)
	{
		if (Shoot[i].Flag == HITON) {
			Shoot[i].X += 6.0f;
		}
		if (Shoot[i].X > SCREEN_WIDTH) {
			Shoot[i].Flag = HITOFF;
			Shoot[i].X = Shoot[i].DefX;
			Shoot[i].Y = DEFSHOTY;
			Shoot[i].Disp = false;
		}
	}
}
void SHOOT::Hit(int i) {
	if (Shoot[i].Disp) {
		for (int n = 0; n < ENEMYNUM; n++) {
			if (HitTest(Shoot[i].X, Shoot[i].Y, SHOOTSIZE, SHOOTSIZE, Enemy[n].X, Enemy[n].Y, Enemy[n].Width, Enemy[n].Height) && Enemy[n].nHp > 0 && Enemy[n].Hitting <= 0) {
				Shoot[i].Disp = false;
				Enemy[n].nHp -= 1;
				Enemy[n].Hitting = 5;
			}
		}
	}
}
void SHOOT::Draw() {
	for (int i = 0; i < SHOOTNUM; i++)
	{
		if (Shoot[i].Disp || Shoot[i].Flag == HITOFF) {
			DXDrawPolygon(Shoot[i].X, Shoot[i].Y, 0, SHOOTSIZE, SHOOTSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), Shoot[i].Tex);
		}
	}
}

void ENEMYSHOOT::Init(){
	bool bret = DXLoadTexture(ENEMYSHOOTTEX, &Tex);
}

void ENEMYSHOOT::Update() {
	for (int i = 0; i < EnemyShootNum; i++)
	{
		if (EnemyShoot[i].Flag == HITON) {
			EnemyShoot[i].X -= 7.0f;
		}
		if (EnemyShoot[i].X <0) {
			EnemyShoot[i].Flag = HITOFF;
			EnemyShoot[i].X = 0;
			EnemyShoot[i].Y = 0;
			EnemyShoot[i].Disp = false;
		}
	}
	if (Loop == 120) {
		Loop = 0;
		EnemyShootNum = 0;
		for (int i = 0; i < ENEMYNUM; i++) {
			EnemyShootNum += 2;
			if (Enemy[i].nHp > 0) {
				EnemyShoot[2 * i].X = Enemy[i].X - Enemy[i].Width / 2;
				EnemyShoot[2 * i].Y = Enemy[i].Y - 6;
				EnemyShoot[2 * i].Flag = HITON;
				EnemyShoot[2 * i].Disp = true;
				EnemyShoot[2 * i + 1].X = Enemy[i].X - Enemy[i].Width / 2;
				EnemyShoot[2 * i + 1].Y = Enemy[i].Y + 10;
				EnemyShoot[2 * i + 1].Flag = HITON;
				EnemyShoot[2 * i + 1].Disp = true;
			}
		}
	}
	else
	{
		Loop += 1;
	}
}

void ENEMYSHOOT::Hit() {
	for (int i = 0; i < EnemyShootNum; i++) {
		if (EnemyShoot[i].Disp) {
			for (int n = 0; n < ENEMYNUM; n++) {
				if (HitTest(EnemyShoot[i].X, EnemyShoot[i].Y, ENEMYSHOOTSIZE, ENEMYSHOOTSIZE, Player.X, Player.Y, Player.Width, Player.Height) && Player.nHp >0 && Player.InvincibleState <= 0) {
					Shoot[i].Disp = false;
					Player.X -= 30;
					Player.nHp -= 1;
					Player.InvincibleState = 10;
				}
			}
		}
	}
}

void ENEMYSHOOT::Draw() {
	for (int i = 0; i < EnemyShootNum; i++)
	{
		if (EnemyShoot[i].Disp && EnemyShoot[i].Flag == HITON) {
			DXDrawPolygon(EnemyShoot[i].X, EnemyShoot[i].Y, 0, ENEMYSHOOTSIZE, ENEMYSHOOTSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), Tex);
		}
	}
}

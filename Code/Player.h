#include <windows.h>
#include <d3dx9.h>			//	�`�揈���ɕK�v
#define PLAYERTEX TEXT("image/player.png")
#define	LANDTEX TEXT("image/land.png")
#define SCRENTEX	TEXT("image/scren.png")
//#define SCRENUPTEX TEXT("image/screnup.png")
#define ENEMYTEX TEXT("image/enemy.png")
#define BOOWTEX TEXT("image/BOOW.png")
#define CLOUDTEX TEXT("image/Cloud.png")
#define ENEMYBOOWTEX TEXT("image/EnemyBOOW.png")
#define SHOOTTEX	TEXT("image/Shoot.png")
#define ENEMYSHOOTTEX	TEXT("image/EnemyShoot.png")
#define HEAT1TEXT	TEXT("image/Heat1.png")
#define HEAT2TEXT TEXT("image/Heat2.png")
#define	INVINCIBLETEX	 TEXT("image/Invincible.png")
#define	WINTEX	 TEXT("image/WIN.png")
#define	LOSETEX	 TEXT("image/LOSE.png")
#define	TITLETEX	 TEXT("image/title.png")
#define	TITLEINTTEX	TEXT("image/titleint.png")


#define PLAYER_HEIGHT	(64)
#define	PLAYER_WIDTH	(80)
#define INVINCIBLE_SIZE (90)
#define ENEMYNUM (3)
#define ENEMY_HEIGHT	(50)
#define	ENEMY_WIDTH	(70)

#define ENEMY0DEFY (SCREEN_HEIGHT / 2 - 150)
#define ENEMY1DEFY  (SCREEN_HEIGHT / 2)
#define ENEMY2DEFY (SCREEN_HEIGHT / 2 + 150)
#define	HITOFF			(0)							//
#define	HITON			(1)							//
#define SHOOTNUM		(7)
#define SHOOTSIZE		(10)
#define ENEMYSHOOTSIZE		(6)
#define SHOOTGAP		(15)
#define DEFSHOTY		(20.0f)
#define PLAYERSPEED (5)
#define CLOUDHEIGHT (164)
#define CLOUDWDITH (695)
#define CLOUD_Y (SCREEN_HEIGHT-70-CLOUDHEIGHT/2)
#define HEATSIZE		(20.0f)
#define HEATGAP		(30.0f)
#define WINDOWICO TEXT("image/ico.ico")
#define ENEMYSHOOTMAX (255)

//=========================================
//		�\���̒�`
//=========================================
typedef struct {
	LPDIRECT3DTEXTURE9 Tex;
	LPDIRECT3DTEXTURE9	DeadTex;
	float X;	//�N��
	float Y;	//�U����
	float Width;
	float Height;
	float Ustart;
	float Vstart;
	float Uwidth;
	float Vheight;
	int nHp;	//�̗�
	float Flg;
	int Hitting;
	void Init(int);
	void Update(int);
	void Draw();
	int cnt;
}ENEMY;

typedef struct {
	LPDIRECT3DTEXTURE9 Tex;
	LPDIRECT3DTEXTURE9	DeadTex;
	LPDIRECT3DTEXTURE9	InvincibleTex;
	float X;	//�N��
	float Y;	//�U����
	float Width;
	float Height;
	float Ustart;
	float Vstart;
	float Uwidth;
	float Vheight;
	int	nHp;	//�̗�
	int InvincibleState;
	float Flg;
	void Init();
	void Update();
	void Draw();
	const int Anime1[32] = { 0,0,0,0,0,0,0,0,0,0,1,1,1,2,2,2,2,2,2,2,2,1,1,1,0xff };//0xff�F�I���R�[�h
	const int Anime2[16] = { 8,8,9,9,0xff };
	const int *Anime_data[2] = { Anime1,Anime2 };
	int Anime_style;// = 0;
	const int *Animept = Anime_data[Anime_style];
	int cnt;
}PLAYER;

typedef struct{
	LPDIRECT3DTEXTURE9 Tex;
	int		Flag;
	float	X;	
	float	DefX;
	float	Y;
	bool	Disp;
	bool notshooting = true;
	void Init(int);
	void Update(int);
	void Hit(int);
	void Draw();
}SHOOT;

typedef struct {
	LPDIRECT3DTEXTURE9 Tex;
	int		Flag;
	float	X;
	float	Y;
	bool	Disp;
	int Loop = 0;
	void Init();
	void Update();
	void Hit();
	void Draw();
}ENEMYSHOOT;

typedef struct  {
	LPDIRECT3DTEXTURE9 Tex;
	float	X; 
	float	Y;
	float Width;
	float Height;
	float Ustart;
	float Vstart;
	float Uwidth;
	float Vheight;
	void Init();
	void Update();
	void TitleDraw(int);
	void BackDraw();
	void UpDraw(int);
}IMAGE;

enum {
	TITLE,
	GAME_PLAY,
	RESULT,
	GAME_OVER,
	GAME_WIN
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		�֐��錾
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float		UpMove(float, float, float, float);				//��ړ��֐�
float		RightMove(float, float, float, float);			//���ړ��֐�
float		LeftMove(float, float, float, float);				//���ړ��֐�
float		DownMove(float, float, float, float);				//���ړ��֐�
bool		HitTest(float, float, float, float, float, float, float, float);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

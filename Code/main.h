#ifndef _MAIN_H_
#define	_MAIN_H_

//=========================================
//		Include File
//=========================================
#include <windows.h>
#include <d3dx9.h>			//	描画処理に必要
#include <tchar.h>
//=========================================
//		LibraryのLink
//=========================================
#if 1																				// 	ここを"0"にした場合、「構成Property」－＞「Link」－＞「入力」－＞「追加の依存File」に対象Libraryを設定する
#pragma	comment	(lib, "d3d9.lib")		//		描画処理に必要
#pragma	comment	(lib, "d3dx9.lib")	//		[d3d9.lib]の拡張Library
#pragma	comment	(lib, "dxguid.lib")	//		DirectX Component使用に必要
#endif 

//=========================================
//		Macroの定義
//=========================================
#define	SCREEN_WIDTH		(960)		//		Windowの幅
#define	SCREEN_HEIGHT	(544)		//		Windowの高さ
//#define	SCREEN_WIDTH		(640)		//		Windowの幅
//#define	SCREEN_HEIGHT	(480)		//		Windowの高さ
#endif




//struct VERTEX2D{			//三角形
//	float x, y, z;					//頂点座標
//	float rhw;						//同次座標 =１
//	D3DCOLOR  diffuse;	//拡散光
//};
//struct VERTEX2DTEX{		//四角形
//	float x, y, z;						//頂点座標
//	float rhw;							//同次座標 =１
//	D3DCOLOR  diffuse;		//拡散光
//	float u, v;							//uv座標
//};

//bool DXLoadTexture(char *, LPDIRECT3DTEXTURE9 *);
//void DXDrawPolygon(float, float, float, float, float, DWORD, LPDIRECT3DTEXTURE9);
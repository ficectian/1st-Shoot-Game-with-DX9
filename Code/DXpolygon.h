#pragma once		//移動Include guard

#include <windows.h>
#include <d3dx9.h>			//	描画処理に必要
#include <tchar.h>

bool DXLoadTexture(LPCWSTR filename, LPDIRECT3DTEXTURE9 * tex);
void DXDrawPolygon(float x, float y, float z, float width, float height, DWORD color, LPDIRECT3DTEXTURE9 tex);

void DXDrawAnimePolygon(float x, float y, float z, float width, float height, float u, float uw, float v, float vh, DWORD color, LPDIRECT3DTEXTURE9 tex);

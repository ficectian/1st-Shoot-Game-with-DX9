#include "main.h"

extern LPDIRECT3DDEVICE9 g_pD3DDevice;	//一番上
struct VERTEX2DTEX{
	float x, y, z;
	float rhw;
	DWORD color;
	float u, v;
};

bool DXLoadTexture( 	//p78
	//char * filename,
	LPCWSTR filename,
	LPDIRECT3DTEXTURE9 * tex){
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(g_pD3DDevice, filename, tex);
	if (FAILED(hr)){
		MessageBox(NULL, TEXT("texture error"), TEXT("Tex"), MB_OK);
			return false;
	}
	return true;

}

void DXDrawPolygon(float x, float y, float z, float width, float height, DWORD color, LPDIRECT3DTEXTURE9 tex){
	VERTEX2DTEX v[4];
	v[0].x = x - width / 2;
	v[0].y = y - height / 2;
	v[0].z = z;
	v[0].u = 0.0f;
	v[0].v = 0.0f;
	v[0].color = color;
	v[0].rhw = 1.0f;  //同次座標

	v[1].x = x + width / 2;
	v[1].y = y - height / 2;
	v[1].z = z;
	v[1].u = 1.0f;
	v[1].v = 0.0f;
	v[1].color = color;
	v[1].rhw = 1.0f;  //同次座標

	v[2].x = x - width / 2;
	v[2].y = y + height / 2;
	v[2].z = z;
	v[2].u = 0.0f;
	v[2].v = 1.0f;
	v[2].color = color;
	v[2].rhw = 1.0f;  //同次座標

	v[3].x = x + width / 2;
	v[3].y = y + height / 2;
	v[3].z = z;
	v[3].u = 1.0f;
	v[3].v = 1.0f;
	v[3].color = color;
	v[3].rhw = 1.0f;  //同次座標

	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);	//頂点format
	g_pD3DDevice->SetTexture(0, tex);	//textureを貼る
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VERTEX2DTEX));	//三角形描画
}

void DXDrawAnimePolygon(float x, float y, float z, float width, float height, float u, float uw, float v, float vh,DWORD color, LPDIRECT3DTEXTURE9 tex) {

	VERTEX2DTEX vPt[4];
	vPt[0].x = x - width / 2;
	vPt[0].y = y - height / 2;
	vPt[0].z = z;
	vPt[0].u = u;
	vPt[0].v = v;
	vPt[0].color = color;
	vPt[0].rhw = 1.0f;  //同次座標

	vPt[1].x = x + width / 2;
	vPt[1].y = y - height / 2;
	vPt[1].z = z;
	vPt[1].u = u + uw;
	vPt[1].v = v;
	vPt[1].color = color;
	vPt[1].rhw = 1.0f;  //同次座標

	vPt[2].x = x - width / 2;
	vPt[2].y = y + height / 2;
	vPt[2].z = z;
	vPt[2].u = u;
	vPt[2].v = v + vh;
	vPt[2].color = color;
	vPt[2].rhw = 1.0f;  //同次座標

	vPt[3].x = x + width / 2;
	vPt[3].y = y + height / 2;
	vPt[3].z = z;
	vPt[3].u = u + uw;
	vPt[3].v = v + vh;
	vPt[3].color = color;
	vPt[3].rhw = 1.0f;  //同次座標

	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);	//頂点format
	g_pD3DDevice->SetTexture(0, tex);	//textureを貼る
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPt, sizeof(VERTEX2DTEX));	//三角形描画

}

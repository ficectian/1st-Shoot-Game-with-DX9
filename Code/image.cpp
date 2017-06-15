#include	"main.h"
#include "DXpolygon.h"
#include "Player.h"
#include "input.h"

IMAGE *Image;
IMAGE	Title;
IMAGE	Titleint;
IMAGE Scren;
IMAGE Land;
IMAGE Cloud;
IMAGE	Heat1;
IMAGE	Heat2;

void IMAGE::Init() {
	bool bret = DXLoadTexture(TITLETEX, &Title.Tex);
	bret = DXLoadTexture(TITLEINTTEX, &Titleint.Tex);
	bret = DXLoadTexture(SCRENTEX, &Scren.Tex);
	bret = DXLoadTexture(LANDTEX, &Land.Tex);
	bret = DXLoadTexture(CLOUDTEX, &Cloud.Tex);
	bret = DXLoadTexture(HEAT1TEXT, &Heat1.Tex);
	bret = DXLoadTexture(HEAT2TEXT, &Heat2.Tex);

	Title.Width = SCREEN_WIDTH;
	Title.Height = SCREEN_HEIGHT;
	Title.X = Title.Width / 2;
	Title.Y = Title.Height / 2;
	Titleint.Width = 219;
	Titleint.Height = 65;
	Titleint.X = Titleint.Width / 2 + 50;
	Titleint.Y = SCREEN_HEIGHT - 50 - Titleint.Height / 2;
	Land.Width = 1024;
	Land.Height = 512;
	Land.X = Land.Width / 2;
	Land.Y = SCREEN_HEIGHT - Land.Height / 2;
	Land.Ustart = 0.0f;
	Land.Uwidth = 1.0f;
	Land.Vstart = 0.0f;
	Land.Vheight = 1.0f;
	Cloud.Width = 695;
	Cloud.Height = 164;
	Cloud.X = SCREEN_WIDTH;
	Cloud.Y = SCREEN_HEIGHT - 70 - Cloud.Height / 2;
	Scren.Width = SCREEN_WIDTH;
	Scren.Height = SCREEN_HEIGHT;
	Scren.X = Scren.Width / 2;
	Scren.Y = Scren.Height / 2;

}

void IMAGE::Update(){
	Land.Ustart += 0.0003f;
	Cloud.X -= 1.0f;
	if (Cloud.X + CLOUDWDITH / 2 < 0) {
		Cloud.X = SCREEN_WIDTH + CLOUDWDITH / 2;
	}
}
void IMAGE::TitleDraw(int fcnt) {
	DXDrawPolygon(Title.X, Title.Y, 0, Title.Width, Title.Height, D3DCOLOR_RGBA(255, 255, 255, 255), Title.Tex);
	if (fcnt > 30) {
		DXDrawPolygon(Titleint.X, Titleint.Y, 0, Titleint.Width, Titleint.Height, D3DCOLOR_RGBA(255, 255, 255, 255), Titleint.Tex);
	}

}
void IMAGE::BackDraw() {
	DXDrawPolygon(Scren.X, Scren.Y, 0, Scren.Width, Scren.Height, D3DCOLOR_RGBA(255, 255, 255, 255), Scren.Tex);
	DXDrawAnimePolygon(Land.X, Land.Y, 0, Land.Width, Land.Height, Land.Ustart, Land.Uwidth, Land.Vstart, Land.Vheight,D3DCOLOR_RGBA(255, 255, 255, 255), Land.Tex);
	DXDrawPolygon(Cloud.X, Cloud.Y, 0, Cloud.Width, Cloud.Height, D3DCOLOR_RGBA(255, 255, 255, 255), Cloud.Tex);

}

void IMAGE::UpDraw(int HP) {
	for (int i = 0; i < HP; i++) {
		if (HP >= 3) {
			for (int n = 0; n < 3; n++) {
				DXDrawPolygon((SCREEN_WIDTH-100) + n*HEATGAP, 15, 0, HEATSIZE, HEATSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), Heat1.Tex);
			}
			for (int n = 0; n < HP - 3; n++) {
				DXDrawPolygon((SCREEN_WIDTH - 100) + n*HEATGAP, 15, 0, HEATSIZE, HEATSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), Heat2.Tex);
			}
		}
		else {
			for (int n = 0; n < HP; n++) {
				DXDrawPolygon((SCREEN_WIDTH - 100) + n*HEATGAP, 15, 0, HEATSIZE, HEATSIZE, D3DCOLOR_RGBA(255, 255, 255, 255), Heat1.Tex);
			}
		}
	}
}





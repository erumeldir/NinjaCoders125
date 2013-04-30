#pragma once
// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9d.lib")
#include <d3dx9.h>
#include "defs.h"
#include <time.h>

class HeadsUpDisplay
{
public:
	HeadsUpDisplay(LPDIRECT3DDEVICE9 direct3dDevice);
	virtual ~HeadsUpDisplay(void);

	void displayText(string hudText, string monsterHUDText);
	void displayHealthBars(int playerHealth, int monsterHealth);
	void displayBackground();
	void diplayGameOver();

private:
	ID3DXFont* direct3dText; // the pointer to the device class

	LPD3DXSPRITE sprite;
	LPD3DXSPRITE sprite1;
	LPD3DXSPRITE sprite2;
	
	IDirect3DTexture9 *g_texture;
	IDirect3DTexture9 *test1_texture;

	int hudTopX, hudTopY;

	LPD3DXLINE healthLine;
	LPD3DXLINE monsterLine;
	LPD3DXLINE backgroundLine;

	clock_t initTime;
};


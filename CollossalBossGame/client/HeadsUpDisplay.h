#pragma once
// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9d.lib")
#include <d3dx9.h>
#include "defs.h"
#include <time.h>
#include "ConfigurationManager.h"
#include "PlayerCObj.h"
#include "ClientObjectManager.h"
#include "ClientEngine.h"
#include <map>

class HeadsUpDisplay
{
public:
	HeadsUpDisplay(LPDIRECT3DDEVICE9 direct3dDevice);
	virtual ~HeadsUpDisplay(void);

	void displayText(string hudText, string monsterHUDText);
	void displayHealthBars(int playerHealth, int monsterHealth);
	void displayBackground();
	void displayGameOver();
	void displayStart(bool & gamestarted);

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

	LPD3DXSPRITE p1connect;
	LPD3DXSPRITE p2connect;
	LPD3DXSPRITE p3connect;
	LPD3DXSPRITE p4connect;
	LPD3DXSPRITE youarep1;
	LPD3DXSPRITE youarep2;
	LPD3DXSPRITE youarep3;
	LPD3DXSPRITE youarep4;
	LPD3DXSPRITE pressstart;
	LPD3DXSPRITE playerready;
	LPD3DXSPRITE blackbackground;
	IDirect3DTexture9 *p1connecttxt;
	IDirect3DTexture9 *p2connecttxt;
	IDirect3DTexture9 *p3connecttxt;
	IDirect3DTexture9 *p4connecttxt;
	IDirect3DTexture9 *youarep1txt;
	IDirect3DTexture9 *youarep2txt;
	IDirect3DTexture9 *youarep3txt;
	IDirect3DTexture9 *youarep4txt;
	IDirect3DTexture9 *pressstarttxt;
	IDirect3DTexture9 *playerreadytxt;
	IDirect3DTexture9 *blackbackgroundtxt;

	clock_t initTime;
};

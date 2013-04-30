/*
* RenderEngine.h
* Defines the render engine class.
* This object is specific to the client.
*/

#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9d.lib")
#include <d3dx9.h>

#include <list>
#include <stdio.h>
#include <string>
#include "ClientObject.h"
#include "XAnimator_lib.h"
#include "Camera.h"
#include "HeadsUpDisplay.h"
#include <time.h>

using namespace std;


// define the screen resolution
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

#include "RenderModel.h"

class RenderEngine {
public:
	static void init() { re = new RenderEngine(); }
	static RenderEngine *get() { return re; }
	static void clean() { delete re; }

	void render();

	LPDIRECT3D9 direct3dInterface; // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 direct3dDevice; // the pointer to the device class
	void renderThis(ClientObject *obj);
	
	Camera * getCamera() { return cam; }
	void updateCamera(const Point_t &pos, const Rot_t &rot);

	void setHUDText(string newText, int health) { hudText = newText; healthPts = health; }
	void setMonsterHUDText(string newText, int health) { monsterHUDText = newText; monsterHealthPts = health; }

	//Models
	void animate(int id, const D3DXMATRIX &pos);
	bool loadModel(const char * filename, int * idAddr, const D3DXMATRIX &rootMat);

	bool debugFlag;
	//Debug
	IXAnimator *getAnim() { return xAnimator; }


private:
	void startWindow ();
	void renderInitalization();	//the stuff that can't be pulled from here
	void sceneDrawing();
	void drawHUD();
	void gamestartdisplayinit();
	void gamestartdisplaylogic();

	RenderEngine();
	virtual ~RenderEngine();

	//Static members
	static RenderEngine *re;
	static IXAnimator* xAnimator;
	D3DXMATRIX world;
	string hudText;
	string monsterHUDText;
	int healthPts;
	int monsterHealthPts;

	HWND windowHandle;	
	list<ClientObject *> lsObjs;

	Camera* cam;
	HeadsUpDisplay* hud;

	//Configuration fields
	float cameraDist;

	// Game Start Sprite and Logic Fields
	//LPD3DXSPRITE p1connect;
	//LPD3DXSPRITE p2connect;
	//LPD3DXSPRITE p3connect;
	//LPD3DXSPRITE p4connect;
	//LPD3DXSPRITE youarep1;
	//LPD3DXSPRITE youarep2;
	//LPD3DXSPRITE youarep3;
	//LPD3DXSPRITE youarep4;
	//LPD3DXSPRITE pressstart;
	//LPD3DXSPRITE playerready;
	//LPD3DXSPRITE blackbackground;
	//IDirect3DTexture9 *p1connecttxt;
	//IDirect3DTexture9 *p2connecttxt;
	//IDirect3DTexture9 *p3connecttxt;
	//IDirect3DTexture9 *p4connecttxt;
	//IDirect3DTexture9 *youarep1txt;
	//IDirect3DTexture9 *youarep2txt;
	//IDirect3DTexture9 *youarep3txt;
	//IDirect3DTexture9 *youarep4txt;
	//IDirect3DTexture9 *pressstarttxt;
	//IDirect3DTexture9 *playerreadytxt;
	//IDirect3DTexture9 *blackbackgroundtxt;
	bool gamestarted; // begins as false, when everyone's pressed start, then set this to true.
	bool gamestartedtexturesinitialized;
};
typedef RenderEngine RE;

#endif


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
#include "ClientObject.h"
#include "XAnimator_lib.h"
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

	D3DXMATRIX getViewOffset() { return camera; }

	void renderThis(ClientObject *obj);
	
	void setCameraPos(const Point_t &pos, const Point_t &rot);

	void setCameraInfo(const Point_t &lookAt, const Point_t &pos, const Point_t &up);

	//Models
	void animate(int id, const D3DXMATRIX &pos);
	bool loadModel(const char * filename, int * idAddr);

private:
	void startWindow ();
	void renderInitalization();	//the stuff that can't be pulled from here
	void sceneDrawing();

	RenderEngine();
	virtual ~RenderEngine();

	//Static members
	static RenderEngine *re;
	static IXAnimator* xAnimator;
	D3DXMATRIX camera, world;
	D3DXVECTOR3 camLookAt, camPos, camUp;

	float xpos, ypos, zpos;

	HWND windowHandle;	
	list<ClientObject *> lsObjs;
};
typedef RenderEngine RE;

#endif


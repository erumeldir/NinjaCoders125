/*
 * RenderEngine.h
 * Defines the render engine class.
 * This object is specific to the client.
 */

#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

#include <d3dx9.h>

// define the screen resolution
#define SCREEN_WIDTH  1024 
#define SCREEN_HEIGHT 768

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class RenderEngine {
public:
	static void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow) { re = new RenderEngine(hInstance, hPrevInstance, nCmdShow); }
	static RenderEngine *get() { return re; }
	static void clean() { delete re; }
	
	void render();
	
	LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

private:
	void startWindow (HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow);
	void coreInitialization();		//the stuff that can't be pulled from here
	
	HWND hWnd;						//

	LPDIRECT3DVERTEXBUFFER9 v_buffer;

	RenderEngine(HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow);
	virtual ~RenderEngine();

	static RenderEngine *re;
};
typedef RenderEngine RE;

#endif

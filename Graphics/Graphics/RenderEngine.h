/*
 * RenderEngine.h
 * Defines the render engine class.
 * This object is specific to the client.
 * 
 * Authors: Bryan, Nathan
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
LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

class RenderEngine {
public:
	static void init(HINSTANCE hInstance) { re = new RenderEngine(hInstance); }
	static RenderEngine *get() { return re; }
	static void clean() { delete re; }
	
	void render();
	
	LPDIRECT3D9 direct3dInterface;    // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 direct3dDevice;    // the pointer to the device class

private:
	void startWindow (HINSTANCE hInstance);
	void renderInitalization();		//the stuff that can't be pulled from here
	void sceneDrawing();

	HWND windowHandle;	

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

	RenderEngine(HINSTANCE hInstance);
	virtual ~RenderEngine();

	static RenderEngine *re;
};
typedef RenderEngine RE;

#endif

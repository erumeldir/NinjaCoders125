/* 
 * GraphicsTester.cpp
 * The current file which runs the rendering. To be replaced
 *
 * One problem: the Renderer needs hInstance. If we find another way to get hInstance
 *   we don't need to use the WINAPI version of main.
 *
 * Authors: Bryan
 */

#include "RenderEngine.h"

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int windowShowFlag)
{
	//Initialize the window and renderer
	RenderEngine::init(hInstance);

	//we've gotten the renderer
	RenderEngine * re = RenderEngine::get();

	

	// enter the main loop:
	MSG msg;	//for his stuff...

	while(TRUE)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			break;

		//call a renderer
		re->render();
	}

	// clean up the thing
	RenderEngine::clean();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			} break;
	}

	return DefWindowProc (windowHandle, message, wParam, lParam);
}


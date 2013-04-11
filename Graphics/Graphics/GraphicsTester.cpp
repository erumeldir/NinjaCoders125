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
#include "CModel.h"


	D3DXMATRIXA16 matWorld, matYWorld, matXWorld, matTranslate, matUp, matView, matProj, matZWorld;

	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	//Movement variables
	static float fAngle = 0.0f;
	static float fForward = 0.0f;
	static float fUp = 0.0f;
	static float fAngle2 = 0.0f;
	static float fAngle3 = 0.0f;
	static bool bWireframe = false;


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

	//make it
	CModel * g_pModel = new CModel(re->direct3dDevice);
							//Load the new
							g_pModel->LoadXFile((char*)LPCTSTR("tiny.x"));

							//Make sure we can see it
							float radius = g_pModel->GetBoundingSphereRadius();
							vEyePt = D3DXVECTOR3(0.0f, radius*2, -(radius*2));
							LPD3DXVECTOR3 ptemp = g_pModel->GetBoundingSphereCenter();
							vLookatPt = D3DXVECTOR3(ptemp->x, ptemp->y, ptemp->z);
							fAngle = 0.0f;
							fForward = 0.0f;
							fUp = 0.0f;
							fAngle2 = 0.0f;

	// enter the main loop:
	MSG msg;	//for his stuff...

	re->load();
	re->direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

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
		re->render(g_pModel);
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


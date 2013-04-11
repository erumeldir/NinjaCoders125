/*
 * RenderEngine.cpp
 * Defines the source code for the Render Engine.
 * Authors: Bryan, Nathan
 */
#include "RenderEngine.h"
#include "Models.h"
#include "CAllocateHierarchy.h"

RenderEngine *RenderEngine::re;

/* create a window that we will render in
 *   hInstance: current application instance
 *	 
 *   Returns nothing
 *   Bryan
 */

void RenderEngine::startWindow(HINSTANCE hInstance)
{
	static WNDCLASSEX windowClass;

	//Getting some memory to define the class
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	//define the class's elements
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.lpszClassName = L"WindowClass";

	//create the class
	RegisterClassEx(&windowClass);

	windowHandle = CreateWindowEx(NULL,
						  L"WindowClass",
						  L"Our First Direct3D Program",
						  WS_EX_TOPMOST | WS_POPUP,
						  0, 0,
						  SCREEN_WIDTH, SCREEN_HEIGHT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	ShowWindow(windowHandle, 1);	//1 is the flag for whether to show the window
}

/*Create window, initialize everything (the model information is currently here, general init is under renderInitaliztion
 * Bryan
 */
RenderEngine::RenderEngine(HINSTANCE hInstance) {
	startWindow(hInstance);
	maxBones = 0;
	firstMesh = NULL;

	renderInitalization();		//start initialization

	//now initialize any models we want to have first
	 // create three vertices using the CUSTOMVERTEX struct built earlier
	CUSTOMVERTEX vertices[] =
	{
		{ 320.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
		{ 520.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ 120.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
	};

	// create the vertex and store the pointer into vertexBuffer, which is created globally
	direct3dDevice->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX),
							   0,
							   CUSTOMFVF,
							   D3DPOOL_MANAGED,
							   &vertexBuffer,
							   NULL);

	VOID* vertexInfo;    // the void pointer

	vertexBuffer->Lock(0, 0, (void**)&vertexInfo, 0);    // lock the vertex buffer
	memcpy(vertexInfo, vertices, sizeof(vertices));    // copy the vertices to the locked buffer
	vertexBuffer->Unlock();    // unlock the vertex buffer

}

/*Initialize the 3d Render Environment
 * Bryan
 */
D3DPRESENT_PARAMETERS deviceInfo;    // create a struct to hold various device information

void RenderEngine::renderInitalization()
{
	direct3dInterface = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	
	ZeroMemory(&deviceInfo, sizeof(deviceInfo));    // clear out the struct for use
	deviceInfo.Windowed = TRUE;    // program windowed, not fullscreen
	deviceInfo.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	deviceInfo.hDeviceWindow = windowHandle;    // set the window to be used by Direct3D
	deviceInfo.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
	deviceInfo.BackBufferWidth = SCREEN_WIDTH;    // set the width of the buffer
	deviceInfo.BackBufferHeight = SCREEN_HEIGHT;    // set the height of the buffer

	// create a device class using this information and information from the deviceInfo stuct
	direct3dInterface->CreateDevice(D3DADAPTER_DEFAULT,
					  D3DDEVTYPE_HAL,
					  windowHandle,
					  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					  &deviceInfo,
					  &direct3dDevice);
}

/*release all resources
 * Bryan
 */
RenderEngine::~RenderEngine() {
	direct3dDevice->Release();    // close and release the 3D device
	direct3dInterface->Release();    // close and release Direct3D
}

/*where we actually draw a scene
* Bryan
*/
void RenderEngine::sceneDrawing() {
	//model drawing
	LPMESHCONTAINER pMesh = firstMesh;

	//While there is a mesh try to draw it
	while(pMesh)
	{
		//Select the mesh to draw
		LPD3DXMESH pDrawMesh = (pMesh->pSkinInfo)
			? pMesh->pSkinMesh: pMesh->MeshData.pMesh;
		
		//Draw each mesh subset with correct materials and texture
		for (DWORD i = 0; i < pMesh->NumMaterials; ++i)
		{
			direct3dDevice->SetMaterial(&pMesh->pMaterials9[i]);
			direct3dDevice->SetTexture(0, pMesh->ppTextures[i]);
			pDrawMesh->DrawSubset(i);
		}

		//Go to the next one
		pMesh = (LPMESHCONTAINER)pMesh->pNextMeshContainer;
	}

}

/*we set up and clean up from rendering a scene
 * Bryan
 */

LARGE_INTEGER nowtime;
void RenderEngine::render(CModel * g_pModel) {
		LONGLONG ticks;

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	double starttime;
	starttime = (double)time.QuadPart;

	QueryPerformanceFrequency(&time);
	ticks = time.QuadPart;

	// clear the window to a deep blue
	direct3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	static int iFrames = 0;
			static double sttime = 0.0;
			iFrames++;
			if (iFrames == 100) 
			{
				QueryPerformanceCounter(&nowtime);
				float fps = float(iFrames/((nowtime.QuadPart - sttime)/ticks));
				LPCWCHAR cBuff = L"X File Model Viewer - Press \'C\' to view the controls FPS: ";
				SetWindowText(deviceInfo.hDeviceWindow,cBuff);
				sttime = (double)nowtime.QuadPart;
				iFrames = 0;
			}
			
			//////////////////////////////////////////////////////////////////////////
			// Setup the world and view
			//////////////////////////////////////////////////////////////////////////
			
	//Movement variables
	static float fAngle = 0.0f;
	static float fForward = 0.0f;
	static float fUp = 0.0f;
	static float fAngle2 = 0.0f;
	static float fAngle3 = 0.0f;
	static bool bWireframe = false;
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixIdentity(&matYWorld);
			D3DXMatrixIdentity(&matXWorld);
			D3DXMatrixIdentity(&matZWorld);
			D3DXMatrixIdentity(&matTranslate);
			D3DXMatrixIdentity(&matUp);

			
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);


			//This makes the model rotate Y
			D3DXMatrixRotationY( &matYWorld, D3DXToRadian(fAngle));
			//This makes the model rotate X
			D3DXMatrixRotationX( &matXWorld, D3DXToRadian(fAngle2));
			//This makes the model rotate Z
			D3DXMatrixRotationZ( &matZWorld, D3DXToRadian(fAngle3));

			//This moves the stuff around
			D3DXMatrixTranslation(&matTranslate, 0.0f, fUp, fForward);
			
			matWorld = (matYWorld * matXWorld * matZWorld * matTranslate);

			direct3dDevice->SetTransform( D3DTS_WORLD, &matWorld);

			D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
			direct3dDevice->SetTransform( D3DTS_VIEW, &matView );
	direct3dDevice->BeginScene();    // begins the 3D scene

	// do 3D rendering on the back buffer here
	sceneDrawing();

	g_pModel->Draw();


	direct3dDevice->EndScene();    // ends the 3D scene

	direct3dDevice->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}

/*loads a single model
 *
 * Authors: Bryan
 */
#include<string>
void RenderEngine::load () {
	//Moved to CModel.cpp
}

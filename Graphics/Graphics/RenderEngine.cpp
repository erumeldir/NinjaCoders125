/*
 * RenderEngine.cpp
 * Defines the source code for the Render Engine.
 * Authors: Bryan, Nathan
 */
#include "RenderEngine.h"
#include "Models.h"

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

void RenderEngine::renderInitalization()
{
	direct3dInterface = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	D3DPRESENT_PARAMETERS deviceInfo;    // create a struct to hold various device information

	ZeroMemory(&deviceInfo, sizeof(deviceInfo));    // clear out the struct for use
	deviceInfo.Windowed = FALSE;    // program windowed, not fullscreen
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
		// select which vertex format we are using
	direct3dDevice->SetFVF(CUSTOMFVF);

	// select the vertex buffer to display
	direct3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));

	// copy the vertex buffer to the back buffer
	direct3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}

/*we set up and clean up from rendering a scene
 * Bryan
 */
void RenderEngine::render() {
	// clear the window to a deep blue
	direct3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

	direct3dDevice->BeginScene();    // begins the 3D scene

	// do 3D rendering on the back buffer here
	sceneDrawing();

	direct3dDevice->EndScene();    // ends the 3D scene

	direct3dDevice->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}

/*loads a single model
 *
 * Authors: Bryan
 */
#include<string>
void load () {
	//the file
	char * strFileName;
	strcpy(strFileName, "slime");

	//Allocation class. doesn't seem to be used after this...
	CAllocateHierarchy Alloc;

	if(FAILED(D3DXLoadMeshHierarchyFromX(strFileName,		// File load
										D3DXMESH_MANAGED,	// Load Options
										direct3dDevice,		// D3D Device
										&Alloc,				// Hierarchy allocation class
										NULL,				// NO Effects
										&m_pFrameRoot,		// Frame hierarchy
										&m_pAnimController)))// Animation Controller
	{
		MessageBox(NULL, strFileName, "Model Load Error", MB_OK);
	}

}
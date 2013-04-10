
/*
* RenderEngine.cpp
* Defines the source code for the Render Engine.
* Authors: Bryan, Nathan
*/
#include "RenderEngine.h"
//#include "Models.h"

RenderEngine *RenderEngine::re;

#define CUSTOMFVF (D3DFVF_XYZRHW)

struct RENDERVERTEX
{
	FLOAT x, y, z, rhw; // from the D3DFVF_XYZRHW flag
	//rhw is a perspective flag. not sure why it's a float
};



/* create a window that we will render in
*
* Returns nothing
* Bryan
*/
void RenderEngine::startWindow()
{
	static WNDCLASSEX windowClass;

	//Getting some memory to define the class
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	//define the class's elements
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.lpszClassName = "WindowClass";

	//create the class
	RegisterClassEx(&windowClass);

	windowHandle = CreateWindowEx(
		NULL,
		"WindowClass",
		"Our First Direct3D Program",
		WS_EX_TOPMOST | WS_POPUP,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		windowClass.hInstance,
		NULL
	);

	ShowWindow(windowHandle, 1);	//1 is the flag for whether to show the window
}

/*Initialize the 3d Render Environment
* Bryan
*/

void RenderEngine::renderInitalization()
{
	direct3dInterface = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface

	D3DPRESENT_PARAMETERS deviceInfo; // create a struct to hold various device information

	ZeroMemory(&deviceInfo, sizeof(deviceInfo)); // clear out the struct for use
	deviceInfo.Windowed = TRUE; // program windowed, not fullscreen
	deviceInfo.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard old frames
	deviceInfo.hDeviceWindow = windowHandle; // set the window to be used by Direct3D
	deviceInfo.BackBufferFormat = D3DFMT_X8R8G8B8; // set the back buffer format to 32-bit
	deviceInfo.BackBufferWidth = SCREEN_WIDTH; // set the width of the buffer
	deviceInfo.BackBufferHeight = SCREEN_HEIGHT; // set the height of the buffer

	// create a device class using this information and information from the deviceInfo stuct
	direct3dInterface->CreateDevice(D3DADAPTER_DEFAULT,
	D3DDEVTYPE_HAL,
	windowHandle,
	D3DCREATE_SOFTWARE_VERTEXPROCESSING,
	&deviceInfo,
	&direct3dDevice);
}

/*
* Initialize DirectX and any other rendering libraries that we may have.
*/
RenderEngine::RenderEngine() {
	startWindow();
	renderInitalization();	//start initialization

	//now initialize any models we want to have first
	// create three vertices using the RENDERVERTEX struct built earlier
	RENDERVERTEX vertices[] =
	{
		{ 320.0f, 50.0f, 0.5f, 1.0f, },
		{ 520.0f, 400.0f, 0.5f, 1.0f, },
		{ 120.0f, 400.0f, 0.5f, 1.0f, },
	};

	// create the vertex and store the pointer into vertexBuffer, which is created globally
	direct3dDevice->CreateVertexBuffer(3*sizeof(RENDERVERTEX),
	0,
	CUSTOMFVF,
	D3DPOOL_MANAGED,
	&vertexBuffer,
	NULL);

	VOID* vertexInfo; // the void pointer

	vertexBuffer->Lock(0, 0, (void**)&vertexInfo, 0); // lock the vertex buffer
	memcpy(vertexInfo, vertices, sizeof(vertices)); // copy the vertices to the locked buffer
	vertexBuffer->Unlock(); // unlock the vertex buffer
}

/*
* Clean up DrectX and any other rendering libraries that we may have.
*/
RenderEngine::~RenderEngine() {
	direct3dDevice->Release(); // close and release the 3D device
	direct3dInterface->Release(); // close and release Direct3D
}

/*where we actually draw a scene
* Bryan
*/
void RenderEngine::sceneDrawing() {
	// select which vertex format we are using
	direct3dDevice->SetFVF(CUSTOMFVF);

	//while the queue has objects, update
	//TODO: Object queue

	//TODO: while queue has stuff, loop
	//messy code: want to get the model info, then store that.
	RENDERVERTEX vertices[] =
	{
		{ 200.0f, 0.0f, 0.0f, 1.0f, },
		{ 300.0f, 350.0f, 0.0f, 1.0f, },
		{ 0.0f, 350.0f, 0.0f, 1.0f, },
	};

	direct3dDevice->CreateVertexBuffer(3*sizeof(RENDERVERTEX),
	0,
	CUSTOMFVF,
	D3DPOOL_MANAGED,
	&vertexBuffer,
	NULL);

	VOID* vertexInfo; // the void pointer

	vertexBuffer->Lock(0, 0, (void**)&vertexInfo, 0); // lock the vertex buffer
	memcpy(vertexInfo, vertices, sizeof(vertices)); // copy the vertices to the locked buffer
	vertexBuffer->Unlock(); // unlock the vertex buffer

	//end messy code

	// select the vertex buffer to display
	direct3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(RENDERVERTEX));

	// copy the vertex buffer to the back buffer
	direct3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	//end the while loop
}

/*we set up and clean up from rendering a scene
* The RenderEngine should maintain a list of RenderModels,
* which have all the information they need to render a complete world.
* Bryan
*/
void RenderEngine::render() {
	// clear the window to a deep blue
	direct3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(40, 40, 40), 1.0f, 0);

	direct3dDevice->BeginScene(); // begins the 3D scene

	// do 3D rendering on the back buffer here
	sceneDrawing();

	direct3dDevice->EndScene(); // ends the 3D scene

	direct3dDevice->Present(NULL, NULL, NULL, NULL); // displays the created frame
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

#if 0
/*
 * RenderEngine.cpp
 * Defines the source code for the Render Engine.
 */
#include "RenderEngine.h"

RenderEngine *RenderEngine::re;

/*
 * Initialize DirectX and any other rendering libraries that we may have.
 */
RenderEngine::RenderEngine() {
}

/*
 * Clean up DrectX and any other rendering libraries that we may have.
 */
RenderEngine::~RenderEngine() {
}

/*
 * Render the world: The RenderEngine should maintain a list of RenderModels,
 * which have all the information they need to render a complete world.
 */
void RenderEngine::render() {
}

#endif
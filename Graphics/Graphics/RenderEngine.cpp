/*
 * RenderEngine.cpp
 * Defines the source code for the Render Engine.
 */
#include "RenderEngine.h"
#include "Models.h"

RenderEngine *RenderEngine::re;

void RenderEngine::startWindow(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
				   int nCmdShow)
{
    static WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL,
                          L"WindowClass",
                          L"Our First Direct3D Program",
                          WS_EX_TOPMOST | WS_POPUP,
                          0, 0,
                          SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);	
}


/*
 * Initialize DirectX and any other rendering libraries that we may have.
 */

RenderEngine::RenderEngine(HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow) {
	startWindow(hInstance, hPrevInstance, nCmdShow);
	coreInitialization();

	//now initialize any models we want to have first
	 // create three vertices using the CUSTOMVERTEX struct built earlier
    CUSTOMVERTEX vertices[] =
    {
        { 320.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
        { 520.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
        { 120.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
    };

    // create the vertex and store the pointer into v_buffer, which is created globally
    d3ddev->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX),
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &v_buffer,
                               NULL);

    VOID* pVoid;    // the void pointer

    v_buffer->Lock(0, 0, (void**)&pVoid, 0);    // lock the vertex buffer
    memcpy(pVoid, vertices, sizeof(vertices));    // copy the vertices to the locked buffer
    v_buffer->Unlock();    // unlock the vertex buffer

}

/*
 * The parts that are initialized and CAN'T be removed
 */

void RenderEngine::coreInitialization()
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

    D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
    d3dpp.Windowed = FALSE;    // program windowed, not fullscreen
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = SCREEN_WIDTH;    // set the width of the buffer
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;    // set the height of the buffer

    // create a device class using this information and information from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);
}

/*
 * Clean up DrectX and any other rendering libraries that we may have.
 */
RenderEngine::~RenderEngine() {
	d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D
}

/*
 * Render the world: The RenderEngine should maintain a list of RenderModels,
 * which have all the information they need to render a complete world.
 */
void RenderEngine::render() {
	// clear the window to a deep blue
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

    d3ddev->BeginScene();    // begins the 3D scene

    // do 3D rendering on the back buffer here
	        // select which vertex format we are using
        d3ddev->SetFVF(CUSTOMFVF);

        // select the vertex buffer to display
        d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

        // copy the vertex buffer to the back buffer
        d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);



    d3ddev->EndScene();    // ends the 3D scene

    d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}


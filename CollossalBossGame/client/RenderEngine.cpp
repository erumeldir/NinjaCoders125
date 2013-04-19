
/*
* RenderEngine.cpp
* Defines the source code for the Render Engine.
* Authors: Bryan, Nathan
*/
#include "RenderEngine.h"
//#include "Models.h"
#include "defs.h"
#include "RenderModel.h"
#include "ClientObjectManager.h"
#include "ConfigurationManager.h"
#include <mmsystem.h>

#if defined(DEBUG) | defined(_DEBUG)
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)
#endif

//Static Members
RenderEngine *RenderEngine::re;
IXAnimator* RenderEngine::xAnimator;

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
		CW_USEDEFAULT, CW_USEDEFAULT, //0, 0,
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
	deviceInfo.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard old frames	//D3DSWAPEFFECT_COPY
	deviceInfo.hDeviceWindow = windowHandle; // set the window to be used by Direct3D
	deviceInfo.BackBufferFormat = D3DFMT_A8R8G8B8;//D3DFMT_X8R8G8B8; // set the back buffer format to 32-bit
	deviceInfo.BackBufferWidth = SCREEN_WIDTH; // set the width of the buffer
	deviceInfo.BackBufferHeight = SCREEN_HEIGHT; // set the height of the buffer
	
	deviceInfo.BackBufferCount = 1;
	deviceInfo.MultiSampleType = D3DMULTISAMPLE_NONE;
	deviceInfo.MultiSampleQuality = 0;
	deviceInfo.hDeviceWindow = windowHandle;
	deviceInfo.EnableAutoDepthStencil = TRUE;
	deviceInfo.AutoDepthStencilFormat = D3DFMT_D24S8;
	deviceInfo.Flags = 0;
	deviceInfo.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	deviceInfo.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	// create a device class using this information and information from the deviceInfo stuct
	direct3dInterface->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		windowHandle,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&deviceInfo,
		&direct3dDevice);

	
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,&D3DXVECTOR3(0,1,-4),&D3DXVECTOR3(0,1,0), &D3DXVECTOR3(0,1,0) );
	direct3dDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIX matProj;
	//TODO: determine clipping
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 800.0f/600.0f, 1.0f, 300.0f );
	direct3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	
	direct3dDevice->SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );	//Enable depth buffering
	direct3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(80,80,80) );
	direct3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	// Fill in a light structure defining our light
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	light.Diffuse.a  = 1.0f;
	light.Range      = 500.0f;
	// Create a direction for our light - it must be normalized  
	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3(0.0f,-0.3f,0.5);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );

	// Tell the device about the light and turn it on
	direct3dDevice->SetLight( 0, &light );
	direct3dDevice->LightEnable( 0, TRUE ); 

	direct3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	
}

/**
 * Initializes a HUD, which right now is just text, so we're 
 * using DirectX Fonts, tutorial/explanation from here:
 * http://www.drunkenhyena.com/cgi-bin/view_cpp_article.pl?chapter=3;article=17
 * Author(s): Suman, Haro
 */
void RenderEngine::HUDInitialization() {
	D3DXCreateFont(	this->direct3dDevice,     //D3D Device
				    22,                       //Font height
					0,					      //Font width
					FW_NORMAL,                //Font Weight
					1,                        //MipLevels
					false,                    //Italic
					DEFAULT_CHARSET,          //CharSet
					OUT_DEFAULT_PRECIS,       //OutputPrecision
					ANTIALIASED_QUALITY,      //Quality
					DEFAULT_PITCH|FF_DONTCARE,//PitchAndFamily
					"Georgia",                //pFacename,
					&this->direct3dText);     //ppFont
	D3DXCreateLine(this->direct3dDevice, &healthLine);
	D3DXCreateLine(this->direct3dDevice, &backgroundLine);

}

/*
 * Initialize DirectX and any other rendering libraries that we may have.
 */
RenderEngine::RenderEngine() {
	// Set configuration options
	cameraDist = CM::get()->find_config_as_float("CAM_DIST");
	hudTopX = CM::get()->find_config_as_int("HUD_TOP_X");
	hudTopY = CM::get()->find_config_as_int("HUD_TOP_Y");

	startWindow();
	renderInitalization();	//start initialization
	HUDInitialization();
	xAnimator=CreateXAnimator(direct3dDevice);	//get our animator

	D3DXMatrixIdentity(&world);

	cam = new Camera(cameraDist);
	hudText = "DEFAULT";
}


void RenderEngine::updateCamera(const Point_t &pos, const Rot_t &rot)
{
	cam->setTargetPosAndRot(pos, rot);
	// Update the camera view matrix
	cam->viewTarget();
	// Tell D3D to set the view matrix
	direct3dDevice->SetTransform(D3DTS_VIEW, cam->getViewMatrix());
}

/*
* Clean up DrectX and any other rendering libraries that we may have.
* Bryan
*/
RenderEngine::~RenderEngine() {
	direct3dDevice->Release(); // close and release the 3D device
	direct3dInterface->Release(); // close and release Direct3D
	direct3dText->Release(); // close and release the Text
	healthLine->Release();
	backgroundLine->Release();
	delete cam;
}

void RenderEngine::drawHUD() {
	RECT font_rect;

   //A pre-formatted string showing the current frames per second
	SetRect(&font_rect,
			hudTopX,
			hudTopY,
			SCREEN_WIDTH,
			SCREEN_HEIGHT);

    this->direct3dText->DrawText(NULL,        //pSprite
								hudText.c_str(),	 //pString
                                -1,          //Count
                                &font_rect,  //pRect
                                DT_LEFT|DT_NOCLIP,//Format,
                                0xFF000000); //Color

	D3DXVECTOR2 blines[] = {D3DXVECTOR2(10.0f, 40.0f), D3DXVECTOR2(110.0f, 40.0f)};
	backgroundLine->SetWidth(15.0f);
	backgroundLine->Draw(blines, 2, D3DCOLOR_ARGB(255, 0, 0, 0));

	D3DXVECTOR2 hlines[] = {D3DXVECTOR2(10.0f, 40.0f), D3DXVECTOR2(this->healthPts + 10.f , 40.0f)};
	healthLine->SetWidth(15.0f);
	healthLine->Draw(hlines, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
}

/*where we actually draw a scene
* Bryan
*/
void RenderEngine::sceneDrawing() {
	for(list<ClientObject *>::iterator it = lsObjs.begin();
			it != lsObjs.end();
			++it) {
		(*it)->getRenderModel()->render();
	}
	lsObjs.clear();
}

void RenderEngine::renderThis(ClientObject *obj) {
	lsObjs.push_back(obj);
}

/*we set up and clean up from rendering a scene
* The RenderEngine should maintain a list of RenderModels,
* which have all the information they need to render a complete world.
* Bryan
*/
void RenderEngine::render() {
	// clear the window to a deep blue
	direct3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(240, 240, 240), 1.0f, 0);

	direct3dDevice->BeginScene(); // begins the 3D scene

	// do 3D rendering on the back buffer here
	sceneDrawing();
	drawHUD();

	direct3dDevice->EndScene(); // ends the 3D scene

	direct3dDevice->Present(NULL, NULL, NULL, NULL); // displays the created frame
}

// todo take time
#define TIME_SINCE_LAST_UPDATE 33 // 4
void RenderEngine::animate(int id, const D3DXMATRIX &pos) {

	//RenderEngine::direct3dDevice->SetTransform(D3DTS_VIEW, &pos);
	RenderEngine::xAnimator->Render(id,pos,TIME_SINCE_LAST_UPDATE);
}

bool RenderEngine::loadModel(const char * filename, int * idAddr) { 
	return RenderEngine::xAnimator->LoadXFile(filename,idAddr);
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	
		case WM_KEYUP:
			printf("Key up\n");
			break;
		case WM_DESTROY:
		{
			printf("Window destroyed\n");
			PostQuitMessage(0);
			return 0;
		} break;
		default:
			printf("Unknown message 0x%x\n", message);
			break;
	}

	return DefWindowProc (windowHandle, message, wParam, lParam);
}


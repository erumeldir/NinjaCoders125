
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
#include "PlayerCObj.h"
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
	deviceInfo.BackBufferFormat = D3DFMT_UNKNOWN;//D3DFMT_A8R8G8B8;//D3DFMT_X8R8G8B8; // set the back buffer format to 32-bit
	deviceInfo.BackBufferWidth = SCREEN_WIDTH; // set the width of the buffer
	deviceInfo.BackBufferHeight = SCREEN_HEIGHT; // set the height of the buffer
	
	deviceInfo.BackBufferCount = 1;//2;
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
//		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		D3DCREATE_MIXED_VERTEXPROCESSING,
//		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&deviceInfo,
		&direct3dDevice);


	D3DXMATRIX matProj;
	//TODO: determine clipping
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 800.0f/600.0f, 1.0f, CM::get()->find_config_as_float("CLIPPING_PLANE") );
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
	vecDir = D3DXVECTOR3(-1.0f,-0.1f,0.5);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );

	// Tell the device about the light and turn it on
	direct3dDevice->SetLight( 0, &light );

	
	// Fill in a light structure defining our light
	D3DLIGHT9 light2;
	ZeroMemory( &light2, sizeof(D3DLIGHT9) );
	light2.Type       = D3DLIGHT_DIRECTIONAL;
	light2.Diffuse.r  = 1.0f;
	light2.Diffuse.g  = 1.0f;
	light2.Diffuse.b  = 1.0f;
	light2.Diffuse.a  = 1.0f;
	light2.Range      = 500.0f;
	// Create a direction for our light - it must be normalized  
	D3DXVECTOR3 vecDir1;
//	vecDir1 = D3DXVECTOR3(0.0f,-0.3f,-0.5);
	vecDir1 = D3DXVECTOR3(1.0f,-0.1f,-0.5);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light2.Direction, &vecDir1 );

	// Tell the device about the light and turn it on
	direct3dDevice->SetLight( 1, &light2 );

	direct3dDevice->LightEnable( 0, TRUE ); 
	direct3dDevice->LightEnable( 1, TRUE ); 

	direct3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );	
}

/**
 * Initializes a HUD, which right now is just text, so we're 
 * using DirectX Fonts, tutorial/explanation from here:
 * http://www.drunkenhyena.com/cgi-bin/view_cpp_article.pl?chapter=3;article=17
 * Author(s): Suman, Haro
 */


/**
 * Initializes the sprites 
 * Author(s): Franklin
 */
void RenderEngine::gamestartdisplayinit() {
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/p1connect.png", &p1connecttxt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/p2connect.png", &p2connecttxt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/p3connect.png", &p3connecttxt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/p4connect.png", &p4connecttxt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/youarep1.png", &youarep1txt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/youarep2.png", &youarep2txt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/youarep3.png", &youarep3txt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/youarep4.png", &youarep4txt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/pressstart.png", &pressstarttxt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/playerready.png", &playerreadytxt);
	//D3DXCreateTextureFromFile(this->direct3dDevice, "res/blackbackground.png", &blackbackgroundtxt);

	//D3DXCreateSprite(this->direct3dDevice,&p1connect);
	//D3DXCreateSprite(this->direct3dDevice,&p2connect);
	//D3DXCreateSprite(this->direct3dDevice,&p3connect);
	//D3DXCreateSprite(this->direct3dDevice,&p4connect);
	//D3DXCreateSprite(this->direct3dDevice,&youarep1);
	//D3DXCreateSprite(this->direct3dDevice,&youarep2);
	//D3DXCreateSprite(this->direct3dDevice,&youarep3);
	//D3DXCreateSprite(this->direct3dDevice,&youarep4);
	//D3DXCreateSprite(this->direct3dDevice,&pressstart);
	//D3DXCreateSprite(this->direct3dDevice,&playerready);
	//D3DXCreateSprite(this->direct3dDevice,&blackbackground);
}

/**
 * draws a sprite 
 * Author(s): Franklin
 */
//void displaytexture(LPD3DXSPRITE * sprite, D3DXVECTOR3 * pos, IDirect3DTexture9 ** texture) {
//	(*sprite)->Begin(D3DXSPRITE_ALPHABLEND); 
//	(*sprite)->Draw(*texture,NULL,NULL,pos,0xFFFFFFFF); 
//	(*sprite)->End();
//}

/**
 * Initializes the sprites 
 * Author(s): Franklin
 */
void RenderEngine::gamestartdisplaylogic() {
	hud->displayStart(gamestarted);
	//if (!gamestarted) {
	//	if (!gamestartedtexturesinitialized) {
	//		gamestartdisplayinit();
	//		gamestartedtexturesinitialized = true;
	//	}
	//	map<uint, ClientObject *> * objlist = COM::get()->getObjects();
	//	int pid = COM::get()->player_id;
	//	int playercount = 0;
	//	int playernumber = 0;
	//	int ready[5]; ready[1] = 0; ready[2] = 0; ready[3] = 0; ready[4] = 0;
	//	map<uint, ClientObject *>::iterator it = objlist->begin();
	//	for(; it != objlist->end(); ++it) {
	//		ClientObject * o = it->second;
	//		string s = typeid(*o).name();
	//		// if it's not a Player object...
	//		if(!s.compare("class PlayerCObj")) {
	//			PlayerCObj * pc = (PlayerCObj *)o;
	//			playercount++;
	//			if (pc->getId() == pid) {
	//				playernumber = playercount;
	//				ready[playercount] = pc->ready;
	//			} else {
	//				ready[playercount] = pc->ready;
	//			}
	//		}
	//	}
	//	if(playercount >= 1) {
	//		D3DXVECTOR3 p1c(0,0,0.25);
	//		D3DXVECTOR3 p1r(0,0,0);
	//		if (ready[1]) { displaytexture(&playerready, &p1r, &playerreadytxt); }
	//		(playernumber == 1) ? displaytexture(&youarep1, &p1c, &youarep1txt) : displaytexture(&p1connect, &p1c, &p1connecttxt);
	//	}
	//	if(playercount >= 2) {
	//		D3DXVECTOR3 p2c(300,0,0.25);
	//		D3DXVECTOR3 p2r(300,0,0);
	//		if (ready[2]) { displaytexture(&playerready, &p2r, &playerreadytxt); }
	//		(playernumber == 2) ? displaytexture(&youarep2, &p2c, &youarep2txt) : displaytexture(&p2connect, &p2c, &p2connecttxt);
	//	}
	//	if(playercount >= 3) {
	//		D3DXVECTOR3 p3c(0,300,0.25);
	//		D3DXVECTOR3 p3r(0,300,0);
	//		if (ready[3]) { displaytexture(&playerready, &p3r, &playerreadytxt); }
	//		(playernumber == 3) ? displaytexture(&youarep3, &p3c, &youarep3txt) : displaytexture(&p3connect, &p3c, &p3connecttxt);
	//	}
	//	if(playercount >= 4) {
	//		D3DXVECTOR3 p4c(300,300,0.25);
	//		D3DXVECTOR3 p4r(300,300,0);
	//		if (ready[4]) { displaytexture(&playerready, &p4r, &playerreadytxt); }
	//		(playernumber == 4) ? displaytexture(&youarep4, &p4c, &youarep4txt) : displaytexture(&p4connect, &p4c, &p4connecttxt);
	//	}
	//	if(!ready[playernumber]) {
	//		D3DXVECTOR3 rdy(300,200,0);
	//		displaytexture(&pressstart, &rdy, &pressstarttxt);
	//	}
	//	D3DXVECTOR3 blk(0,0,0.5);
	//	displaytexture(&blackbackground, &blk, &blackbackgroundtxt);

	//	bool allready = true;
	//	for(int j = 1; j < playercount+1; j++) {
	//		if(ready[j] != 1) {
	//			allready = false;
	//		}
	//	}
	//	if(allready) {
	//		gamestarted = true;
	//	}
	//}
}

/*
 * Initialize DirectX and any other rendering libraries that we may have.
 */
RenderEngine::RenderEngine() {
	// Set configuration options
	cameraDist = CM::get()->find_config_as_float("CAM_DIST");
	debugFlag = CM::get()->find_config_as_bool("RENDER_DEBUG_FLAG");

	startWindow();
	renderInitalization();	//start initialization
	xAnimator=CreateXAnimator(direct3dDevice);	//get our animator

	D3DXMatrixIdentity(&world);

	cam = new Camera(cameraDist);
	hud = new HeadsUpDisplay(direct3dDevice);
	hudText = "DEFAULT";
	monsterHUDText = "DEFAULT";
	this->gamestarted = false;
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
	delete hud;
	delete cam;
}

void RenderEngine::drawHUD() {
	hud->displayText(this->hudText,this->monsterHUDText);
	hud->displayHealthBars(this->healthPts, this->monsterHealthPts);
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
	direct3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(21, 0, 105), 1.0f, 0);

	direct3dDevice->BeginScene(); // begins the 3D scene

	gamestartdisplaylogic();
	hud->displayBackground();

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

bool RenderEngine::loadModel(const char * filename, int * idAddr, const D3DXMATRIX &rootMat) { 
	// Ignore the 0UL, it's some flag thing that's optional (0UL is the default value)
	// I added it here so I can specify the root matrix (for models that need to be rotated)
	// todo optimization (maybe add scaling here too, would that be faster?)
	return RenderEngine::xAnimator->LoadXFile(filename,idAddr, 0UL, &rootMat);
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_DESTROY:
		{
			printf("Window destroyed\n");
			PostQuitMessage(0);
			return 0;
		} break;
		default:
			//printf("Unknown message 0x%x\n", message);
			break;
	}

	return DefWindowProc (windowHandle, message, wParam, lParam);
}


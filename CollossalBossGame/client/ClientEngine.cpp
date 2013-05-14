/*
 * ClientEngine.cpp
 */
#include "ClientNetworkManager.h"
#include "ClientEngine.h"
#include "defs.h"
#include "RenderEngine.h"
#include "ClientObjectManager.h"

//Static members
ClientEngine *ClientEngine::ce;
int ClientEngine::exitStatus;


/*
 * Engine constructor.  Initializes the other engines.
 */
ClientEngine::ClientEngine() {
	initializeSubModules();
}

/*
 * Engine destructor.  Cleans the other engines.
 */
ClientEngine::~ClientEngine() {
	//Clean engines
	cleanSubModules();
}


/*
 * End the game with an optional error message
 */
void ClientEngine::exit(int i) {
	DC::get()->print("Exiting with error code %d\n", i);
	isRunning = false;
	exitStatus = i;
}

void ClientEngine::initializeSubModules() {
	isRunning = true;
	exitStatus = 0;

	//Initialize engines
	DC::init("clientLog.txt");
	CNM::init();
	RE::init();
	COM::init();
	xctrl = new XboxController(1); // For now, we can decide later if we want to change the id

	state.reset();
}

void ClientEngine::cleanSubModules() {
	RE::clean();
	COM::clean();
	CNM::clean();
	DC::clean();

	state.reset();
}

/*
 * The main game loop of the engine.  Initializes and runs the game.
 */
void ClientEngine::run() {

	// Wait until we connect to a server
	while(!ClientNetworkManager::get()->isConnected()) { 
		ClientNetworkManager::get()->update(); 
	}

	while(isRunning) {
		
		//Send event information to the server
		xctrl->sendInput();
		
		// Fetch all Data From the Server 
		while(ClientNetworkManager::get()->update()) {}

		//Update game logic/physics (will be moved to the server)
		COM::get()->update();

		//Render
		RE::get()->render();
		
		//Poll events
		

		//Sleep(10);

		if(CE::get()->state.currentState == GAME_LOADING) {
			char a[4];
			ClientNetworkManager::get()->sendData(CLIENT_READY, a, 4, COM::get()->player_id);
		}
	}
}

void ClientEngine::serverUpdate(char * buf) {
	state.deserialize(buf);
}

void ClientEngine::reset() {
	// Shutdown and reset network manager
	// clean Client Engine
	cleanSubModules();
	// reinit Client Engine
	initializeSubModules();
}
/*
 * ClientEngine.cpp
 */

#include "ClientEngine.h"

//Static members
ClientEngine *ClientEngine::ce;
int ClientEngine::exitStatus;


/*
 * Engine constructor.  Initializes the other engines.
 */
ClientEngine::ClientEngine() {
	isRunning = true;
	exitStatus = 0;
}

void ClientEngine::exit(int i) {
	isRunning = false;
	exitStatus = i;
}

/*
 * Engine destructor.  Cleans the other engines.
 */
ClientEngine::~ClientEngine() {
}

/*
 * The main game loop of the engine.  Initializes and runs the game.
 */
void ClientEngine::run() {
	while(isRunning) {
	}
}

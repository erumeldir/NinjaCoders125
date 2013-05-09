#include "ClientNetworkManager.h"
#include "ClientGameStateManager.h"
#include "ClientEngine.h"

ClientGameStateManager ClientGameStateManager::game;

ClientGameStateManager::ClientGameStateManager(void)
{
	this->init();
}

ClientGameStateManager::~ClientGameStateManager(void) { }

ClientGameStateManager * ClientGameStateManager::get() {
	return &game;
}

void ClientGameStateManager::init() {
	gs.reset();
}

void ClientGameStateManager::serverUpdate(char * buf) {
	gs.deserialize(buf);
}

void ClientGameStateManager::reset() {
	// Shutdown and reset network manager
	// clean Client Engine
	// reinit Client Engine
}
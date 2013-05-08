#include "ClientGameStateManager.h"

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
#include "ClientWorldManager.h"

ClientWorldManager ClientWorldManager::world;

ClientWorldManager::ClientWorldManager(void)
{
	this->init();
}



ClientWorldManager::~ClientWorldManager(void)
{
}


ClientWorldManager * ClientWorldManager::get() {
	return &world;
}

void ClientWorldManager::init() {
	ws.reset();
}

void ClientWorldManager::serverUpdate(char * buf) {
	ws.deserialize(buf);
}
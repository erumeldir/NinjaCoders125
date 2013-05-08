#pragma once
#include "ServerNetworkManager.h"
#include "ServerWorldManager.h"
#include "ServerObjectManager.h"
#include "game.h"

ServerWorldManager ServerWorldManager::world;

ServerWorldManager::~ServerWorldManager() {}

ServerWorldManager::ServerWorldManager() {
	this->init();
	this->register_handlers();
}

void ServerWorldManager::register_handlers() {
	EventManager::get()->registerHandler(EVENT_RESET, this);
	EventManager::get()->registerHandler(EVENT_HARD_RESET, this);
	EventManager::get()->registerHandler(EVENT_MONSTER_SPAWN, this);
	EventManager::get()->registerHandler(EVENT_MONSTER_DEATH, this);
	EventManager::get()->registerHandler(EVENT_PLAYER_DEATH, this);
	EventManager::get()->registerHandler(EVENT_CONNECTION, this);
	EventManager::get()->registerHandler(EVENT_DISCONNECT, this);
}

void ServerWorldManager::init() {
	ws.reset();
}

ServerWorldManager * ServerWorldManager::get() {
	return &world;
}

void ServerWorldManager::event_reset(EventData * data, void * obj) {
	if(ws.currentState == WORLD_BEGIN) {

	}
	if(ws.currentState == WORLD_END) {
		if(ws.playerDeathCount == ws.totalPlayerCount || ws.monsterDeathCount == ws.totalMonsterCount) {
			event_hard_reset(NULL, NULL);
		}
	}
}

void ServerWorldManager::event_hard_reset(EventData * data, void * obj) {
	SecureZeroMemory(&ws, sizeof(WorldState));
	// Fire some function to reset the positions of all server objects.
	// TODO: Hard Reset involving the clearing of the entire thing and reconnecting everyone. 
	// Call any potential reconnect model swapping a feature.
	SOM::get()->reset();
	gameInit();
}

void ServerWorldManager::event_player_death(EventData * data, void * obj) {
	ws.playerDeathCount++;
	assert((ws.playerDeathCount <= ws.totalPlayerCount) && "Implementation Error");
}

void ServerWorldManager::event_monster_death(EventData * data, void * obj) {
	ws.monsterDeathCount++;
	assert((ws.monsterDeathCount <= ws.totalMonsterCount) && "Implementation Error");
}

void ServerWorldManager::event_connection(EventData * data, void * obj) {
	ws.playerconnect(data->value);
}

void ServerWorldManager::event_monster_spawn(EventData * data, void * obj) {
	ws.totalMonsterCount++;
}

void ServerWorldManager::event_disconnect(EventData * data, void * obj) {
	ws.totalPlayerCount--;

	assert((ws.totalPlayerCount < 0) && "Implementation Error");
}

void ServerWorldManager::sendState() {
	char * buf = SNM::get()->getSendBuffer();
	int dlen = ws.serialize(buf);
	SNM::get()->sendToAll(WORLD_MANAGER, -1, dlen);
}

void ServerWorldManager::updateState(char * buf) {
	WorldData wd;
	memcpy(&wd, buf, sizeof(WorldData));
	if(wd.reset) {
		event_reset(NULL, NULL);
	}
	if(wd.hardreset) {
		event_hard_reset(NULL, NULL);
	}
	if(wd.start) {
		ws.playerready(wd.playerid);
	}
	if(wd.clientready) {
		ws.clientready(wd.playerid);
	}
}
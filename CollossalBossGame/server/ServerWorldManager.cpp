#pragma once
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
	EventManager::get()->registerHandler(EVENT_MONSTER_SPAWN, this);
	EventManager::get()->registerHandler(EVENT_MONSTER_DEATH, this);
	EventManager::get()->registerHandler(EVENT_PLAYER_DEATH, this);
	EventManager::get()->registerHandler(EVENT_CONNECTION, this);
	EventManager::get()->registerHandler(EVENT_DISCONNECT, this);
}

void ServerWorldManager::init() {
	totalPlayerCount = 0;
	totalMonsterCount = 0;
	playerDeathCount = 0;
	monsterDeathCount = 0;
	resetCount = 0;
	gameOverFlag = 0;
}

ServerWorldManager * ServerWorldManager::get() {
	return &world;
}

void ServerWorldManager::event_reset(EventData * data, void * obj) {
	if(playerDeathCount == totalPlayerCount || monsterDeathCount == totalMonsterCount) {
		resetCount++;
	}
	if(resetCount == totalPlayerCount) {
		totalMonsterCount = 0;
		playerDeathCount = 0;
		monsterDeathCount = 0;
		resetCount = 0;
		gameOverFlag = 0;
		// Fire some function to reset the positions of all server objects.
		SOM::get()->reset();
		gameInit();
	}
}

void ServerWorldManager::event_player_death(EventData * data, void * obj) {
	playerDeathCount++;
	assert((playerDeathCount <= totalPlayerCount) && "Implementation Error");
}

void ServerWorldManager::event_monster_death(EventData * data, void * obj) {
	monsterDeathCount++;
	assert((monsterDeathCount <= totalMonsterCount) && "Implementation Error");
}

void ServerWorldManager::event_connection(EventData * data, void * obj) {
	totalPlayerCount++;
}

void ServerWorldManager::event_monster_spawn(EventData * data, void * obj) {
	totalMonsterCount++;
}

void ServerWorldManager::event_disconnect(EventData * data, void * obj) {
	totalPlayerCount--;
	assert((totalPlayerCount < 0) && "Implementation Error");
}


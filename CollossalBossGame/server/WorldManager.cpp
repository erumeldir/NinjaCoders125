#pragma once
#include "WorldManager.h"
#include "ServerObjectManager.h"
#include "EventManager.h"
#include "game.h"

void manageEvent(EventTypes evt, void * obj) {
	ServerObject * sobj = (ServerObject *)(obj);
	switch(evt) {
		case EVENT_PLAYER_DEATH:
			WorldManager::get()->event_player_death(sobj);
			break;
		case EVENT_MONSTER_DEATH:
			WorldManager::get()->event_monster_death(sobj);
			break;
		case EVENT_RESET:
			WorldManager::get()->event_reset(sobj);
			break;
		case EVENT_CONNECTION:
			WorldManager::get()->event_connection(sobj);
			break;
		case EVENT_MONSTER_SPAWN:
			WorldManager::get()->event_monster_spawn(sobj);
			break;
		case EVENT_DISCONNECT:
			WorldManager::get()->event_connection(sobj);
			break;
		default:
			// Print Debug Statement - Event not handled by this object.
			break;
	}
}

WorldManager WorldManager::world;

WorldManager::~WorldManager() {}

WorldManager::WorldManager() {
	this->init();
	EventManager::get()->registerHandler(&manageEvent);
}

void WorldManager::init() {
	totalPlayerCount = 0;
	totalMonsterCount = 0;
	playerDeathCount = 0;
	monsterDeathCount = 0;
	resetCount = 0;
	gameOverFlag = 0;
}

WorldManager * WorldManager::get() {
	return &world;
}

void WorldManager::event_reset(ServerObject * obj) {
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

void WorldManager::event_player_death(ServerObject * obj) {
	playerDeathCount++;
	assert((playerDeathCount <= totalPlayerCount) && "Implementation Error");
}

void WorldManager::event_monster_death(ServerObject * obj) {
	monsterDeathCount++;
	assert((monsterDeathCount <= totalMonsterCount) && "Implementation Error");
}

void WorldManager::event_connection(ServerObject * obj) {
	totalPlayerCount++;
}

void WorldManager::event_monster_spawn(ServerObject * obj) {
	totalMonsterCount++;
}

void WorldManager::event_disconnect(ServerObject * obj) {
	totalPlayerCount--;
	assert((totalPlayerCount < 0) && "Implementation Error");
}


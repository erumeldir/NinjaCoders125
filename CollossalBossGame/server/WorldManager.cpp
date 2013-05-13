#pragma once
#include "WorldManager.h"
#include "ServerObjectManager.h"
#include "game.h"

WorldManager WorldManager::world;

WorldManager::~WorldManager() {}

WorldManager::WorldManager() {
	this->init();
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

void WorldManager::event_reset(int playerid) {
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

void WorldManager::event_player_death(int playerid) {
	playerDeathCount++;
	assert((playerDeathCount <= totalPlayerCount) && "Implementation Error");
}

void WorldManager::event_monster_death() {
	monsterDeathCount++;
	assert((monsterDeathCount <= totalMonsterCount) && "Implementation Error");
}

void WorldManager::event_connection(int playerid) {
	totalPlayerCount++;
}

void WorldManager::event_monster_spawn() {
	totalMonsterCount++;
}

void WorldManager::event_disconnect(int playerid) {
	totalPlayerCount--;
	assert((totalPlayerCount < 0) && "Implementation Error");
}

void WorldManager::event_hard_reset(int playerid) {
	this->event_reset(playerid);
}

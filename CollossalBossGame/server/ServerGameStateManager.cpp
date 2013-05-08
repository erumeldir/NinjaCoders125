#pragma once
#include "ServerNetworkManager.h"
#include "ServerObjectManager.h"
#include "ServerGameStateManager.h"
#include "game.h"

ServerGameStateManager ServerGameStateManager::game;

ServerGameStateManager::~ServerGameStateManager() {}

ServerGameStateManager::ServerGameStateManager() {
	this->init();
}

void ServerGameStateManager::init() {
	gs.reset();
}

ServerGameStateManager * ServerGameStateManager::get() {
	return &game;
}

void ServerGameStateManager::event_reset(int playerid) {
	if(gs.currentState == GAME_BEGIN) {

	}
	if(gs.currentState == GAME_END) {
		if(gs.playerDeathCount == gs.totalPlayerCount || gs.monsterDeathCount == gs.totalMonsterCount) {
			event_hard_reset(playerid);
		}
	}
}

void ServerGameStateManager::event_hard_reset(int playerid) {
	SecureZeroMemory(&gs, sizeof(WorldState));
	// Fire some function to reset the positions of all server objects.
	// TODO: Hard Reset involving the clearing of the entire thing and reconnecting everyone. 
	// Call any potential reconnect model swapping a feature.
	SOM::get()->reset();
	gameInit();
}

void ServerGameStateManager::event_player_death(int playerid) {
	gs.playerDeathCount++;
	assert((gs.playerDeathCount <= gs.totalPlayerCount) && "Implementation Error");
}

void ServerGameStateManager::event_monster_death() {
	gs.monsterDeathCount++;
	assert((gs.monsterDeathCount <= gs.totalMonsterCount) && "Implementation Error");
}

void ServerGameStateManager::event_connection(int playerid) {
	gs.playerconnect(playerid);
}

void ServerGameStateManager::event_monster_spawn() {
	gs.totalMonsterCount++;
}

void ServerGameStateManager::event_disconnect(int playerid) {
	gs.totalPlayerCount--;

	assert((gs.totalPlayerCount < 0) && "Implementation Error");
}

void ServerGameStateManager::sendState() {
	char * buf = SNM::get()->getSendBuffer();
	int dlen = gs.serialize(buf);
	SNM::get()->sendToAll(GAMESTATE_MANAGER, -1, dlen);
}

void ServerGameStateManager::updateState(char * buf) {
	GameData gd;
	memcpy(&gd, buf, sizeof(GameData));
	if(gd.reset) {
		event_reset(gd.playerid);
	}
	if(gd.hardreset) {
		event_hard_reset(gd.playerid);
	}
	if(gd.start) {
		gs.playerready(gd.playerid);
	}
	if(gd.clientready) {
		gs.clientready(gd.playerid);
	}
}
#pragma once
#include "ServerNetworkManager.h"
#include "ServerGameStateManager.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "PhysicsEngine.h"
#include "game.h"
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <time.h> 
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
	if(gs.currentState == GAME_START) {

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
	SNM::get()->getSendBuffer();
	SNM::get()->sendToAll(RESET, 0);
	this->teardownGame();
	this->gs.reset();
	this->initializeGame();
}

void ServerGameStateManager::event_player_death(int playerid) {
	gs.playerDeathCount++;
	assert((gs.playerDeathCount <= gs.totalPlayerCount) && "Implementation Error");
}

void ServerGameStateManager::event_monster_death() {
	gs.monsterDeathCount++;
	if(gs.monsterDeathCount == gs.totalMonsterCount) {
		gs.currentState = GAME_END;
	}
	assert((gs.monsterDeathCount <= gs.totalMonsterCount) && "Implementation Error");
}

void ServerGameStateManager::event_connection(int playerid) {
	gs.playerconnect(playerid);
	if(gs.totalPlayerCount == 1) {
		gs.currentState = GAME_SCENE_SELECT;
	}
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

	if(gd.hardreset) {
		event_hard_reset(gd.playerid);
	}
	switch(gs.currentState) {
		case GAME_CONNECTING:
			break;
		case GAME_SCENE_SELECT:
			if(gd.left && (gs.getplayerlocation(gd.playerid) == 0)) { gs.sceneselection--; }
			if(gd.right && (gs.getplayerlocation(gd.playerid) == 0)) { gs.sceneselection++; }
 			if(gd.start) { 
				gs.currentState = GAME_LOADING; 
				// Initialize the world depending on the current scene selection.
				gameInit();
			}
			break;
		case GAME_LOADING:
			if(gd.clientready) { gs.clientready(gd.playerid); }
			break;
		case GAME_CLASS_SELECT:
			if(gd.left || gd.right) {
				gs.classselect(gd.playerid, gd.right, gd.left);
			}
			if(gd.right) { gs.sceneselection++; }
			break;
		case GAME_START:
			if(gd.start) { gs.playerready(gd.playerid); }
			break;
		case GAME_RUNNING:
			break;
		case GAME_END:
			if(gd.start) { event_reset(gd.playerid); }
			break;
		default:
			cout << "Odd State" << endl;


	}

	if(gs.currentState == GAME_SCENE_SELECT || gs.currentState == GAME_CLASS_SELECT) {

	}

}



////////////////////////////// GAME THINGS STOLEN FROM GAMESERVER.H ///////////////////////////////
#define TICK 1000/30 // 30 times per second (in ms)

void ServerGameStateManager::gameStart() {
	initializeGame();

	time_t beginLoopTimer;
	time_t endLoopTimer;
	double totalLoopTime;

	while(true) {
		// Get timestamp
		time(&beginLoopTimer);

		gameLoop();
		
		// Wait until next clock tick
		time(&endLoopTimer);
		totalLoopTime = difftime(endLoopTimer, beginLoopTimer)  * 1000; // in ms
		// Be sure to set debug to the right thing!
		if (totalLoopTime < TICK) {
			Sleep((DWORD)(TICK - totalLoopTime));
		}
		else
		{
			int tick = TICK;
			DC::get()->print("WARNING!!! total loop time %f is greater than tick time: %d\n...NOTE: this might mean a client is connecting\n", totalLoopTime, tick);
		}
	}
}

void ServerGameStateManager::gameLoop() {
	switch(gs.currentState) {
		case GAME_CONNECTING:
		case GAME_SCENE_SELECT:
			SNM::get()->update();
			this->sendState();
			SNM::get()->getSendBuffer();
			SNM::get()->sendToAll(COMPLETE, 0);
			break;
		case GAME_LOADING:
		case GAME_CLASS_SELECT:
		case GAME_START:
		case GAME_RUNNING:
		case GAME_END:
			SNM::get()->update();
			SOM::get()->update();
			SOM::get()->sendState();
			this->sendState();
			SNM::get()->getSendBuffer();
			SNM::get()->sendToAll(COMPLETE, 0);
			break;
		default:
			cout << "Odd State" << endl;
	}





}

void ServerGameStateManager::initializeGame() {
	DC::init("serverLog.txt");
	SOM::init();
	PE::init();
}

void ServerGameStateManager::teardownGame() {
	SOM::clean();
	PE::clean();
	DC::clean();
}
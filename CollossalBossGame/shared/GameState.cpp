#include "GameState.h"
#include <windows.h>
#include "assert.h"

GameState::GameState(void)
{
	this->reset();
}


GameState::~GameState(void) { }

void GameState::reset()
{
	SecureZeroMemory(this, sizeof(GameState));
}

int GameState::serialize(char * buf) {
	memcpy(buf, this, sizeof(GameState));
	return sizeof(GameState);
}

void GameState::deserialize(char * buf) {
	memcpy(this, buf, sizeof(GameState));
}

void GameState::playerconnect(int playerid) {
	for(int i = 0; i < 4; i++) {
		if(playersconnected[i] == playerid) {
			return;
		}
	}
	playersconnected[totalPlayerCount] = playerid;
	totalPlayerCount++;
}

void GameState::playerready(int playerid) {
	if(this->currentState != GAME_BEGIN) {
		// Ignore it if it's not the right state.
		return;
	}
	bool allready = true;
	for(int i = 0; i < 4; i++) {
		if(playersconnected[i] == playerid) {
			playersready[i] = true;
		}
		if(playersready[i] == false) {
			allready = false;
		}
	}
	if(allready) {
		this->currentState = GAME_RUNNING;
	}
}

void GameState::clientready(int playerid) {
	if(this->currentState != GAME_LOADING) {
		// Dunno where this is coming from. IGNORE
		return;
	}
	bool allready = true;
	for(int i = 0; i < 4; i++) {
		if(playersconnected[i] == playerid) {
			playersready[i] = true;
		}
		if(playersready[i] == false) {
			allready = false;
		}
	}
	if(allready) {
		this->currentState = GAME_BEGIN;
	}
}
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
	if(this->currentState != GAME_START) {
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
		this->currentState = GAME_START;
	}
}

int GameState::getplayerlocation(int playerid) {
	for(int i = 0; i < 4; i++) {
		if(playersconnected[i] == playerid) {
			return i;
		}
	}
}

void GameState::classselect(int playerid, bool inc, bool dec) {
	bool done = false;
	int i = (inc) ? 1 : -1;
	while(!done) {
		int playerloc = getplayerlocation(playerid);
		int searchvalue = (classselection[playerloc]+i)%5;
		bool acceptablevalue = true;
		for(int i = 0; i < 4; i++) {
			if(classselection[i] == searchvalue) {
				acceptablevalue = false;
			}
		}
		if(acceptablevalue) {
			classselection[playerloc] = (classselection[playerloc]+i)%5;
		} else {
			(inc) ? i++ : i--;
		}
	}
}
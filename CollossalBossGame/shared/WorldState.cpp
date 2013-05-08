#include "WorldState.h"
#include <windows.h>
#include "assert.h"

WorldState::WorldState(void)
{
	this->reset();
}


WorldState::~WorldState(void)
{

}

void WorldState::reset()
{
	SecureZeroMemory(this, sizeof(WorldState));
}

int WorldState::serialize(char * buf) {
	memcpy(buf, this, sizeof(WorldState));
	return sizeof(WorldState);
}

void WorldState::deserialize(char * buf) {
	memcpy(this, buf, sizeof(WorldState));
}

void WorldState::playerconnect(int playerid) {
	for(int i = 0; i < 4; i++) {
		if(playersconnected[i] == playerid) {
			return;
		}
	}
	playersconnected[totalPlayerCount] = playerid;
	totalPlayerCount++;
}

void WorldState::playerready(int playerid) {
	if(this->currentState != WORLD_BEGIN) {
		// Ignore it if it's not the right state.
		return;
	}
	bool allready = true;
	for(int i = 0; i < 4; i++) {
		if(playersconnected[i] == playerid) {
			playersready[i] == true;
		}
		if(playersready[i] == false) {
			allready = false;
		}
	}
	if(allready) {
		this->currentState = WORLD_RUNNING;
	}
}

void WorldState::clientready(int playerid) {
	if(this->currentState != WORLD_LOADING) {
		// Dunno where this is coming from. IGNORE
		return;
	}
	bool allready = true;
	for(int i = 0; i < 4; i++) {
		if(playersconnected[i] == playerid) {
			playersready[i] == true;
		}
		if(playersready[i] == false) {
			allready = false;
		}
	}
	if(allready) {
		this->currentState = WORLD_BEGIN;
	}
}
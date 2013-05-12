#pragma once
#include "defs.h"
#include <assert.h>
#include "NetworkData.h"
#include "ServerObject.h"

class WorldManager
{
public:
	WorldManager();
	~WorldManager();
	void init();
	static WorldManager * get();

	void event_player_death(int playerid);
	void event_monster_death();
	void event_reset(int playerid);
	void event_hard_reset(int playerid);
	void event_connection(int playerid);
	void event_monster_spawn();
	void event_disconnect(int playerid);

private:
	static WorldManager world; // Statics ftw.

	// World Values
	int totalPlayerCount;	// Useful information
	int totalMonsterCount;	// Useful information
	int playerDeathCount;	// Reset Logic 1
	int monsterDeathCount;	// Reset Logic 2
	int resetCount; // Reset Logic 3
	int gameOverFlag; // Reset Logic 4
	
};

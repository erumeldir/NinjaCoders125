#pragma once
#include "defs.h"
#include <assert.h>
#include "NetworkData.h"
#include "ServerObject.h"
#include "EventManager.h"

class WorldManager
{
public:
	WorldManager();
	~WorldManager();
	void init();
	static WorldManager * get();

	// static void manageEvent(WorldManager * wm, EventTypes evt, ServerObject * obj);

	void event_player_death(ServerObject * obj);
	void event_monster_death(ServerObject * obj);
	void event_reset(ServerObject * obj);
	void event_connection(ServerObject * obj);
	void event_monster_spawn(ServerObject * obj);
	void event_disconnect(ServerObject * obj);

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

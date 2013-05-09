#pragma once
#include "defs.h"
#include <assert.h>
#include "NetworkData.h"
#include "GameState.h"

class ServerGameStateManager
{
public:
	ServerGameStateManager();
	~ServerGameStateManager(void);
	void init();
	static ServerGameStateManager * get();

	void event_player_death(int playerid);
	void event_monster_death();
	void event_reset(int playerid);
	void event_hard_reset(int playerid);
	void event_connection(int playerid);
	void event_monster_spawn();
	void event_disconnect(int playerid);

	void sendState();
	void updateState(char * buf);

	void gameStart();
	void gameLoop();
	void initializeGame();
	void teardownGame();

	GameState gs;
private:
	static ServerGameStateManager game; // Statics ftw.

};
typedef ServerGameStateManager SGSM;
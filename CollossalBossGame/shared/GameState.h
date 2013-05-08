#pragma once

#include <vector>
#include <string>

using namespace std;

class GameState
{
public:
	GameState(void);
	~GameState(void);

	void reset();
	int serialize(char * buf);
	void deserialize(char * buf);

	void playerconnect(int playerid);
	void playerready(int playerid);
	void clientready(int playerid);

	// World States
	int currentState;

	// World Statistics
	int totalMonsterCount;
	int totalPlayerCount;
	int playerDeathCount;
	int monsterDeathCount;

	int playersconnected[4];
	bool clientsready[4];
	bool playersready[4];
};

enum GameStates {
	GAME_LOADING = 0,
	GAME_BEGIN,
	GAME_RUNNING,
	GAME_END
};
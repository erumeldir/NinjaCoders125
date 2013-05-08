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

	int getplayerlocation(int playerid);
	void playerconnect(int playerid);
	void playerready(int playerid);
	void clientready(int playerid);

	void classselect(int playerid, bool inc, bool dec);

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

	int classselection[4];
	int sceneselection;
};

enum GameStates {
	GAME_CONNECTING = 0,
	GAME_SCENE_SELECT,
	GAME_LOADING,
	GAME_CLASS_SELECT,
	GAME_START,
	GAME_RUNNING,
	GAME_END
};
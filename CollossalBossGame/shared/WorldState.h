#pragma once

#include <vector>
#include <string>

using namespace std;

class WorldState
{
public:
	WorldState(void);
	~WorldState(void);

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

enum WorldStates {
	WORLD_LOADING = 0,
	WORLD_BEGIN,
	WORLD_RUNNING,
	WORLD_END
};
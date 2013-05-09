#pragma once

#include "GameState.h"
#include "ClientGameStateManager.h"

class ClientGameStateManager
{
public:
	ClientGameStateManager(void);
	~ClientGameStateManager(void);

	void serverUpdate(char * buf);
	void init();
	void reset();
	static ClientGameStateManager * get();

	GameState gs;
private:
	static ClientGameStateManager game; // Statics ftw.
};
typedef ClientGameStateManager CGSM;
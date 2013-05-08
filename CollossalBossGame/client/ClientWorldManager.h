#pragma once

#include "WorldState.h"
#include "ClientWorldManager.h"

class ClientWorldManager
{
public:
	ClientWorldManager(void);
	~ClientWorldManager(void);

	void serverUpdate(char * buf);
	void init();
	static ClientWorldManager * get();

	WorldState ws;
private:
	static ClientWorldManager world; // Statics ftw.
};
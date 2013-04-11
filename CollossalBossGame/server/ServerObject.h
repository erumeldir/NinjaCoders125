#pragma once
#include "defs.h"
#include "PhysicsModel.h"

class ServerObject
{
public:
	ServerObject(uint id);
	virtual ~ServerObject(void);

	//To create a server object, you must implement these methods
	virtual bool update() = 0;
	virtual PhysicsModel *getPhysicsModel() = 0;
	virtual pair<int, char*> serialize() = 0;

	// Can be overriden, but should be only used by objects that receive
	// input from the client (a.k.a. playerobjects)
	virtual void deserialize(char* newState);

	uint getId() { return id; }

private:
	uint id;
};


#pragma once
#include "defs.h"
#include "PhysicsModel.h"
#include <assert.h>

class ServerObject
{
public:
	ServerObject(uint id);
	virtual ~ServerObject(void);

	//To create a server object, you must implement these methods
	virtual bool update() = 0;
	virtual PhysicsModel *getPhysicsModel() = 0;
	virtual int serialize(char * buf) = 0;

	// Can be overriden, but should be only used by objects that receive
	// input from the client (a.k.a. playerobjects)
	virtual void deserialize(char* newState) {
		// TODO: add to error console
		// DC::get()->print("ERROR! Trying to deserialize input for a server object that doesn't take input from the client\n");
		assert(false && "ERROR! Trying to deserialize input for a server object that doesn't take input from the client\n");
	}

	uint getId() { return id; }

private:
	uint id;
};


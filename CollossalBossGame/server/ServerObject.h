#pragma once
#include "defs.h"

class ServerObject
{
public:
	ServerObject(uint id);
	virtual ~ServerObject(void);

	//To create a server object, you must implement these methods
	virtual bool update() = 0;

	uint getId() { return id; }

private:
	uint id;
};


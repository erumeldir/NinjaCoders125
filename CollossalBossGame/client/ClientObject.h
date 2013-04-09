#pragma once

#include "defs.h"

class ClientObject
{
public:
	ClientObject(uint id);
	virtual ~ClientObject(void);

	//To create a client object, you must implement this method
	virtual bool update() = 0;


	uint getId() { return id; }

private:
	uint id;
};


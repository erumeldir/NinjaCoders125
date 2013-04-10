#pragma once

#include "defs.h"
#include "RenderModel.h"

class ClientObject
{
public:
	ClientObject(uint id);
	virtual ~ClientObject(void);

	//To create a client object, you must implement these methods
	virtual bool update() = 0;
	virtual RenderModel* getRenderModel() = 0;


	uint getId() { return id; }

private:
	uint id;
};


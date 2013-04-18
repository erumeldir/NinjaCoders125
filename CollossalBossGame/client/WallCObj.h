#pragma once
#include "ClientObject.h"
#include "Frame.h"

class WallCObj : public ClientObject
{
public:
	WallCObj(uint id, char *serializedData);
	
	virtual ~WallCObj(void);
	
	virtual bool update();

	virtual RenderModel* getRenderModel() { return rm; }

	virtual void deserialize(char* newState);

private:
	RenderModel *rm;
};


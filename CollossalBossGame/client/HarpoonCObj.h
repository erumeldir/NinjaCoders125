#pragma once

#include "ClientObject.h"
#include "Action.h"
#include "ShootingEffect.h"

class HarpoonCObj : public ClientObject
{
public:
	HarpoonCObj(uint id, char *serializedData);
	virtual ~HarpoonCObj(void);

	virtual bool update();

	virtual RenderModel* getRenderModel() { return rm; }

	virtual void deserialize(char* newState);

private:
	RenderModel *rm;
	ShootingEffect* pewPew;
	int diameter;
};


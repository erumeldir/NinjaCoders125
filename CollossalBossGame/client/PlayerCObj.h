#pragma once
#include "RenderModel.h"
#include "ChargeEffect.h"
#include "ClientObject.h"

class PlayerCObj : public ClientObject
{
public:
	PlayerCObj(uint id, char *data);
	virtual ~PlayerCObj(void);

	virtual void showStatus();

	virtual bool update();

	virtual RenderModel* getRenderModel() { return rm; }

	virtual void deserialize(char* newState);

	int ready;

private:
	RenderModel * box;
	int health;
	float charge;
	RenderModel *rm;
	float cameraPitch;
	Quat_t camRot;
	ChargeEffect* chargingEffect;
};


#pragma once

#include "PlayerSObj.h"

class MechanicSObj : public PlayerSObj
{
public:
	MechanicSObj(uint id, uint clientId);
	virtual ~MechanicSObj(void);

	virtual void initialize();
protected:
	virtual void releaseCharge();
	virtual void actionAttack();
};
#pragma once
#include "ServerObject.h"
#include "Action.h"

class PlayerSObj : public ServerObject
{
public:
	PlayerSObj(uint id);
	virtual ~PlayerSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	void deserialize(char* newInput);
	virtual ObjectType getType() { return OBJ_PLAYER; }
	virtual void onCollision(ServerObject *obj);
	inputstatus getInput() { return istat; }
	int getHealth() { return health; } 
	char serialbuffer[100];

	// todo cleanup!! maybe make counter class/struct
	bool attacking, newAttack;
	uint jumpCounter, attackCounter;
	int health;


private:
	PhysicsModel *pm;
	inputstatus istat;
	Point_t lastCollision;
	bool jumping, newJump, appliedJumpForce;
	// Configuration options
	float jumpDist;
	int movDamp;

};


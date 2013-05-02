#pragma once
#include "ServerObject.h"
#include "ServerWorldManager.h"
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
	void initialize();
	virtual void onCollision(ServerObject *obj, const Vec3f &collNorm);
	int getHealth() { return health; } 
	void setAnimationState(int state) { modelAnimationState = state; }
	char serialbuffer[100];

	bool attacking, newAttack;
	uint jumpCounter, attackCounter;
	int health;
	bool ready;

private:
	PhysicsModel *pm;
	inputstatus istat;
	Point_t lastCollision;
	bool jumping, newJump, appliedJumpForce;
	// Configuration options
	float jumpDist;
	int movDamp;
	bool firedeath;
	int gravityTimer;
	int modelAnimationState;
};


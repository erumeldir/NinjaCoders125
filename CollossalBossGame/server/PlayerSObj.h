#pragma once
#include "ServerObject.h"
#include "WorldManager.h"
#include "Action.h"


class PlayerSObj : public ServerObject
{
public:
	PlayerSObj(uint id, uint clientId);
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
	int damage;
	bool ready;

private:
	uint clientId;
	PhysicsModel *pm;
	inputstatus istat;
	Point_t lastCollision;
	bool jumping, newJump, appliedJumpForce;
	bool charging, newCharge;
	float charge;
	// Configuration options
	float jumpDist;
	float chargeForce, chargeUpdate;
	int movDamp;

	float t;
	float tRate;
	Quat_t yawRot;			//Yaw about the default up vector
	Quat_t initUpRot;
	Quat_t finalUpRot;
	DIRECTION lastGravDir;

	bool firedeath;
	int gravityTimer;
	int modelAnimationState;
	int swordDamage, chargeDamage;
};


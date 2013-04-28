#pragma once
#include "ServerObject.h"
#include "TentacleSObj.h"

class MonsterSObj : public ServerObject
{
public:
	MonsterSObj(uint id, uint tentId, Model modelNum, Point_t pos, Rot_t rot);
	virtual ~MonsterSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return tentacle->getPhysicsModel(); }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_MONSTER; }
	virtual void onCollision(ServerObject *obj, const Vec3f &collisionNormal){ }
	void hit(int ptAmount);
	TentacleSObj * getTentacle() { return tentacle; }
	
	char serialbuffer[100];

private:
	TentacleSObj *tentacle;
	PhysicsModel *pm;
	int health, maxHealth;
};


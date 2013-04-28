#pragma once
#include "ServerObject.h"
#include "TentacleSObj.h"

class MonsterSObj : public ServerObject
{
public:
	MonsterSObj(uint id, Point_t pos, Rot_t rot);
	virtual ~MonsterSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return tentacle[0]->getPhysicsModel(); }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_MONSTER; }
	virtual void onCollision(ServerObject *obj, const Vec3f &collisionNormal){ }
	void hit(int ptAmount);
	TentacleSObj * getTentacle(int i) { return tentacle[i]; }
	void createTentacle(uint id, Model modelNum, Point_t pos, Rot_t rot) {
		tentacle[numTentacles++] = new TentacleSObj(id, modelNum, pos, rot);
	}
	
	char serialbuffer[100];

private:
	TentacleSObj *tentacle[10];
	PhysicsModel *pm;
	int health, maxHealth, numTentacles;
};


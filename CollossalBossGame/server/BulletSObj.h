#pragma once

#include "ServerObject.h"

class BulletSObj : public ServerObject
{
public:
	BulletSObj(uint id, Model modelNum, Point_t pos, Vec3f initialForce);
	virtual ~BulletSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_GENERAL; }
	virtual void onCollision(ServerObject *obj, const Vec3f &collisionNormal);
	Vec3f getNormal() { return normal; }
	char serialbuffer[100];

	int health;
private:
	PhysicsModel *pm;
	Model modelNum;
	int t;
	Vec3f normal;
};
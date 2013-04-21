#pragma once
#include "ServerObject.h"
class WallSObj :
	public ServerObject
{
public:
	WallSObj(uint id, Model modelNum, Point_t pos, Rot_t rot, Vec3f normal);
	virtual ~WallSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_GENERAL; }
	virtual void onCollision(ServerObject *obj) {}
	Vec3f getNormal() { return normal; }
	char serialbuffer[100];

private:
	PhysicsModel *pm;
	Model modelNum;
	int t;
	Vec3f normal;
};


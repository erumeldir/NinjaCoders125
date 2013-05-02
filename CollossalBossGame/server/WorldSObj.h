#pragma once
#include "ServerObject.h"
#include "defs.h"

class WorldSObj : public ServerObject {
public:
	WorldSObj(uint id);
	virtual ~WorldSObj(void);

	virtual bool update();
	virtual int serialize(char * buf);
	virtual PhysicsModel *getPhysicsModel() { return NULL; }
	virtual ObjectType getType()			{ return OBJ_WORLD; }
	virtual void onCollision(ServerObject *obj, const Vec3f &collisionNormal) {}

private:
	int gravityTimer;
	int gravityInterval;
	DIRECTION gravDir;
};


#pragma once

#include "ServerObject.h"
#include "PlayerSObj.h"

class HarpoonSObj : public ServerObject
{
public:
	HarpoonSObj(uint id, Model modelNum, Point_t pos, Vec3f initialForce, int dmg, int diameter, PlayerSObj * pso);
	virtual ~HarpoonSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_HARPOON; } // Need to make Harpoon extend from bullet maybe. So this works and there isn't a crash if reinterpret cast doesn't work.
	virtual void onCollision(ServerObject *obj, const Vec3f &collisionNormal);
	Vec3f getNormal() { return normal; }
	char serialbuffer[100];

	int health;
	int damage;
	int diameter;
private:
	PhysicsModel *pm;
	Model modelNum;
	int t;
	Vec3f normal;
	PlayerSObj * pso;
};
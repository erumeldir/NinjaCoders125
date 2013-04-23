#pragma once
#include "ServerObject.h"

enum WallDir {
	WALL_NORTH,	//+z (back)
	WALL_EAST,	//+x (right)
	WALL_SOUTH,	//-z (front)
	WALL_WEST,	//-x (left)
	WALL_UP,		//+y (ceiling)
	WALL_DOWN,	//-y (floor)
	NUM_DIRS
};

class WallSObj :
	public ServerObject
{
public:
	WallSObj(uint id, Model modelNum, Point_t pos, WallDir dir);
	virtual ~WallSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_GENERAL; }
	virtual void onCollision(ServerObject *obj) {}
	virtual void initialize();

	Vec3f getNormal() { return normal; }
	char serialbuffer[100];

private:
	PhysicsModel *pm;
	Model modelNum;
	int t;
	Vec3f normal;
};


#pragma once
#include "ServerObject.h"

class MonsterSObj : public ServerObject
{
public:
	MonsterSObj(void);
	virtual ~MonsterSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	void deserialize(char* newInput);
	virtual ObjectType getType() { return OBJ_MONSTER; }
	virtual void onCollision(ServerObject *obj);

	char serialbuffer[100];

private:
	PhysicsModel *pm;
	int health;
};


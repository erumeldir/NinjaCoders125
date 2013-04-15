#pragma once
#include "ServerObject.h"
#include "Action.h"
#include "NetworkData.h"

class TestSObj : public ServerObject {
public:
	TestSObj(uint id);
	virtual ~TestSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	inputstatus istat;
	virtual ObjectType getType() { return OBJ_GENERAL; }

	char serialbuffer[100];

private:
	PhysicsModel *pm;
};


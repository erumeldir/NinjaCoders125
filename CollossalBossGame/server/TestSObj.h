#pragma once
#include "ServerObject.h"
#include "Action.h"

class TestSObj : public ServerObject {
public:
	TestSObj(uint id);
	virtual ~TestSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	inputstatus istat;

	char serialbuffer[100];

private:
	PhysicsModel *pm;
};


#pragma once
#include "ServerObject.h"

class TestSObj : public ServerObject {
public:
	TestSObj(uint id);
	virtual ~TestSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
private:
	PhysicsModel *pm;
};


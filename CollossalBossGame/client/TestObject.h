#pragma once
#include "ClientObject.h"

class TestObject : public ClientObject
{
public:
	TestObject(uint id);
	virtual ~TestObject(void);

	virtual bool update();

private:
	int lifeCounter;
};


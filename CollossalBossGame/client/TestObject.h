#pragma once
#include "ClientObject.h"
#include "Frame.h"

class TestObject : public ClientObject
{
public:
	TestObject(uint id, Frame newCenter);
	virtual ~TestObject(void);

	virtual bool update();

private:
	int lifeCounter;
	Frame center;
};


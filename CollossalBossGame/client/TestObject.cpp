#include "TestObject.h"


TestObject::TestObject(uint id, Frame newCenter) :
	ClientObject(id)
{
	printf("Created new TestObject %d\n", id);
	lifeCounter = id;
	center = newCenter;
}


TestObject::~TestObject(void)
{
}

bool TestObject::update() {
	if(lifeCounter <= 0) {
		return true;
	}
	printf("Hi! My name is TestObject %d!\n", getId());
	lifeCounter--;
	return false;
}
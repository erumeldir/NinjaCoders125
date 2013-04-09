#include "TestObject.h"


TestObject::TestObject(uint id) :
	ClientObject(id)
{
	printf("Created new TestObject %d\n", id);
	lifeCounter = id;
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
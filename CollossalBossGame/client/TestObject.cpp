#include "TestObject.h"
#include "ClientEngine.h"
#include <math.h>
#include <Windows.h>
#include "RenderEngine.h"
#include "NetworkData.h"
#define M_PI 3.14159


TestObject::TestObject(uint id, char *serializedData) :
	ClientObject(id)
{
	DC::get()->print("Created new TestObject %d\n", id);

	ObjectState *state = (ObjectState*)serializedData;

	rm = new RenderModel(Point_t(),Rot_t(), state->modelNum);
	deserialize(serializedData);
}


TestObject::~TestObject(void)
{
	// delete xctrl;
	delete rm;
}

bool TestObject::update() {
	return false;
}

void TestObject::deserialize(char* newState) {
	ObjectState *state = (ObjectState*)newState;
	rm->getFrameOfRef()->deserialize(newState + sizeof(ObjectState));
}

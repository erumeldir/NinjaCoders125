#include "TestSObj.h"


TestSObj::TestSObj(uint id, Model modelNum, Point_t pos) : ServerObject(id) {
	DC::get()->print("Created new TestSObj %d\n", id);
	pm = new PhysicsModel(pos, Rot_t(), 500);

	this->modelNum = modelNum;
}


TestSObj::~TestSObj(void) {
}

bool TestSObj::update() {
	return false;
}

int TestSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = modelNum;
	return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
}

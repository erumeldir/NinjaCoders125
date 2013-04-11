#include "TestSObj.h"


TestSObj::TestSObj(uint id) : ServerObject(id) {
	pm = new PhysicsModel(Point_t(), Rot_t());
}


TestSObj::~TestSObj(void) {
}

bool TestSObj::update() {
	return false;
}

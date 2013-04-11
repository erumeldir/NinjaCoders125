#include "TestSObj.h"


TestSObj::TestSObj(uint id) : ServerObject(id) {
	pm = new PhysicsModel(Point_t(), Rot_t(), 500);
}


TestSObj::~TestSObj(void) {
}

bool TestSObj::update() {
	return false;
}

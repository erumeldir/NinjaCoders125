#include "TestSObj.h"
#include <math.h>

TestSObj::TestSObj(uint id, Model modelNum, Point_t pos, int dir) : ServerObject(id) {
	DC::get()->print("Created new TestSObj %d\n", id);
	pm = new PhysicsModel(pos, Rot_t(), 500);
	this->dir = dir;
	this->modelNum = modelNum;
	switch (modelNum) {
		case MDL_2:
			pm->setColBox(CB_LARGE);
			break;
		default:
			break;
	}
	t = 0;
}


TestSObj::~TestSObj(void) {
}

bool TestSObj::update() {
#define MOVE_AMT 1
#define DIV 25
	/*
	 * North = +Z (away from player start)
	 * East  = +X (right of player start)
	 * South = -Z (towards player start)
	 * West  = -X (left of player start)
	 */
	switch(dir) {
	case TEST_NORTH:
		pm->applyForce(Vec3f(0, 0, MOVE_AMT * sin((double)t / DIV)));
		break;
	case TEST_EAST:
		pm->applyForce(Vec3f(MOVE_AMT * sin((double)t / DIV), 0, 0));
		break;
	case TEST_SOUTH:
		pm->applyForce(Vec3f(0, 0, -MOVE_AMT * sin((double)t / DIV)));
		break;
	case TEST_WEST:
		pm->applyForce(Vec3f(-MOVE_AMT * sin((double)t / DIV), 0, 0));
		break;
	default:
		break;
	}
	++t;

	return false;
}

int TestSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = modelNum;
	return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
}

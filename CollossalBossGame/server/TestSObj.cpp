#include "TestSObj.h"
#include <math.h>
#include "ConfigurationManager.h"

TestSObj::TestSObj(uint id, Model modelNum, Point_t pos, Rot_t rot, int dir) : ServerObject(id) {
	DC::get()->print("Created new TestSObj %d\n", id);
	Box bxVol;
	this->dir = dir;
	this->modelNum = modelNum;
	switch (modelNum) {
		case MDL_4:
		case MDL_5:
		case MDL_1:	//box
			bxVol = CM::get()->find_config_as_box("BOX_CUBE");//Box(-5, 0, -5, 10, 10, 10);
			break;
		case MDL_2:	//Pyramid
			bxVol = CM::get()->find_config_as_box("BOX_PYRAMID");//Box(-20, 0, -20, 40, 40, 40);
			//pm->setColBox(CB_LARGE);
			break;
		case MDL_3:	//plane
#define WALL_WIDTH 150
			bxVol = Box(-WALL_WIDTH / 2, 0, -WALL_WIDTH / 2,
					WALL_WIDTH, 10, WALL_WIDTH);
			//pm->setColBox(CB_FLAT);
			break;
		default:
			bxVol = Box();
			break;
	}
	
	pm = new PhysicsModel(pos, rot, 500, bxVol);
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
		pm->applyForce(Vec3f(0, 0, MOVE_AMT * sin((float)t / DIV)));
		break;
	case TEST_EAST:
		pm->applyForce(Vec3f(MOVE_AMT * sin((float)t / DIV), 0, 0));
		break;
	case TEST_SOUTH:
		pm->applyForce(Vec3f(0, 0, -MOVE_AMT * sin((float)t / DIV)));
		break;
	case TEST_WEST:
		pm->applyForce(Vec3f(-MOVE_AMT * sin((float)t / DIV), 0, 0));
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

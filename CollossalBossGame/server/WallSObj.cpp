#include "WallSObj.h"
#include "defs.h"
#include <math.h>

WallSObj::WallSObj(uint id, Model modelNum, Point_t pos, Rot_t rot) : ServerObject(id) {
	DC::get()->print("Created new WallSObj %d\n", id);
#define WALL_WIDTH 150
	Box bxVol = Box(pos.x - WALL_WIDTH / 2, pos.y, pos.z - WALL_WIDTH / 2,
					WALL_WIDTH, 10, WALL_WIDTH);
	pm = new PhysicsModel(pos, rot, 500, bxVol, true);
	this->modelNum = modelNum;
	pm->setColBox(CB_FLAT);
	t = 0;
	this->setFlag(IS_STATIC, true);
}


WallSObj::~WallSObj(void) {
}

bool WallSObj::update() {
	return false;
}

int WallSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = modelNum;
	return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
}

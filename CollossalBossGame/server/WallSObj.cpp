#include "WallSObj.h"
#include "defs.h"
#include <math.h>

WallSObj::WallSObj(uint id, Model modelNum, Point_t pos, Rot_t rot, Vec3f normal) : ServerObject(id) {
	DC::get()->print("Created new WallSObj %d\n", id);
	this->normal = normal;
#define WALL_WIDTH 150
	Box bxVol;
	/*= Box(-WALL_WIDTH / 2, 0, -WALL_WIDTH / 2,
					WALL_WIDTH, 10, WALL_WIDTH);
					*/
	if(rot.z < -M_PI / 4 && rot.z > -3 * M_PI / 4) {
		DC::get()->print("(left)\n");
		bxVol = Box(-20, -WALL_WIDTH / 2, -WALL_WIDTH / 2,
			20, WALL_WIDTH, WALL_WIDTH);
	} else if(rot.z > 3 * M_PI / 4) {
		DC::get()->print("(ceiling)\n");
		bxVol = Box(-WALL_WIDTH / 2, 0, -WALL_WIDTH / 2,
			WALL_WIDTH, 20, WALL_WIDTH);
	} else if(rot.z > M_PI / 4 && rot.z < 3 * M_PI / 4) {
		DC::get()->print("(right)\n");
		bxVol = Box(0, -WALL_WIDTH / 2, -WALL_WIDTH / 2,
			20, WALL_WIDTH, WALL_WIDTH);
	} else {
		DC::get()->print("(floor)\n");
		bxVol = Box(-WALL_WIDTH / 2, -1000, -WALL_WIDTH / 2,
			WALL_WIDTH, 1000, WALL_WIDTH);
	}
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

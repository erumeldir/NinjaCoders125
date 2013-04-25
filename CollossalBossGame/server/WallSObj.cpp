#include "WallSObj.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include <math.h>

#define WALL_WIDTH 2000 //150; increased size to reduce edge-case collision errors
#define WALL_THICKNESS 200

WallSObj::WallSObj(uint id, Model modelNum, Point_t pos, DIRECTION dir, Vec3f scale) : ServerObject(id) {
	if(SOM::get()->debugFlag) DC::get()->print("Created new WallSObj %d ", id);
	Box bxVol;
	Rot_t rot;
	uint collDir = dir;
	switch(dir) {
	case NORTH:
		DC::get()->print("(north)\n");
		bxVol = Box(-WALL_WIDTH / 2, -WALL_WIDTH / 2, -WALL_THICKNESS,
			WALL_WIDTH, WALL_WIDTH, WALL_THICKNESS);
		//rot = Rot_t(M_PI / 2,0,0);
		rot = Rot_t();
		collDir = NORTH;
		break;
	case SOUTH:
		DC::get()->print("(south)\n");
		bxVol = Box(-WALL_WIDTH / 2, -WALL_WIDTH / 2, 0,
			WALL_WIDTH, WALL_WIDTH, WALL_THICKNESS);
		//rot = Rot_t(-M_PI / 2,0,0);
		rot = Rot_t();
		collDir = SOUTH;
		break;
	case EAST:
		DC::get()->print("(east)\n");
		bxVol = Box(0, -WALL_WIDTH / 2, -WALL_WIDTH / 2,
			WALL_THICKNESS, WALL_WIDTH, WALL_WIDTH);
		//rot = Rot_t(0,0,M_PI / 2);
		rot = Rot_t();
		collDir = WEST;
		break;
	case WEST:
		DC::get()->print("(west)\n");
		bxVol = Box(-WALL_THICKNESS, -WALL_WIDTH / 2, -WALL_WIDTH / 2,
			WALL_THICKNESS, WALL_WIDTH, WALL_WIDTH);
		//rot = Rot_t(0,0,-M_PI / 2);
		rot = Rot_t(0,M_PI,0);
		collDir = EAST;
		break;
	case UP:
		DC::get()->print("(ceiling)\n");
		bxVol = Box(-WALL_WIDTH / 2, 0, -WALL_WIDTH / 2,
			WALL_WIDTH, WALL_THICKNESS, WALL_WIDTH);
		rot = Rot_t(0,0,M_PI);
		collDir = DOWN;
		break;
	default:
		DC::get()->print("(floor)\n");
		bxVol = Box(-WALL_WIDTH / 2, -WALL_THICKNESS, -WALL_WIDTH / 2,
			WALL_WIDTH, WALL_THICKNESS, WALL_WIDTH);
		rot = Rot_t(0,0,0);
		collDir = UP;
		break;
	}

	pm = new PhysicsModel(pos, rot, 500, bxVol, collDir);
	this->modelNum = modelNum;
	this->scale = scale;
	pm->setColBox(CB_FLAT);
	t = 0;
	this->setFlag(IS_STATIC, true);
	this->setFlag(IS_WALL, true);
}

WallSObj::~WallSObj(void) {
	delete pm;
}

bool WallSObj::update() {
	return false;
}

void WallSObj::initialize() {
	// Walls don't have to reinit.
}

int WallSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = modelNum;
	state->scale = scale;
	return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
}

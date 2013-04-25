#include "WallSObj.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include <math.h>

#define WALL_WIDTH 2000 //150; increased size to reduce edge-case collision errors
#define WALL_THICKNESS 20

WallSObj::WallSObj(uint id, Model modelNum, Point_t pos, WallDir dir) : ServerObject(id) {
	if(SOM::get()->debugFlag) DC::get()->print("Created new WallSObj %d ", id);
	Box bxVol;
	Rot_t rot;
	switch(dir) {
	case WALL_NORTH:
		DC::get()->print("(north)\n");
		bxVol = Box(-WALL_WIDTH / 2, -WALL_WIDTH / 2, -WALL_THICKNESS,
			WALL_WIDTH, WALL_WIDTH, WALL_THICKNESS);
		normal = Vec3f( 0, 0, 1);
		rot = Rot_t(M_PI / 2,0,0);
		break;
	case WALL_SOUTH:
		DC::get()->print("(south)\n");
		bxVol = Box(-WALL_WIDTH / 2, -WALL_WIDTH / 2, 0,
			WALL_WIDTH, WALL_WIDTH, WALL_THICKNESS);
		normal = Vec3f( 0, 0, -1);
		rot = Rot_t(-M_PI / 2,0,0);
		break;
	case WALL_EAST:
		DC::get()->print("(east)\n");
		bxVol = Box(0, -WALL_WIDTH / 2, -WALL_WIDTH / 2,
			WALL_THICKNESS, WALL_WIDTH, WALL_WIDTH);
		normal = Vec3f(-1, 0, 0);
		rot = Rot_t(0,0,M_PI / 2);
		break;
	case WALL_WEST:
		DC::get()->print("(west)\n");
		bxVol = Box(-WALL_THICKNESS, -WALL_WIDTH / 2, -WALL_WIDTH / 2,
			WALL_THICKNESS, WALL_WIDTH, WALL_WIDTH);
		normal = Vec3f( 1, 0, 0);
		rot = Rot_t(0,0,-M_PI / 2);
		break;
	case WALL_UP:
		DC::get()->print("(ceiling)\n");
		bxVol = Box(-WALL_WIDTH / 2, 0, -WALL_WIDTH / 2,
			WALL_WIDTH, WALL_THICKNESS, WALL_WIDTH);
		normal = Vec3f( 0,-1, 0);
		rot = Rot_t(0,0,M_PI);
		break;
	default:
		DC::get()->print("(floor)\n");
		bxVol = Box(-WALL_WIDTH / 2, -WALL_THICKNESS, -WALL_WIDTH / 2,
			WALL_WIDTH, WALL_THICKNESS, WALL_WIDTH);
		normal = Vec3f( 0, 1, 0);
		rot = Rot_t(0,0,0);
		break;
	}
	pm = new PhysicsModel(pos, rot, 500, bxVol, true);

	this->modelNum = modelNum;
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

int WallSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = modelNum;
	return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
}

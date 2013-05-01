#include "WorldSObj.h"
#include "PhysicsEngine.h"
#include "ConfigurationManager.h"
#include "NetworkData.h"

WorldSObj::WorldSObj(uint id) : ServerObject(id) {
	setFlag(IS_STATIC, true);
	setFlag(IS_PASSABLE, true);
	gravityInterval = CM::get()->find_config_as_int("GRAVITY_SWITCH_INTERVAL");
	gravityTimer = 0;
}


WorldSObj::~WorldSObj(void) {
}


bool WorldSObj::update() {
	if(gravityInterval > 0) {
		gravityTimer++;

		//Initial gravity-switching test
		static char cdir = 'v';
		if(gravityTimer == gravityInterval) {
			PE::get()->setGravDir(NORTH);
			gravDir = NORTH;
			cdir = 'N';
		} else if(gravityTimer == gravityInterval * 2) {
			PE::get()->setGravDir(UP);
			gravDir = UP;
			cdir = 'U';
		} else if(gravityTimer == gravityInterval * 3) {
			PE::get()->setGravDir(EAST);
			gravDir = EAST;
			cdir = 'E';
		} else if(gravityTimer >= gravityInterval * 4) {
			PE::get()->setGravDir(DOWN);
			gravDir = DOWN;
			gravityTimer = 0;
			cdir = 'D';
		}
		DC::get()->print(CONSOLE, "%c Gravity timer = %d     \r", cdir, gravityTimer);
	}

	return false;
}

int WorldSObj::serialize(char * buf) {
	WorldState *state = (WorldState*)buf;
	state->gravDir = gravDir;
	return sizeof(WorldState);
}

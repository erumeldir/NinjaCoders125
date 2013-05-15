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

enum GRAV_ORDER {
	GRAV_ORDER_START,
//Switch the order of these values to change the order of directions through which gravity will iterate
	GRAV_ORDER_N,
	GRAV_ORDER_E,
	GRAV_ORDER_W,
	GRAV_ORDER_S,
	GRAV_ORDER_U,
	GRAV_ORDER_D,
	GRAV_ORDER_END
};

bool WorldSObj::update() {
	if(gravityInterval >= 0) {
		gravityTimer++;

		//Initial gravity-switching test
		static char cdir = '?';
		if(gravityTimer == gravityInterval * GRAV_ORDER_N) {
			PE::get()->setGravDir(NORTH);
			gravDir = NORTH;
			cdir = 'N';
		} else if(gravityTimer == gravityInterval * GRAV_ORDER_E) {
			PE::get()->setGravDir(EAST);
			gravDir = EAST;
			cdir = 'E';
		} else if(gravityTimer == gravityInterval * GRAV_ORDER_U) {
			PE::get()->setGravDir(UP);
			gravDir = UP;
			cdir = 'U';
		} else if(gravityTimer == gravityInterval * GRAV_ORDER_S) {
			PE::get()->setGravDir(SOUTH);
			gravDir = SOUTH;
			cdir = 'S';
		} else if(gravityTimer == gravityInterval * GRAV_ORDER_W) {
			PE::get()->setGravDir(WEST);
			gravDir = WEST;
			cdir = 'W';
		} else if(gravityTimer == gravityInterval * GRAV_ORDER_D) {
			PE::get()->setGravDir(DOWN);
			gravDir = DOWN;
			cdir = 'D';
		}
		if(gravityTimer >= gravityInterval * (GRAV_ORDER_END - 1)) {
			gravityTimer = 0;
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

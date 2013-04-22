#include "WallSObj.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include <math.h>

WallSObj::WallSObj(uint id, Model modelNum, Point_t pos, Rot_t rot) : ServerObject(id) {
	if(SOM::get()->debugFlag) DC::get()->print("Created new WallSObj %d\n", id);
	pm = new PhysicsModel(pos, rot, 500, true);
	this->modelNum = modelNum;
	pm->setColBox(CB_FLAT);
	t = 0;
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

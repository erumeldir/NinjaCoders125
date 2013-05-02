#include "TentacleCObj.h"
#include "ClientObjectManager.h"

TentacleCObj::TentacleCObj(uint id, char *data) : ClientObject(id, OBJ_TENTACLE)
{
	if (COM::get()->debugFlag) DC::get()->print("Created new TentacleCObj %d\n", id);
	TentacleState *state = (TentacleState*)data;
	rm = new RenderModel(Point_t(),Quat_t(), state->modelNum);
}


TentacleCObj::~TentacleCObj(void)
{
	delete rm;
}

bool TentacleCObj::update() {
	return false;
}

void TentacleCObj::deserialize(char* newState) {
	TentacleState *state = (TentacleState*)newState;
	rm->getFrameOfRef()->deserialize(newState + sizeof(TentacleState));
}
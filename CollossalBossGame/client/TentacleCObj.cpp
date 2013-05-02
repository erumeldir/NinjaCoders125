#include "TentacleCObj.h"
#include "ClientObjectManager.h"

TentacleCObj::TentacleCObj(uint id, char *data) : ClientObject(id)
{
	if (COM::get()->debugFlag) DC::get()->print("Created new TentacleCObj %d\n", id);
	TentacleState *state = (TentacleState*)data;
	rm = new RenderModel(Point_t(),Rot_t(), state->modelNum, state->animationState, Vec3f(1.f,1.f,1.f));
	box = new RenderModel(Point_t(),Rot_t(), (Model)+12, 0, Vec3f(1.f,1.f,1.f));
}

TentacleCObj::~TentacleCObj(void)
{
	delete rm;
}

RenderModel* TentacleCObj::getBox() {
	//1. get the box model
	//2. return the model
	return box;
}

bool TentacleCObj::update() {
	return false;
}

void TentacleCObj::deserialize(char* newState) {
	TentacleState *state = (TentacleState*)newState;
	this->getRenderModel()->setModelState(state->animationState);
	rm->getFrameOfRef()->deserialize(newState + sizeof(TentacleState));
}
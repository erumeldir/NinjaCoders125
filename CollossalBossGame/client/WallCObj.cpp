#include "WallCObj.h"


WallCObj::WallCObj(uint id, char *serializedData) :
	ClientObject(id)
{
	DC::get()->print("Created new WallCObj %d\n", id);

	ObjectState *state = (ObjectState*)serializedData;

	rm = new RenderModel(Point_t(),Rot_t(), state->modelNum);
	deserialize(serializedData);
}


WallCObj::~WallCObj(void)
{
	delete rm;
}

bool WallCObj::update() {
	return false;
}

void WallCObj::deserialize(char* newState) {
	ObjectState *state = (ObjectState*)newState;
	rm->getFrameOfRef()->deserialize(newState + sizeof(ObjectState));
}
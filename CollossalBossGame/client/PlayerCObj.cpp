#include "PlayerCObj.h"
#define M_PI 3.14159

PlayerCObj::PlayerCObj(uint id) :
	ClientObject(id)
{
	printf("Created new PlayerCObj %d\n", id);
	rm = new RenderModel(Point_t(300,500,0),Rot_t(0,0,M_PI));
}


PlayerCObj::~PlayerCObj(void)
{
	delete rm;
}

bool PlayerCObj::update() {
	// TODO: This should delete if the server told it to
	return false;
}

void PlayerCObj::deserialize(char* newState) {
	rm->getFrameOfRef()->deserialize(newState);
}
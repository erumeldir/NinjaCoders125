#include "MonsterCObj.h"
#include "ClientObjectManager.h"


MonsterCObj::MonsterCObj(uint id, char *data) : ClientObject(id)
{
	if (COM::get()->debugFlag) DC::get()->print("Created new MonsterCObj %d\n", id);
	MonsterState *state = (MonsterState*)data;
	this->health = state->health;
	rm = new RenderModel(Point_t(300.f, 500.f, 0.f),Rot_t(0.f, 0.f, M_PI), state->modelNum);
}


MonsterCObj::~MonsterCObj(void)
{
	delete rm;
}

bool MonsterCObj::update() {
	return false;
}

void MonsterCObj::deserialize(char* newState) {
	MonsterState *state = (MonsterState*)newState;
	this->health = state->health;
	rm->getFrameOfRef()->deserialize(newState + sizeof(MonsterState));
}
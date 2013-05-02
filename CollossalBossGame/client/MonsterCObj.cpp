#include "MonsterCObj.h"
#include "ClientObjectManager.h"
#include "RenderEngine.h"


MonsterCObj::MonsterCObj(uint id, char *data) : ClientObject(id)
{
	if (COM::get()->debugFlag) DC::get()->print("Created new MonsterCObj %d\n", id);
	MonsterState *state = (MonsterState*)data;
	this->health = state->health;
	rm = new RenderModel(Point_t(),Rot_t(), (Model)-1);
}


MonsterCObj::~MonsterCObj(void)
{
	// this is to be safe, and to fix a random bug we got 1/10,000 times
	RE::get()->setMonsterHUDText("MONSTER!!", 0);
	delete rm;
}

bool MonsterCObj::update() {
	RE::get()->setMonsterHUDText("MONSTER!!", health);
	return false;
}

void MonsterCObj::deserialize(char* newState) {
	MonsterState *state = (MonsterState*)newState;
	this->health = state->health;
	rm->getFrameOfRef()->deserialize(newState + sizeof(MonsterState));
}
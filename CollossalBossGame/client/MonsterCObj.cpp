#include "MonsterCObj.h"
#include "ClientObjectManager.h"
#include "RenderEngine.h"


MonsterCObj::MonsterCObj(uint id, char *data) : ClientObject(id)
{
	if (COM::get()->debugFlag) DC::get()->print("Created new MonsterCObj %d\n", id);
	MonsterState *state = (MonsterState*)data;
	this->health = state->health;
	rm = NULL;
}

RenderModel* MonsterCObj::getBox() {
	//1. get the box model
	//2. return the model
	return NULL;
}

MonsterCObj::~MonsterCObj(void)
{
	delete rm;
}

bool MonsterCObj::update() {
	RE::get()->setMonsterHUDText("MONSTER!!", health);
	return false;
}

void MonsterCObj::deserialize(char* newState) {
	MonsterState *state = (MonsterState*)newState;
	this->health = state->health;
}
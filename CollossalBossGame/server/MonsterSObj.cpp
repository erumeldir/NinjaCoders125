#include "MonsterSObj.h"
#include "ConfigurationManager.h"

MonsterSObj::MonsterSObj(uint id, Model modelNum, Point_t pos, Rot_t rot) : ServerObject(id)
{
	Box bxVol = CM::get()->find_config_as_box("BOX_CUBE");
	this->modelNum = modelNum;
	pm = new PhysicsModel(pos, rot, CM::get()->find_config_as_float("PLAYER_MASS"), bxVol);
}


MonsterSObj::~MonsterSObj(void)
{
	delete pm;
}

bool MonsterSObj::update() {
	if (health < 0) {
		return true; // I died!
	}

	return false;
}

int MonsterSObj::serialize(char * buf) {
	MonsterState *state = (MonsterState*)buf;
	state->modelNum = this->modelNum;
	state->health = health;
	return pm->ref->serialize(buf + sizeof(MonsterState)) + sizeof(MonsterState);
}

void MonsterSObj::onCollision(ServerObject *obj) {
	// if I collided against the player, AND they're attacking me, loose health
}
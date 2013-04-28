#include "MonsterSObj.h"
#include "ConfigurationManager.h"
#include <time.h>

MonsterSObj::MonsterSObj(uint id, uint tentId, Model modelNum, Point_t pos, Rot_t rot) : ServerObject(id)
{
	this->tentacle = new TentacleSObj(tentId, modelNum, pos, rot);
	this->maxHealth = CM::get()->find_config_as_int("INIT_HEALTH");
	this->health = this->maxHealth;
	this->setFlag(IS_STATIC, 1);

}

MonsterSObj::~MonsterSObj(void)
{
	delete pm;
}

bool MonsterSObj::update() {
	if (health <= 0) {
		return true; // I died!
	} else {
		hit(tentacle->getDamage());
	}
	//attacks are done by the tentacles
	return false;
}

int MonsterSObj::serialize(char * buf) {
	MonsterState *state = (MonsterState*)buf;
	state->modelNum = this->tentacle->getModelNum();
	state->health = health;
	return this->getPhysicsModel()->ref->serialize(buf + sizeof(MonsterState)) + sizeof(MonsterState);
}

void MonsterSObj::hit(int ptAmount) {
	health-=ptAmount;
	if(this->health < 0) health = 0;
	if(this->health > this->maxHealth) health = this->maxHealth;
}
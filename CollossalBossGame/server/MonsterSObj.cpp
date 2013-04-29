#include "MonsterSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include "PlayerSObj.h"
#include <time.h>

MonsterSObj::MonsterSObj(uint id) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new MonsterObj %d\n", id);
	this->health = 0;
	pm = new PhysicsModel(Point_t(), Rot_t(), CM::get()->find_config_as_float("PLAYER_MASS"));
	this->setFlag(IS_STATIC, 1);
	maxTentacles = 0;

}

MonsterSObj::~MonsterSObj(void)
{
	delete pm;
}

/* update()
 * Whenever the tentacle updates, it figures what state it's in (aiming, sweeping, smashing, or idling).
 * If it's idling and someone's nearby, then it can start aiming when we get that figured out. 
 * If idling, someone is not near by, we can randomly start sweeping.
 * Once we get aiming & smashing a single player, if aiming and we've waited enough time, then we smash
 * If attacking or sweeping, we continue.
 *
 * Author: Bryan, Haro, Suman
 */
bool MonsterSObj::update() {
	int numTentacles = tentacles.size();
	health = 0;
	if(numTentacles > 0) {
		for (set<TentacleSObj*>::iterator it = tentacles.begin();
			it != tentacles.end();
			++it)
			health += (*it)->getHealth();
		health /= maxTentacles;
	}

	if (health <= 0 && numTentacles == 0) {
		return true; // I died!
		// DO NOTHING
		// DONT YOU DARE
	}

	// UNSURE for now!
	// Monster AI
	/*
	if (!stateCounter--)
	{
		distribution = std::uniform_int_distribution<int> (CYCLE*9, CYCLE*50);
		stateCounter = distribution(generator);
		// todo config field DC::get()->print("stateCounter %d\n", stateCounter);
	}
	switch (state) 
	{
	case IDLE:

		break;
	case SWEEP:
		state = IDLE;
		break;
	default:
		break;
	}
	*/
	return false;
}

int MonsterSObj::serialize(char * buf) {
	MonsterState *state = (MonsterState*)buf;
	state->health = health;
	return this->getPhysicsModel()->ref->serialize(buf + sizeof(MonsterState)) + sizeof(MonsterState);
}

void MonsterSObj::onCollision(ServerObject *obj, const Vec3f &collisionNormal) {
	// should only collide on tentacle this is a container class
}
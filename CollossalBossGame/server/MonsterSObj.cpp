#include "MonsterSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include "PlayerSObj.h"
#include <time.h>

MonsterSObj::MonsterSObj(uint id) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new MonsterObj %d\n", id);
	Box bxVol = CM::get()->find_config_as_box("BOX_MONSTER");
	//this->modelNum = modelNum;
	this->health = 0;
	pm = new PhysicsModel(Point_t(), Rot_t(), CM::get()->find_config_as_float("PLAYER_MASS"));
	pm->addBox(bxVol);
	//attackCounter = 0;
	this->setFlag(IS_STATIC, 1);
	maxTentacles = 0;
	//srand(time(NULL)); // initialize our random number generator

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

	/*attackCounter++;

	// this emulates an attack

	// start attacking!
	if (attackCounter > attackBuffer) this->setFlag(IS_HARMFUL, 1);

	// now we're done attacking, reset
	if (attackCounter > (attackBuffer + attackFrames))
	{
		attackCounter = 0;
		this->setFlag(IS_HARMFUL, 0);
		attackBuffer = rand() % 40;
		attackFrames = rand() % 15;
	}*/

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
	//state->modelNum = this->modelNum;
	state->health = health;
	return this->getPhysicsModel()->ref->serialize(buf + sizeof(MonsterState)) + sizeof(MonsterState);
}

void MonsterSObj::onCollision(ServerObject *obj, const Vec3f &collisionNormal) {
	// if I collided against the player, AND they're attacking me, loose health
	//string s = typeid(*obj).name();

	// if the monster is attacking, it pushes everything off it on the last attack frame
	/*if (attackCounter == (attackBuffer + attackFrames))
	{
		// todo, this should use the normal too
		const int monsterForce = 30;
		Vec3f up = Vec3f(0, 1, 0);
		obj->getPhysicsModel()->applyForce(up*monsterForce);
	}

	if(!s.compare("class PlayerSObj")) 
	{	
		PlayerSObj* player = reinterpret_cast<PlayerSObj*>(obj);
		if(player->attacking && player->getHealth() > 0) 
		{
			health-=3;
			player->attacking = false;
		}
		if(this->health < 0) health = 0;
		if(this->health > 100) health = 100;
	}*/

	// should only collide on tentacle this is a container class
}

void MonsterSObj::initialize()
{
	// todo franklin?
}
#include "MonsterSObj.h"
#include "ConfigurationManager.h"
#include "PlayerSObj.h"
#include <time.h>

MonsterSObj::MonsterSObj(uint id, Model modelNum, Point_t pos, Rot_t rot) : ServerObject(id)
{
	Box bxVol = CM::get()->find_config_as_box("BOX_MONSTER");
	this->modelNum = modelNum;
	this->health = CM::get()->find_config_as_int("INIT_HEALTH");
	pm = new PhysicsModel(pos, rot, CM::get()->find_config_as_float("PLAYER_MASS"), bxVol);
	attackCounter = 0;
	this->setFlag(IS_STATIC, 1);

	srand(time(NULL)); // initialize our random number generator

	// todo configuration stuff
	attackBuffer = 20;
	attackFrames = 2;
}


MonsterSObj::~MonsterSObj(void)
{
	delete pm;
}

bool MonsterSObj::update() {
	if (health <= 0) {
		return true; // I died!
	}

	attackCounter++;

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
	}

	return false;
}

int MonsterSObj::serialize(char * buf) {
	MonsterState *state = (MonsterState*)buf;
	state->modelNum = this->modelNum;
	state->health = health;
	return pm->ref->serialize(buf + sizeof(MonsterState)) + sizeof(MonsterState);
}

void MonsterSObj::onCollision(ServerObject *obj, const Vec3f &collisionNormal) {
	// if I collided against the player, AND they're attacking me, loose health
	string s = typeid(*obj).name();

	// if the monster is attacking, it pushes everything off it on the last attack frame
	if (attackCounter == (attackBuffer + attackFrames))
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
	}
}
#include "TentacleSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include <time.h>


TentacleSObj::TentacleSObj(uint id, Model modelNum, Point_t pos, Rot_t rot, MonsterSObj* master) : ServerObject(id)
{
	
	if(SOM::get()->debugFlag) DC::get()->print("Created new TentacleObj %d\n", id);
	overlord = master;
	overlord->addTentacle(this);
	Box bxVol = CM::get()->find_config_as_box("BOX_MONSTER");
	this->modelNum = modelNum;
	this->health = CM::get()->find_config_as_int("INIT_HEALTH");
	pm = new PhysicsModel(pos, rot, CM::get()->find_config_as_float("PLAYER_MASS"));
	pm->addBox(bxVol);
	//this->updatableBoxIndex = pm->addBox(updatableBox);
	attackCounter = 0;
	this->setFlag(IS_STATIC, 1);

	srand((uint)time(NULL)); // initialize our random number generator

	// Configuration options
	attackBuffer = CM::get()->find_config_as_int("TENTACLE_ATTACK_BUF");
	attackFrames = CM::get()->find_config_as_int("TENTACLE_ATTACK_FRAMES");
	pushForce = CM::get()->find_config_as_int("TENTACLE_PUSH_FORCE");

	EventManager::get()->fireEvent(EVENT_MONSTER_SPAWN, NULL, NULL);
}


TentacleSObj::~TentacleSObj(void)
{
	delete pm;
}

bool TentacleSObj::update() {
	//changing collision boxes
	//updatableBox.y = -updatableBox.y;
	//pm->updateBox(this->updatableBoxIndex,this->updatableBox);

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

	if (health <= 0) {
		health = 0;
		overlord->removeTentacle(this);
		EventManager::get()->fireEvent(EVENT_MONSTER_DEATH, NULL, NULL);
		return true; // I died!
		//health = 0;
		// fancy animation 
		// just dont attack
		//attackBuffer = 0;
		//attackFrames = 0;
	}

		return false;
}

int TentacleSObj::serialize(char * buf) {
	TentacleState *state = (TentacleState*)buf;
	state->modelNum = this->modelNum;
	//state->health = health;
	return pm->ref->serialize(buf + sizeof(TentacleState)) + sizeof(TentacleState);
}

void TentacleSObj::onCollision(ServerObject *obj, const Vec3f &collisionNormal) {
	// if I collided against the player, AND they're attacking me, loose health
	string s = typeid(*obj).name();

	// if the monster is attacking, it pushes everything off it on the last attack frame
	if (attackCounter == (attackBuffer + attackFrames))
	{
		Vec3f up = Vec3f(0, 1, 0);
		obj->getPhysicsModel()->applyForce((up + collisionNormal)*(float)pushForce);
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
#include "TentacleSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include "PlayerSObj.h"

TentacleSObj::TentacleSObj(uint id, Model modelNum, Point_t pos, Rot_t rot) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new TentacleObj %d\n", id);
	//pm = new PhysicsModel(Point_t(-50,0,150), Rot_t(), 5);
	Box bxVol = CM::get()->find_config_as_box("BOX_MONSTER");
	pm = new PhysicsModel(Point_t(), Rot_t(), CM::get()->find_config_as_float("PLAYER_MASS"), 
		bxVol);
	
	stateCounter = 1;
	state=T_IDLE;

	// todo configuration stuff
	attackCounter = 0;

	attackBuffer = 20;
	attackFrames = 2;

}


TentacleSObj::~TentacleSObj(void)
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
 * Author: Bryan
 */
bool TentacleSObj::update()
{
	if (!stateCounter--)
	{
		distribution = std::uniform_int_distribution<int> (1, CYCLE);
		stateCounter = distribution(generator);
		DC::get()->print("stateCounter %d\n", stateCounter);
		state = T_SWEEP;
	}
	switch (state) 
	{
	case T_IDLE:

		break;
	case T_SWEEP:
		// start attacking, if we haven't already
		this->setFlag(IS_HARMFUL, 1);

		attackCounter++;
		// now we're done attacking, reset
		if (attackCounter > (attackBuffer + attackFrames))
		{
			attackCounter = 0;
			this->setFlag(IS_HARMFUL, 0);
			attackBuffer = rand() % 40;
			attackFrames = rand() % 15;
			this->state = T_IDLE;
		}

		break;
	default:
		break;
	}


	return 0;
}

int TentacleSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = MDL_0;
	return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
}

int TentacleSObj::getDamage() {
	int i = damage;
	damage = 0;
	return i;
}

void TentacleSObj::onCollision(ServerObject *obj, const Vec3f &collisionNormal) {
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
			damage += 3;
			player->attacking = false;
		}
	}
}
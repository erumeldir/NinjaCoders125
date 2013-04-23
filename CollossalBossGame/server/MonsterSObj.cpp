#include "MonsterSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "defs.h"


MonsterSObj::MonsterSObj(uint id) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new TentacleObj %d\n", id);
	//pm = new PhysicsModel(Point_t(-50,0,150), Rot_t(), 5);
	pm = new PhysicsModel(Point_t(), Rot_t(), CM::get()->find_config_as_float("PLAYER_MASS"), 
		Box());
	stateCounter = 1;
	state=IDLE;
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
 * Author: Bryan
 */
bool MonsterSObj::update()
{
	if (!stateCounter--)
	{
		distribution = std::uniform_int_distribution<int> (CYCLE*9, CYCLE*50);
		stateCounter = distribution(generator);
		DC::get()->print("stateCounter %d\n", stateCounter);
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
	return 0;
}

int MonsterSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = MDL_0;
	return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
}

void MonsterSObj::onCollision(ServerObject * other) {
	DC::get()->print("Collided with a Tentacle\n");
}
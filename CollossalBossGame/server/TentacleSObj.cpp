#include "TentacleSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "defs.h"


TentacleSObj::TentacleSObj(uint id) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new TentacleObj %d\n", id);
	//pm = new PhysicsModel(Point_t(-50,0,150), Rot_t(), 5);
	pm = new PhysicsModel(Point_t(), Rot_t(), CM::get()->find_config_as_float("PLAYER_MASS"));

	state=IDLE;
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
	switch (state) 
	{
	case IDLE:

		break;
	case SWEEP:

		break;
	default:
	}
	return 0;
}

int TentacleSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = MDL_0;
	return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
}

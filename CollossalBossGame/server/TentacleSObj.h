/* TentacleSObj.h
 * This defines our tentacle & it's strategy
 *
 * Author: Bryan
 */

#pragma once
#include "serverobject.h"
#include <random>

typedef enum TentacleState {
	IDLE,
	SWEEP,
	NUM_States
};

#define CYCLE 30

class TentacleSObj :
	public ServerObject
{
public:
	TentacleSObj(uint id);
	~TentacleSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_TENTACLE; }
	virtual void onCollision(ServerObject * other);

private:
	PhysicsModel *pm;
	TentacleState state;

	//The logic is that we keep track of how long until we switch to another state.
	int stateCounter;

	//Determining how many cycles we wait until we activate a sweep. 
	//assumes CYCLE cycles per second
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
};


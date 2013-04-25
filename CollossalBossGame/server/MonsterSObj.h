#pragma once
#include "ServerObject.h"
#include <random>

typedef enum TentacleState {
	IDLE,
	SWEEP,
	NUM_States
};

#define CYCLE 30

/* MonsterSObj.h
 * This defines our tentacle & it's strategy
 *
 * Author: Bryan
 */
class MonsterSObj : public ServerObject
{
public:
	MonsterSObj(uint id, Model modelNum, Point_t pos, Rot_t rot);
	virtual ~MonsterSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_MONSTER; }
	virtual void initialize();						//Initial position/rotation/etc of the object
	virtual void onCollision(ServerObject *obj, const Vec3f &collisionNormal);

	char serialbuffer[100];

private:
	PhysicsModel *pm;
	Model modelNum;
	int health;
	int attackCounter; // number of frames in between when the monster is harmful (emulates an 'attack')
	int attackBuffer; // how many frames pass before we're harmful again
	int attackFrames; // how many continuous frames we are harmful

	TentacleState state;

	//The logic is that we keep track of how long until we switch to another state.
	int stateCounter;

	//Determining how many cycles we wait until we activate a sweep. 
	//assumes CYCLE cycles per second
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
};


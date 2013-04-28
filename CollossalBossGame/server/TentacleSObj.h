/* TentacleSObj.h
 * This defines our tentacle & it's strategy
 *
 * Author: Bryan
 */

#pragma once
#include "serverobject.h"
#include <random>

typedef enum TentacleState {
	T_IDLE,
	T_SWEEP,
	NUM_States
};

#define CYCLE 30

class TentacleSObj :
	public ServerObject
{
public:
	TentacleSObj(uint id, Model modelNum, Point_t pos, Rot_t rot);
	~TentacleSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_TENTACLE; }
	virtual void onCollision(ServerObject * other, const Vec3f &collisionNormal);
	Model getModelNum() { return modelNum; }
	int getDamage();

private:
	PhysicsModel *pm;
	TentacleState state;
	Model modelNum;
	int damage;
	//The logic is that we keep track of how long until we switch to another state.
	int stateCounter;

	//Determining how many cycles we wait until we activate a sweep. 
	//assumes CYCLE cycles per second
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;

	int attackCounter; // number of frames in between when the monster is harmful (emulates an 'attack')
	int attackBuffer; // how many frames pass before we're harmful again
	int attackFrames; // how many continuous frames we are harmful
};


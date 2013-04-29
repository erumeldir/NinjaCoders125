#pragma once
#include "ServerObject.h"
#include "TentacleSObj.h"
#include <random>
//#include <vector>

#define CYCLE 30

/* MonsterSObj.h
 * This defines our tentacle & it's strategy
 *
 * Author: Bryan
 */
// fwd decl
class TentacleSObj;

class MonsterSObj : public ServerObject
{
public:
	MonsterSObj(uint id);
	virtual ~MonsterSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	virtual ObjectType getType() { return OBJ_MONSTER; }
	virtual void onCollision(ServerObject *obj, const Vec3f &collisionNormal);

	void addTentacle(TentacleSObj* t) { tentacles.insert(t); maxTentacles++; }
	void removeTentacle(TentacleSObj* t) { tentacles.erase(t); }

	char serialbuffer[100];

private:
	TentacleSObj *tentacle[10];
	PhysicsModel *pm;
	//Model modelNum;
	int health;
	//int attackCounter; // number of frames in between when the monster is harmful (emulates an 'attack')
	//int attackBuffer; // how many frames pass before we're harmful again
	//int attackFrames; // how many continuous frames we are harmful

	set<TentacleSObj*> tentacles;
	int maxTentacles;
	//TentacleState state;

	//The logic is that we keep track of how long until we switch to another state.
	//int stateCounter;

	//Determining how many cycles we wait until we activate a sweep. 
	//assumes CYCLE cycles per second
	//std::default_random_engine generator;
	//std::uniform_int_distribution<int> distribution;
};


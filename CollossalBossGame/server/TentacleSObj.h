/* TentacleSObj.h
 * This defines our tentacle & it's strategy
 *
 * Author: Bryan
 */

#pragma once
#include "serverobject.h"

typedef enum TentacleState {
	IDLE,
	SWEEP,
	NUM_States
};

class TentacleSObj :
	ServerObject
{
public:
	TentacleSObj(uint id);
	~TentacleSObj(void);

	virtual bool update();
	virtual PhysicsModel *getPhysicsModel() { return pm; }
	virtual int serialize(char * buf);
	void deserialize(char* newInput);
	virtual ObjectType getType() { return OBJ_TENTACLE; }

private:
	PhysicsModel *pm;
	TentacleState state;

};


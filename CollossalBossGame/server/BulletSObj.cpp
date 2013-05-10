#include "BulletSObj.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include <math.h>

BulletSObj::BulletSObj(uint id, Model modelNum, Point_t pos, Vec3f initialForce) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new Bullet %d ", id);
	Box bxVol;
	Quat_t rot = Quat_t();
	bxVol = Box(pos.x, pos.y, pos.z, 1, 1, 1);
	rot = Quat_t();

	pm = new PhysicsModel(pos, rot, 500);
	pm->addBox(bxVol);
	this->modelNum = modelNum;
	t = 0;
	health = 5;
}


BulletSObj::~BulletSObj(void)
{
	delete pm;
}

bool BulletSObj::update() {
	// Apply Force of Gravity on every time step - or not, since we wanted an arc-ing shot
	// return true when it collides with something?
	// That'll wait for onCollision, I suppose.
	return this->health;
}

int BulletSObj::serialize(char * buf) {
	// All this ObjectState stuff is extra. TODO: Remove extra. 

	ObjectState *state = (ObjectState*)buf;
	state->modelNum = modelNum;

	if (SOM::get()->collisionMode)
	{
		CollisionState *collState = (CollisionState*)(buf + sizeof(ObjectState));

		vector<Box> objBoxes = pm->colBoxes;

		collState->totalBoxes = min(objBoxes.size(), maxBoxes);

		for (int i=0; i<collState->totalBoxes; i++)
		{
			collState->boxes[i] = objBoxes[i] + pm->ref->getPos(); // copying applyPhysics
		}

		return pm->ref->serialize(buf + sizeof(ObjectState) + sizeof(CollisionState)) + sizeof(ObjectState) + sizeof(CollisionState);
	}
	else
	{
		return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
	}
}

void BulletSObj::onCollision(ServerObject *obj, const Vec3f &collNorm) {
	if(obj->getType() == OBJ_TENTACLE
		|| obj->getType() == OBJ_GENERAL) {
		this->health = 0;
	}
}
#include "HarpoonSObj.h"
#include "ShooterSObj.h"
#include "PhysicsEngine.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include <math.h>

HarpoonSObj::HarpoonSObj(uint id, Model modelNum, Point_t pos, Vec3f initialForce, int dmg, int diameter, PlayerSObj * pso) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new Bullet %d ", id);
	Box bxVol;
	Quat_t rot = Quat_t();
	int negathing = -(diameter/2);

	bxVol = Box(negathing, negathing, negathing, diameter, diameter, diameter);
	rot = Quat_t();

	pm = new PhysicsModel(pos, rot, 50);
	pm->addBox(bxVol);
	pm->applyForce(initialForce);

	this->modelNum = modelNum;
	t = 0;
	health = 5;
	this->damage = dmg;
	this->diameter = diameter;
	this->pso = pso;
}


HarpoonSObj::~HarpoonSObj(void)
{
	delete pm;
}

bool HarpoonSObj::update() {
	// Apply Force of Gravity on every time step - or not, since we wanted an arc-ing shot
	// return true when it collides with something?
	// That'll wait for onCollision, I suppose.
	this->setFlag(IS_FALLING, 0); // YAY IT'S A LASER PEWPEW
	if(this->getFlag(IS_STATIC)) {
		assert(this->pso != NULL && "I'm not sure what happened here.");
		Vec3f hl = this->pm->ref->getPos();
		Vec3f pl = pso->getPhysicsModel()->ref->getPos();
		Vec3f v = (hl - pl);
		if(fabs(v.x) < 15 && fabs(v.y) < 15 && fabs(v.z) < 15) {
			// pso->setFlag(IS_FLOATING, 1);
			pso->getPhysicsModel()->accel = Vec3f();
			pso->getPhysicsModel()->vel = Vec3f();
		} else {
			v.normalize();
			pso->getPhysicsModel()->accel = (v/2);
			// pso->getPhysicsModel()->vel = v;
			pso->setFlag(IS_FLOATING, 1);
		}
	}
	if (this->health > 0) {
		return false;
	} else {
		return true;
	}
}

int HarpoonSObj::serialize(char * buf) {
	// All this ObjectState stuff is extra. TODO: Remove extra. 
	*(int *)buf = diameter;
	buf = buf + 4;

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

void HarpoonSObj::onCollision(ServerObject *obj, const Vec3f &collNorm) {
	if(obj->getType() == OBJ_GENERAL) {
		this->setFlag(IS_STATIC, 1);
	}
	if(obj->getType() == OBJ_TENTACLE) {
		pso->clearAccessory();
	}
	if(obj->getType() == OBJ_PLAYER) {
		// this->health = 0;
	}
}
#include "PhysicsEngine.h"
#include "ConfigurationManager.h"
#define TIMESTEP 5

//Movement Defines
#define MAX_VEL 5.0f

//Collision Defines
#define SMALLRADIUS 5.0f
#define LARGERADIUS 30.0f

//Static members
PhysicsEngine *PhysicsEngine::pe;

PhysicsEngine::PhysicsEngine(void)
{
	// Configuration options
	gravMag = CM::get()->find_config_as_float("GRAVITY_FORCE");

	xPos = yPos = zPos = 500;
	xNeg = yNeg = zNeg = 0;
	gravVec = Vec3f(0, -1, 0);
	gravDir = DOWN;
}


PhysicsEngine::~PhysicsEngine(void)
{
}


/*
 * applyPhysics(PhysicsModel *mdl)
 * Updates the physical position of the model and returns true if this object
 * should have collision physics applied.
 */
bool PhysicsEngine::applyPhysics(ServerObject *obj) {
	float dt = TIMESTEP;

	if(obj->getFlag(IS_STATIC)) return true;

	PhysicsModel *mdl = obj->getPhysicsModel();

	//Apply gravity if not falling; otherwise, apply friction
	if(obj->getFlag(IS_FALLING)) {
		//gravity
		mdl->applyAccel(gravVec*gravMag);
	} else {
		//friction
	}


	//Update position
	mdl->lastPos = mdl->ref->getPos();
	//if(mdl->ref->getPos().y <= 0) mdl->lastPosOnGround = mdl->ref->getPos();
	float dx = 0.5f * mdl->accel.x * dt * dt + mdl->vel.x * dt,
		  dy = 0.5f * mdl->accel.y * dt * dt + mdl->vel.y * dt,
		  dz = 0.5f * mdl->accel.z * dt * dt + mdl->vel.z * dt;
	mdl->ref->translate(Point_t(dx, dy, dz));



	//Update velocity
	mdl->vel.x = mdl->accel.x * dt + mdl->vel.x / mdl->frictCoeff;
	mdl->vel.y = mdl->accel.y * dt + mdl->vel.y / mdl->frictCoeff;
	mdl->vel.z = mdl->accel.z * dt + mdl->vel.z / mdl->frictCoeff;

	//Cap velocity
	float magSq = mdl->vel.x * mdl->vel.x + mdl->vel.y * mdl->vel.y + mdl->vel.z * mdl->vel.z;
	if(magSq > MAX_VEL * MAX_VEL) {
		mdl->vel *= MAX_VEL / sqrt(magSq);
	}

	//Update acceleration
	mdl->accel = Vec3f();

#if 0
	//Cap position
	Point_t pos = mdl->ref->getPos();
	if(pos.y + mdl->vol.y < yNeg) {
		pos.y = yNeg - mdl->vol.y;
	} else if(pos.y + mdl->vol.y + mdl->vol.h > yPos) {
		pos.y = yPos - (mdl->vol.y + mdl->vol.h);
	}
	if(pos.x + mdl->vol.x < xNeg) {
		pos.x = xNeg - mdl->vol.x;
	} else if(pos.x + mdl->vol.x + mdl->vol.w > xPos) {
		pos.x = xPos - (mdl->vol.x + mdl->vol.w);
	}
	if(pos.z + mdl->vol.z < zNeg) {
		pos.z = zNeg - mdl->vol.z;
	} else if(pos.z + mdl->vol.z + mdl->vol.l > zPos) {
		pos.z = zPos - (mdl->vol.z + mdl->vol.l);
	}
	mdl->ref->setPos(Point_t(pos.x, pos.y, pos.z));
#endif

	//Object falls if it has moved (it may not fall after collision checks have been applied)
	if(fabs(dx) > 0 || fabs(dy) > 0 || fabs(dz) > 0) {
		obj->setFlag(IS_FALLING, true);
		mdl->frictCoeff = AIR_FRICTION;
	}

	return true;	//We'll add a detection for has-moved later
}

void PhysicsEngine::applyPhysics(ServerObject *obj1, ServerObject *obj2)
{
	// go through all the boxes of obj1 
	vector<Box> obj1Boxes = obj1->getPhysicsModel()->colBoxes;
	vector<Box> obj2Boxes = obj2->getPhysicsModel()->colBoxes;
	for(std::vector<Box>::iterator box1 = obj1Boxes.begin(); box1 != obj1Boxes.end(); ++box1)
		// go through all the boxes of obj
		for(std::vector<Box>::iterator box2 = obj2Boxes.begin(); box2 != obj2Boxes.end(); ++box2)
		{
			applyPhysics(obj1, obj2, *box1, *box2);
		}
}

void PhysicsEngine::applyPhysics(ServerObject *obj1, ServerObject *obj2, Box b1, Box b2) {
	PhysicsModel *mdl1 = obj1->getPhysicsModel(),
				 *mdl2 = obj2->getPhysicsModel();
	Box bx1 = b1 + mdl1->ref->getPos(),
		bx2 = b2 + mdl2->ref->getPos();
	Vec3f collNorm1 = Vec3f(),
		  collNorm2 = Vec3f();

	//Check for collision
	if(!aabbCollision(bx1,bx2)) {
		return;
	}

	//Passable objects or static pairs cannot be collided with
	if((obj1->getFlag(IS_PASSABLE) || obj2->getFlag(IS_PASSABLE)) ||
			(obj1->getFlag(IS_STATIC) && obj2->getFlag(IS_STATIC))) {
		obj1->onCollision(obj2, collNorm1);
		obj2->onCollision(obj1, collNorm2);
		return;
	}
	bool applyForce = ((gravVec.x == 0) && (gravVec.y == 0) && (gravVec.z == 0));

	bool canShiftNegX = (mdl1->cdirs & EAST)  && (mdl2->cdirs & WEST),
		 canShiftPosX = (mdl1->cdirs & WEST)  && (mdl2->cdirs & EAST),
		 canShiftNegY = (mdl1->cdirs & UP)    && (mdl2->cdirs & DOWN),
		 canShiftPosY = (mdl1->cdirs & DOWN)  && (mdl2->cdirs & UP),
		 canShiftNegZ = (mdl1->cdirs & NORTH) && (mdl2->cdirs & SOUTH),
		 canShiftPosZ = (mdl1->cdirs & SOUTH) && (mdl2->cdirs & NORTH);
	bool canShiftX, canShiftY, canShiftZ;

	//Move out bounding boxes if collision occurs
	float fXShift1 = bx2.x - (bx1.x + bx1.w),
          fXShift2 = (bx2.x + bx2.w) - bx1.x,
          fXShift;//  = fabs(fXShift1) < fabs(fXShift2) ? fXShift1 : fXShift2;
    float fYShift1 = bx2.y - (bx1.y + bx1.h),
          fYShift2 = (bx2.y + bx2.h) - bx1.y,
          fYShift;//  = fabs(fYShift1) < fabs(fYShift2) ? fYShift1 : fYShift2;
    float fZShift1 = bx2.z - (bx1.z + bx1.l),
          fZShift2 = (bx2.z + bx2.l) - bx1.z,
          fZShift;//  = fabs(fZShift1) < fabs(fZShift2) ? fZShift1 : fZShift2;

	if(canShiftNegX && fabs(fXShift1) < fabs(fXShift2)) {
		fXShift = fXShift1;
		canShiftX = canShiftNegX;
	} else {
		fXShift = fXShift2;
		canShiftX = canShiftPosX;
	}

	if(fabs(fYShift1) < fabs(fYShift2)) {
		fYShift = fYShift1;
		canShiftY = canShiftNegY;
	} else {
		fYShift = fYShift2;
		canShiftY = canShiftPosY;
	}

	if(fabs(fZShift1) < fabs(fZShift2)) {
		fZShift = fZShift1;
		canShiftZ = canShiftNegZ;
	} else {
		fZShift = fZShift2;
		canShiftZ = canShiftPosZ;
	}

	//DC::get()->print(CONSOLE | LOGFILE, "Shift capabilities b/w obj%d, obj%d: (%d,%d,%d) ", obj1->getId(), obj2->getId(), canShiftX, canShiftY, canShiftZ);

	Vec3f ptObj1Shift, ptObj2Shift;
	float sign = 1.0;
    if(canShiftX && fabs(fXShift) < fabs(fYShift) && fabs(fXShift) < fabs(fZShift)) {
        //Shift by X
        if(obj1->getFlag(IS_STATIC)) {
            ptObj1Shift = Vec3f();
            ptObj2Shift = Vec3f(-fXShift, 0, 0);
        } else if(obj2->getFlag(IS_STATIC)) {
            ptObj1Shift = Vec3f(fXShift, 0, 0);
            ptObj2Shift = Vec3f();

        } else {    //Split evenly
            ptObj1Shift = Vec3f(fXShift / 2, 0, 0);
            ptObj2Shift = Vec3f(-fXShift / 2, 0, 0);
        }
		sign = fXShift < 0 ? -1.f : 1.f;
		collNorm1 = Vec3f(sign,0,0);
		collNorm2 = Vec3f(-sign,0,0);
		mdl1->vel.x = 0;
		mdl2->vel.x = 0;
		//DC::get()->print(CONSOLE | LOGFILE, "(shifted x %f (sign = %f))\n", fXShift, sign);
    } else if(canShiftY && fabs(fYShift) < fabs(fXShift) && fabs(fYShift) < fabs(fZShift)) {
        //Shift by Y (vertical)
        if(obj1->getFlag(IS_STATIC)) {
            ptObj1Shift = Vec3f();
            ptObj2Shift = Vec3f(0, -fYShift, 0);
        } else if(obj2->getFlag(IS_STATIC)) {
            ptObj1Shift = Vec3f(0, fYShift, 0);
            ptObj2Shift = Vec3f();
        } else {    //Split evenly
            ptObj1Shift = Vec3f(0, fYShift / 2, 0);
            ptObj2Shift = Vec3f(0, -fYShift / 2, 0);
        }
		sign = fYShift < 0 ? -1.f : 1.f;
		collNorm1 = Vec3f(0,sign,0);
		collNorm2 = Vec3f(0,-sign,0);
		
		//Stop the lower object from falling
        if(bx2.y > bx1.y) {
            obj2->setFlag(IS_FALLING, false);
			obj2->getPhysicsModel()->frictCoeff = GROUND_FRICTION;
        } else {
			obj1->setFlag(IS_FALLING, false);
			obj1->getPhysicsModel()->frictCoeff = GROUND_FRICTION;
        }
		mdl1->vel.y = 0;
		mdl2->vel.y = 0;
		//DC::get()->print(CONSOLE | LOGFILE, "(shifted y %f (sign = %f))\n", fYShift, sign);
    } else {
        //Shift by Z
        if(obj1->getFlag(IS_STATIC)) {
            ptObj1Shift = Vec3f();
            ptObj2Shift = Vec3f(0, 0, -fZShift);
        } else if(obj2->getFlag(IS_STATIC)) {
            ptObj1Shift = Vec3f(0, 0, fZShift);
            ptObj2Shift = Vec3f();
        } else {    //Split evenly
            ptObj1Shift = Vec3f(0, 0, fZShift / 2);
            ptObj2Shift = Vec3f(0, 0, -fZShift / 2);
        }
		sign = fZShift < 0 ? -1.f : 1.f;
		collNorm1 = Vec3f(0,0,sign);
		collNorm2 = Vec3f(0,0,-sign);
		mdl1->vel.z = 0;
		mdl2->vel.z = 0;
		//DC::get()->print(CONSOLE | LOGFILE, "(shifted z %f (sign = %f))\n", fZShift, sign);
	}

#if 1
	//Move the objects by the specified amount
	mdl1->ref->translate(ptObj1Shift);
	mdl2->ref->translate(ptObj2Shift);
	if(applyForce) {
		mdl1->applyForce(collNorm1);
		mdl2->applyForce(collNorm2);
	}
#endif
	//Inform the logic module of the collision event
	obj1->onCollision(obj2, collNorm1);
	obj2->onCollision(obj1, collNorm2);
}

bool PhysicsEngine::aabbCollision(const Box &bx1, const Box &bx2) {
	return !(bx1.x + bx1.w < bx2.x ||
			 bx1.y + bx1.h < bx2.y ||
			 bx1.z + bx1.l < bx2.z ||
			 bx1.x > bx2.x + bx2.w ||
			 bx1.y > bx2.y + bx2.h ||
			 bx1.z > bx2.z + bx2.l);
}
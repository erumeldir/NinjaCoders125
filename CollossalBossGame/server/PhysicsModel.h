#pragma once
#include "defs.h"
#include "Frame.h"

//All physics data should be known to the frames
struct PhysicsModel
{
	PhysicsModel(Point_t pos, Rot_t rot) {
		ref = new Frame(pos,rot);
		appliedAccel = Vec3f();

	}

	virtual ~PhysicsModel() {
		delete ref;
	}

	Frame *ref;	//Frame of Reference/skeleton; also root position and collision info
	Vec3f vel;			//Current velocity
	Vec3f accel;		//Current acceleration
	Vec3f appliedAccel;	//Forces/accelerations applied (we'll worry about rotational stuff later)
	float mass;			//Mass of this object
};

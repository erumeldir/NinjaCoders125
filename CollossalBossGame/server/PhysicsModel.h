#pragma once
#include "defs.h"
#include "Frame.h"
#include <vector>
#define UNITOFHALFLENGTH 25		//as in half the length of a box

//All physics data should be known to the frames
struct PhysicsModel
{
	PhysicsModel(Point_t pos, Quat_t rot, float mass, uint collisionDirs = (NORTH | SOUTH | EAST | WEST | UP | DOWN)) {
		ref = new Frame(pos,rot);
		this->lastPos = pos;
		vel = Vec3f();
		accel = Vec3f();
		this->mass = mass;
		frictCoeff = GROUND_FRICTION;
		this->cdirs = collisionDirs;
	}

	virtual ~PhysicsModel() {
		colBoxes.clear();
		delete ref;
	}


	void applyForce(const Vec3f &force) {
		this->accel.x += force.x / mass;
		this->accel.y += force.y / mass;
		this->accel.z += force.z / mass;
	}

	void applyAccel(const Vec3f &accel) {
		this->accel.x += accel.x;
		this->accel.y += accel.y;
		this->accel.z += accel.z;
	}
	
	int addBox(Box b) { colBoxes.push_back(b); return colBoxes.size() - 1; }
	bool updateBox(uint i, Box b) 
	{ 
		if(i < colBoxes.size())
		{
			colBoxes[i] = b; 
			return true;
		}
		return false;
	}

	Frame *ref;	//Frame of Reference/skeleton; also root position and collision info
	Vec3f vel;			//Current velocity
	Vec3f accel;		//Current acceleration
	Point_t lastPos;
	float mass;			//Mass of this object
	float frictCoeff;	//Friction coefficient
	Vec3f frictNorm;	//Normal on which the friction will be applied
	//Box vol;
	uint cdirs;			//Which faces of the collision volume are valid for collision checking?
	vector<Box> colBoxes;
};

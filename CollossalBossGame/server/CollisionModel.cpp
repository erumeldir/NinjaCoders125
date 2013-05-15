#include "CollisionModel.h"
#include <math.h>

/*
 * Model constructors
 */
HMapModel::HMapModel(const char *filename, const Vec3f &offset, int unitLength, float scale, DIRECTION normalDir) {
	bCreatedHMap = true;
	init(new HMap(filename, unitLength, scale), offset, normalDir);
}

//We're ignoring the normal direction atm
HMapModel::HMapModel(HMap *hmap, const Vec3f &offset, DIRECTION normalDir) {
	bCreatedHMap = false;
	init(hmap, offset, normalDir);				
}

void HMapModel::init(HMap *hmap, const Vec3f &offset, DIRECTION normalDir) {
	this->hmap = hmap;
	float max = hmap->getMax();
	this->bxTotalVolume = Box(offset.x, offset.y, offset.z, hmap->getWidth(), max, hmap->getLength());
}

HMapModel::~HMapModel() {
	if(bCreatedHMap) {
		delete hmap;
	}
}

bool areColliding(const Box &bx1, const Box &bx2) {
	return !(bx1.x + bx1.w < bx2.x ||
			 bx1.y + bx1.h < bx2.y ||
			 bx1.z + bx1.l < bx2.z ||
			 bx1.x > bx2.x + bx2.w ||
			 bx1.y > bx2.y + bx2.h ||
			 bx1.z > bx2.z + bx2.l);
}

bool pointOnHMapCollision(const Point_t &pt, const Point_t &hmapPos, const HMapModel &hmap) {
	//Transform point so it is relative to the hmap top-left corner
	float x = (pt.x - (hmapPos.x + hmap.bxTotalVolume.x)) / hmap.hmap->getUnitLength(),
		  y = (pt.y - (hmapPos.y + hmap.bxTotalVolume.y)),	//Don't normalize the height
		  z = (pt.z - (hmapPos.z + hmap.bxTotalVolume.z)) / hmap.hmap->getUnitLength();
	
	//Get indices of corners
	int minI = floor(x),
		minJ = floor(z),
		maxI = ceil(x),
		maxJ = ceil(z);

	//Determine which triangle to check
	Vec3f v0, v1, norm;
	float h00 = hmap.hmap->getHeightAt(minI,minJ),
		  h11 = hmap.hmap->getHeightAt(maxI,maxJ);
	if(z - minJ < x - minI) {
		//Get two vectors in the plane of the triangle, such that crossing them gets the correct normal
		float h10 = hmap.hmap->getHeightAt(maxI,minJ);
		v0 = Vec3f(-1, h00 - h10, 0);
		v1 = Vec3f(0, h11 - h10, 1);
	} else {
		float h01 = hmap.hmap->getHeightAt(maxI,minJ);
		v0 = Vec3f(0, h11 - h01, 1);
		v1 = Vec3f(-1, h00 - h01, 0);
	}
	Point_t ptOnPlane = Point_t(minI,h00,minJ);	//guaranteed to be on both planes
	cross(&norm, v0, v1);
	norm.normalize();	//We only normalize this because it might serve as the collision normal later

	//now that we have the normal and a point pt on the plane, we can use the
	// equation of the plane to determine whether the point is above or below
	// the heightmap.  If it's above, then there is no collision.  If its
	// below, then a collision occurred.
	// Since we have all of the information here, we might even return the collision normal
	// and height difference as well.

	//Equation of a plane: norm * (pt - ptOnPlane), except using adjusted pt values.
	float hdiff = ptOnPlane.y - (norm.x * (x - ptOnPlane.x) + norm.z * (z - ptOnPlane.z)) / norm.y	//height of the hmap at the current position
		- y;	//The current player's y-position, translated so it is relative the hmap's topleft-most corner
	return hdiff >= 0;	//hdiff is the amount of shift that needs to happen to move the object out of the heightmap.
}

bool areColliding(const Box &bx1, const Point_t &hmapCenter, const HMapModel &hmap) {
	if(areColliding(bx1, hmap.bxTotalVolume)) {
	}
	return false;
}

/*
 * Extract the shift axis and magnitude, as well as the collision normal
 * The calling function is responsible for deciding which box moves which amount.
 * Assumes that a collision has already been detected.
 */
void getCollisionInfo(Vec3f *shift, DIRECTION *collDir, const Box &bx1, const Box &bx2) {
	//Move out bounding boxes if collision occurs
	float fXShift1 = bx2.x - (bx1.x + bx1.w),
          fXShift2 = (bx2.x + bx2.w) - bx1.x,
          fXShift  = fabs(fXShift1) < fabs(fXShift2) ? fXShift1 : fXShift2;
    float fYShift1 = bx2.y - (bx1.y + bx1.h),
          fYShift2 = (bx2.y + bx2.h) - bx1.y,
          fYShift  = fabs(fYShift1) < fabs(fYShift2) ? fYShift1 : fYShift2;
    float fZShift1 = bx2.z - (bx1.z + bx1.l),
          fZShift2 = (bx2.z + bx2.l) - bx1.z,
          fZShift  = fabs(fZShift1) < fabs(fZShift2) ? fZShift1 : fZShift2;

	
	*shift = Vec3f();	//Clear the shift and normal vectors
	float sign = 0.0f;
    if(fabs(fXShift) < fabs(fYShift) && fabs(fXShift) < fabs(fZShift)) {
        //Shift by X
		shift->x = fXShift;
		*collDir = fXShift < 0 ? WEST : EAST;
#if 0
		//Stop the lower object from falling
        if( ((gravDir == WEST) && (bx2.x + obj2Shift.x > bx1.x + obj1Shift.x)) ||
			((gravDir == EAST) && (bx2.x + obj2Shift.x < bx1.x + obj1Shift.x)) ) {
            obj2->setFlag(IS_FALLING, false);
			obj2->getPhysicsModel()->frictCoeff = GROUND_FRICTION;
        } else if( ((gravDir == WEST) && (bx1.x + obj1Shift.x > bx2.x + obj2Shift.x)) ||
				   ((gravDir == EAST) && (bx1.x + obj1Shift.x < bx2.x + obj2Shift.x)) ) {
			obj1->setFlag(IS_FALLING, false);
			obj1->getPhysicsModel()->frictCoeff = GROUND_FRICTION;
		}
#endif
    } else if(fabs(fYShift) < fabs(fXShift) && fabs(fYShift) < fabs(fZShift)) {
        //Shift by Y (vertical)
		shift->y = fYShift;
		*collDir = fYShift < 0 ? DOWN : UP;
#if 0
		//Stop the lower object from falling
        if( ((gravDir == DOWN) && (bx2.y + obj2Shift.y > bx1.y + obj1Shift.y)) ||
			((gravDir == UP)   && (bx2.y + obj2Shift.y < bx1.y + obj1Shift.y)) ) {
            obj2->setFlag(IS_FALLING, false);
			obj2->getPhysicsModel()->frictCoeff = GROUND_FRICTION;
        } else if( ((gravDir == DOWN) && (bx1.y + obj1Shift.y > bx2.y + obj2Shift.y)) ||
				   ((gravDir == UP)   && (bx1.y + obj1Shift.y < bx2.y + obj2Shift.y)) ) {
			obj1->setFlag(IS_FALLING, false);
			obj1->getPhysicsModel()->frictCoeff = GROUND_FRICTION;
		}
#endif
    } else {
        //Shift by Z
		shift->z = fZShift;
		*collDir = fZShift < 0 ? SOUTH : NORTH;
#if 0
		//Stop the lower object from falling
        if( ((gravDir == SOUTH) && (bx2.z + obj2Shift.z > bx1.z + obj1Shift.z)) ||
			((gravDir == NORTH) && (bx2.z + obj2Shift.z < bx1.z + obj1Shift.z)) ) {
            obj2->setFlag(IS_FALLING, false);
			obj2->getPhysicsModel()->frictCoeff = GROUND_FRICTION;
        } else if( ((gravDir == SOUTH) && (bx1.z + obj1Shift.z > bx2.z + obj2Shift.z)) ||
				   ((gravDir == NORTH) && (bx1.z + obj1Shift.z < bx2.z + obj2Shift.z)) ) {
			obj1->setFlag(IS_FALLING, false);
			obj1->getPhysicsModel()->frictCoeff = GROUND_FRICTION;
		}
#endif
	}
}

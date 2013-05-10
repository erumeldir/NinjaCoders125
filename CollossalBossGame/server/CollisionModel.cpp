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

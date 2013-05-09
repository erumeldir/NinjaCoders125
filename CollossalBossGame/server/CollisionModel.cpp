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

bool aabbOnAabbCollision(const Box &bx1, const Box &bx2) {
	return !(bx1.x + bx1.w < bx2.x ||
			 bx1.y + bx1.h < bx2.y ||
			 bx1.z + bx1.l < bx2.z ||
			 bx1.x > bx2.x + bx2.w ||
			 bx1.y > bx2.y + bx2.h ||
			 bx1.z > bx2.z + bx2.l);
}

bool pointOnHMapCollision(const Point_t &pt, const Point_t &hmapCenter, const HMapModel &hmap) {
	int minI, maxI,
		minJ, maxJ;
	//Start with only support for floor heightmaps
	Point_t pos = Point_t(pt.x - hmap.bxTotalVolume.x,
						  pt.y - hmap.bxTotalVolume.y,
						  pt.z - hmap.bxTotalVolume.z);
	minI = (int)floor(pos.x);
	maxI = (int)ceil(pos.x);
	minJ = (int)floor(pos.y);
	maxJ = (int)ceil(pos.y);
	return false;
}

bool aabbOnHMapCollision(const Box &bx1, const Point_t &hmapCenter, const HMapModel &hmap) {
	if(aabbOnAabbCollision(bx1, hmap.bxTotalVolume)) {
	}
	return false;
}

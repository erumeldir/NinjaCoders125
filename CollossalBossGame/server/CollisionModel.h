#pragma once

#include "defs.h"
#include "HMap.h"
#include <vector>
using namespace std;

enum CollisionType {
	CMDL_AABB,
	CMDL_HMAP,
	NUM_COLLISION_TYPES
};

/*
 * Note that all collision models are relative to a frame of reference, which is stored elsewhere
 */
class CollisionModel {
public:
	virtual CollisionType getType() = 0;
	virtual ~CollisionModel() {}
};

class AABBModel : public CollisionModel {
public:
	virtual CollisionType getType() { return CMDL_AABB; }
	vector<Box> vBoxes;
};

class HMapModel : public CollisionModel {
public:
	HMapModel(const char *filename, const Vec3f &offset, int unitLength, float scale, DIRECTION normalDir);
	HMapModel(HMap *hmap, const Vec3f &offset, DIRECTION normalDir);
	virtual ~HMapModel();

	virtual CollisionType getType() { return CMDL_HMAP; }
	HMap *hmap;
	Box bxTotalVolume;
	DIRECTION dir;

private:
	void init(HMap *hmap, const Vec3f &offset, DIRECTION normalDir);
	bool bCreatedHMap;
};

bool areColliding(const Box &bx1, const Box &bx2);
bool areColliding(const Box &bx1, const Point_t &hmapCenter, const HMapModel &hmap);

void getCollisionInfo(Vec3f *shift, DIRECTION *collDir, const Box &bx1, const Box &bx2);
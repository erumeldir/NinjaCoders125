#include "TestSObj.h"


TestSObj::TestSObj(uint id) : ServerObject(id) {
	DC::get()->print("Created new TestSObj %d\n", id);
	pm = new PhysicsModel(Point_t(), Rot_t(), 500);

	// Initialize input status
	istat.attack = false;
	istat.jump = false;
	istat.quit = false;
	istat.specialPower = false;
	istat.rotAngle = 0.0;
	istat.xDist = 0.0;
	istat.yDist = 0.0;
}


TestSObj::~TestSObj(void) {
}

bool TestSObj::update() {
	if (istat.quit) {
		// Send Client quit event
		return true; // delete me!
	}
	if (istat.attack) {
		// Determine attack logic here
		DC::get()->print("SERVER KNOWS YOU'RE ATTACKING!!!                                               \r");
	}
	if (istat.jump) {
		DC::get()->print("SERVER KNOWS YOU'RE JUMPING!                                                   \r");
	}
	if (istat.specialPower) {
		DC::get()->print("SERVER KNOWS YOU'RE SPECIAL POWERING!!!!!                                      \r");
	}
	
	pm->ref->setRot(Rot_t(0, 0, istat.rotAngle));
	Point_t pos = pm->ref->getPos();
	pm->ref->setPos(Point_t(pos.x + istat.xDist, pos.y - istat.yDist, 0));

	return false;
}

pair<int, char*> TestSObj::serialize() {
	return pm->ref->serialize();
}
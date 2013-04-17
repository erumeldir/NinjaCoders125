#include "PlayerSObj.h"
#include "defs.h"

PlayerSObj::PlayerSObj(uint id) : ServerObject(id) {
	DC::get()->print("Created new PlayerSObj %d\n", id);
	//pm = new PhysicsModel(Point_t(-50,0,150), Rot_t(), 5);
	pm = new PhysicsModel(Point_t(0,0,0), Rot_t(), 5);

	// Initialize input status
	istat.attack = false;
	istat.jump = false;
	istat.quit = false;
	istat.specialPower = false;
	istat.rotAngle = 0.0;
	istat.rotHoriz = 0.0;
	istat.rotVert = 0.0;
	istat.xDist = 0.0;
	istat.yDist = 0.0;
}


PlayerSObj::~PlayerSObj(void) {
}

bool PlayerSObj::update() {
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
	
	Rot_t rt = pm->ref->getRot();
	//rt.z += istat.rotHoriz;
	//rt.y += istat.rotVert;
	//pm->ref->setRot(rt);
	//pm->ref->setRot(Rot_t(0, 0, istat.rotAngle));
	float yaw = rt.y + istat.rotHoriz,
		  pitch = rt.x + istat.rotVert;
	if (yaw > 6.2832 || yaw < -6.2832) yaw = 0; // TODO DEFINE then defs 
	if (pitch > 6.2832 || pitch < -6.2832) pitch = 0; // TODO DEFINE then defs 
	printf("New angle is %f,%f                                                            \r", yaw, pitch);
	pm->ref->setRot(Rot_t(0, yaw, 0));
	
	//pm->ref->setRot(Rot_t(0, istat.rotVert, istat.rotHoriz));
#define DIV 100
	//Point_t pos = pm->ref->getPos();
	//pm->ref->setPos(Point_t(pos.x + istat.xDist, pos.y - istat.yDist, 0));
	//pm->applyForce(Vec3f(istat.xDist / DIV, 0, istat.yDist / DIV));
	float rawX = istat.xDist / DIV;
	float rawY = istat.yDist / DIV;
	// TODO Check math...please xD
//	pm->applyForce(Vec3f( (rawX * cos(newAngle)) - (rawY * sin(newAngle)), 0,(rawX * sin(newAngle)) + (rawY * cos(newAngle))));
	pm->applyForce(Vec3f( (rawY * sin(yaw)) + (rawX * sin(yaw + M_PI / 2)), 0, (rawY * cos(yaw)) + (rawX * cos(yaw + M_PI / 2)) ));
	

	return false;
}

int PlayerSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = MDL_0;
	return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
}

void PlayerSObj::deserialize(char* newInput)
{
	inputstatus* newStatus = reinterpret_cast<inputstatus*>(newInput);
	istat = *newStatus;
}
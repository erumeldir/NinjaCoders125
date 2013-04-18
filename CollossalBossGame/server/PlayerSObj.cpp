#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include "defs.h"

PlayerSObj::PlayerSObj(uint id) : ServerObject(id) {
	DC::get()->print("Created new PlayerSObj %d\n", id);
	//pm = new PhysicsModel(Point_t(-50,0,150), Rot_t(), 5);
	pm = new PhysicsModel(Point_t(0,0,0), Rot_t(), CM::get()->find_config_as_int("PLAYER_MASS"));

	// Initialize input status
	istat.attack = false;
	istat.jump = false;
	istat.quit = false;
	istat.specialPower = false;
	istat.rotAngle = 0.0;
	istat.rotHoriz = 0.0;
	istat.rotVert = 0.0;
	istat.rightDist = 0.0;
	istat.forwardDist = 0.0;
}


PlayerSObj::~PlayerSObj(void) {
}

bool PlayerSObj::update() {
	float yDist = 0.f;
	if (istat.quit) {
		// todo Send Client quit event
		return true; // delete me!
	}
	if (istat.attack) {
		// Determine attack logic here
	}
	if (istat.jump && pm->onGround) {
		yDist = CM::get()->find_config_as_int("JUMP_DIST");
	}
	if (istat.specialPower) {
		// Determine special power logic here
	}
	
	Rot_t rt = pm->ref->getRot();
	float yaw = rt.y + istat.rotHoriz,
		  pitch = rt.x + istat.rotVert;
	if (yaw > M_TAU || yaw < -M_TAU) yaw = 0;
	if (pitch > M_TAU || pitch < -M_TAU) pitch = 0;
	pm->ref->setRot(Rot_t(0, yaw, 0));
	
	int divBy = CM::get()->find_config_as_int("MOV_DAMP");
	float rawRight = istat.rightDist / divBy;
	float rawForward = istat.forwardDist / divBy;

	pm->applyForce(Vec3f( (rawForward * sin(yaw)) + (rawRight * sin(yaw + M_PI / 2)), yDist, (rawForward * cos(yaw)) + (rawRight * cos(yaw + M_PI / 2)) ));	

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
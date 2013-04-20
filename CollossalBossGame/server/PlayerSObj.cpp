#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include "WallSObj.h"
#include "defs.h"

PlayerSObj::PlayerSObj(uint id) : ServerObject(id) {
	// Configuration options
	jumpDist = CM::get()->find_config_as_float("JUMP_DIST");
	movDamp = CM::get()->find_config_as_int("MOV_DAMP");

	Point_t pos = Point_t();
	Box bxVol = CM::get()->find_config_as_box("BOX_CUBE");//Box(-10, 0, -10, 20, 20, 20);

	DC::get()->print("Created new PlayerSObj %d\n", id);
	//pm = new PhysicsModel(Point_t(-50,0,150), Rot_t(), 5);
	pm = new PhysicsModel(pos, Rot_t(), CM::get()->find_config_as_float("PLAYER_MASS"), bxVol);
	this->health = CM::get()->find_config_as_int("INIT_HEALTH");
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
	
	if(this->health > 0)
	{
		if (istat.attack) {
			// Determine attack logic here
		}
		if (istat.jump && pm->onGround) {
			yDist = jumpDist;
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
	
		int divBy = movDamp;
		float rawRight = istat.rightDist / divBy;
		float rawForward = istat.forwardDist / divBy;
		float computedRight = ((rawForward * sin(yaw)) + (rawRight * sin(yaw + M_PI / 2.f)));
		float computedForward = ((rawForward * cos(yaw)) + (rawRight * cos(yaw + M_PI / 2.f)));
		pm->applyForce(Vec3f(computedRight, yDist, computedForward));	
	}
	return false;
}

int PlayerSObj::serialize(char * buf) {
	PlayerState *state = (PlayerState*)buf;
	state->modelNum = MDL_0;
	state->health = health;
	return pm->ref->serialize(buf + sizeof(PlayerState)) + sizeof(PlayerState);
}

void PlayerSObj::deserialize(char* newInput)
{
	inputstatus* newStatus = reinterpret_cast<inputstatus*>(newInput);
	istat = *newStatus;
}

void PlayerSObj::onCollision(ServerObject *obj) {
	DC::get()->print("Collided with obj %d\n", obj->getId());
	if(obj->getFlag(IS_HARMFUL))
		this->health--;
	if(obj->getFlag(IS_HEALTHY))
		this->health++;
	if(this->health < 0) health = 0;
	if(this->health > 100) health = 100;


	/**  http://bobobobo.wordpress.com/tag/computer-graphics/
	  */
	if(obj->getFlag(IS_WALL)) //&& istat.jump)
	{
		//jump!
		WallSObj *wall  = reinterpret_cast<WallSObj *>(obj);
		float bounceDamp = 3.f;

		Vec3f incident = this->pm->vel;
		Vec3f normal = wall->getNormal();
		Vec3f reflected = incident - (((normal - incident) * normal) * 2.f);
		// http://www.3dkingdoms.com/weekly/weekly.php?a=2
		pm->vel = (normal * (((incident ^ normal) * -2.f )) + incident) * bounceDamp;
	//	pm->vel = reflected;
	//	pm->applyForce(normal * 5);
	}
}

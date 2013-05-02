#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "WorldManager.h"
#include "WallSObj.h"
#include "defs.h"
#include "PhysicsEngine.h"

PlayerSObj::PlayerSObj(uint id) : ServerObject(id) {
	// Configuration options
	jumpDist = CM::get()->find_config_as_float("JUMP_DIST");
	movDamp = CM::get()->find_config_as_int("MOV_DAMP");


	if(SOM::get()->debugFlag) DC::get()->print("Reinitialized PlayerSObj %d\n", this->getId());

	Point_t pos = Point_t(0, 5, 10);
	Box bxVol = CM::get()->find_config_as_box("BOX_CUBE");//Box(-10, 0, -10, 20, 20, 20);

	pm = new PhysicsModel(pos, Quat_t(), CM::get()->find_config_as_float("PLAYER_MASS"));
	pm->addBox(bxVol);

	lastCollision = pos;
	this->health = CM::get()->find_config_as_int("INIT_HEALTH");

	// Initialize input status
	istat.attack = false;
	istat.jump = false;
	istat.quit = false;
	istat.start = false;
	istat.specialPower = false;
	istat.rotAngle = 0.0;
	istat.rotHoriz = 0.0;
	istat.rotVert = 0.0;
	istat.rightDist = 0.0;
	istat.forwardDist = 0.0;

	newJump = true; // any jump at this point is a new jump
	newAttack = true; // same here
	appliedJumpForce = false;

	lastGravDir = PE::get()->getGravDir();
	t = 1;
	tRate = CM::get()->find_config_as_float("GRAVITY_SWITCH_CAMERA_SPEED");
	yawRot = Quat_t();
	initUpRot = Quat_t();
	finalUpRot = Quat_t();

	bool firedeath = false;
	attacking = false;
	gravityTimer = 0;
	modelAnimationState = IDLE;
	ready = false;
}

void PlayerSObj::initialize() {
	// Configuration options
	jumpDist = CM::get()->find_config_as_float("JUMP_DIST");
	movDamp = CM::get()->find_config_as_int("MOV_DAMP");


	if(SOM::get()->debugFlag) DC::get()->print("Created new PlayerSObj %d\n", this->getId());

	Point_t pos = Point_t(0, 5, 10);
	Box bxVol = CM::get()->find_config_as_box("BOX_CUBE");//Box(-10, 0, -10, 20, 20, 20);

	delete pm;

	pm = new PhysicsModel(pos, Quat_t(), CM::get()->find_config_as_float("PLAYER_MASS"));
	pm->addBox(bxVol);
	lastCollision = pos;
	this->health = CM::get()->find_config_as_int("INIT_HEALTH");
	// Initialize input status
	istat.attack = false;
	istat.jump = false;
	istat.quit = false;
	istat.start = false;
	istat.specialPower = false;
	istat.rotAngle = 0.0;
	istat.rotHoriz = 0.0;
	istat.rotVert = 0.0;
	istat.rightDist = 0.0;
	istat.forwardDist = 0.0;

	newJump = true; // any jump at this point is a new jump
	newAttack = true; // same here
	appliedJumpForce = false;
	bool firedeath = false;
	attacking = false;
	gravityTimer = 0;
	ready = false;

	lastGravDir = PE::get()->getGravDir();
	t = 1;
	tRate = CM::get()->find_config_as_float("GRAVITY_SWITCH_CAMERA_SPEED");
	yawRot = Quat_t();
	initUpRot = Quat_t();
	finalUpRot = Quat_t();
}

PlayerSObj::~PlayerSObj(void) {
	delete pm;
}


bool PlayerSObj::update() {

	if (istat.start && !ready) {
		ready = true; // delete me!
	}

	if (istat.quit) {
		return true; // delete me!
	}
	
	if(this->health > 0)
	{
		firedeath = false;

		if(istat.attack && newAttack) attacking = true;
		newAttack = !istat.attack;

		//if (attacking) attackCounter++;
		//else attackCounter = 0;

		// Jumping can happen in two cases
		// 1. Collisions
		// 2. In the air
		// This handles in the air, collisions
		// are handled in OnCollision()

		// This part discretizes jumps (so no button mashing)
		jumping = istat.jump && newJump; // isFalling?
		newJump = !istat.jump;

		// This part gives us a buffer, so the user can bounce off the wall even 
		// when they pressed 'jump' before they got there
		if (jumping) jumpCounter++;
		else jumpCounter = 0; 

		appliedJumpForce = false; // we apply it on collision

		if (istat.specialPower) {
			// Determine special power logic here
			pm->ref->setPos(Vec3f()); // your special power is to return to the origin
		}

		//Update up direction
		PE *pe = PE::get();
		if(lastGravDir != pe->getGravDir()) {
			lastGravDir = pe->getGravDir();
			//pm->ref->rotate(pe->getCurGravRot());
			slerp(&initUpRot, initUpRot, finalUpRot, t);	//We may not have finished rotating
			finalUpRot = pe->getCurGravRot();
			t = 0;
		}

		//Vec3f up = rotateUp(pm->ref->getRot());

		//Rotate by amount specified by player (does not affect up direction)
		Quat_t upRot;
		slerp(&upRot, initUpRot, finalUpRot, t);
		if(t < 1.0f) {
			t += tRate;
		}

		//Update the yaw rotation of the player (about the default up vector)
		yawRot *= Quat_t(Vec3f(0,1,0), istat.rotHoriz);

		Quat_t qRot = upRot * yawRot;
		pm->ref->setRot(qRot);

		//Move the player: apply a force in the appropriate direction
		float rawRight = istat.rightDist / movDamp;
		float rawForward = istat.forwardDist / movDamp;
		Vec3f total = rotate(Vec3f(rawRight, 0, rawForward), qRot);
		
		pm->applyForce(total);
		
		// change animation according to state
		if(pm->vel.x <= 0.25 && pm->vel.x >= -0.25 && pm->vel.z <= 0.25 && pm->vel.z >= -0.25) {
			this->setAnimationState(IDLE);
		} else {
			this->setAnimationState(WALK);
		}
	} else {
		// TODO Franklin: THE PLAYER IS DEAD. WHAT DO?
		// NOTE: Player should probably be also getting their client id.
		if(!firedeath) {
			firedeath = true;
			EventManager::get()->fireEvent(EVENT_PLAYER_DEATH, this); 
		}
	}
	return false;
}

int PlayerSObj::serialize(char * buf) {
	PlayerState *state = (PlayerState*)buf;
	state->modelNum = MDL_PLAYER;
	state->health = health;
	state->ready = ready;
	if (SOM::get()->debugFlag) DC::get()->print("CURRENT MODEL STATE %d\n",this->modelAnimationState);
	state->animationstate = this->modelAnimationState;
	return pm->ref->serialize(buf + sizeof(PlayerState)) + sizeof(PlayerState);
}

void PlayerSObj::deserialize(char* newInput)
{
	inputstatus* newStatus = reinterpret_cast<inputstatus*>(newInput);
	istat = *newStatus;
	if (istat.start) {
		EventManager::get()->fireEvent(EVENT_RESET, this); 
	}
}

void PlayerSObj::onCollision(ServerObject *obj, const Vec3f &collNorm) {
	if(obj->getFlag(IS_HARMFUL) && !(attacking))
		this->health-=3;
	if(obj->getFlag(IS_HEALTHY))
		this->health++;
	if(this->health < 0) health = 0;
	if(this->health > 100) health = 100;

	
	// If I started jumping a little bit ago, that's a jump
	// appliedJumpForce is because OnCollision gets called twice
	// on every collision, so this makes sure you only apply the
	// jump force once every iteration
	if(!appliedJumpForce && (jumpCounter > 0 && jumpCounter < 10))
	{
		// surface bouncing
		// Get the collNorm from the surface
		float bounceDamp = 0.05f;

		Vec3f incident = pm->ref->getPos() - lastCollision;

		// incident is zero, so we just jump upwards
		// this happens when you jump of the same surface
		// you were at before (so the floor, or when you
		// slide off the wall and then jump)
		if ((incident.x < .01 && incident.x > -.01)
			|| (incident.y < .01 && incident.y > -.01)
			|| (incident.z < .01 && incident.z > -.01))
		{
			Vec3f force = (PE::get()->getGravVec() * -1) + collNorm;
			pm->vel = Vec3f();
			pm->applyForce(force*jumpDist);
		}
		// we have incident! so we bounce
		else
		{
			// http://www.3dkingdoms.com/weekly/weekly.php?a=2
			// optimize: *= ^= better!
			pm->vel = (collNorm * (((incident ^ collNorm) * -2.f )) + incident) * bounceDamp;
		}

		appliedJumpForce = true;
	}

	// Set last collision pos for bouncing off different surfaces
	lastCollision = pm->ref->getPos();
	jumping = false;
}

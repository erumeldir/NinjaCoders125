#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "WorldManager.h"
#include "WallSObj.h"
#include "defs.h"
#include "PhysicsEngine.h"

PlayerSObj::PlayerSObj(uint id) : ServerObject(id) {
	// Set all your pointers to NULL here, so initialize()
	// knows if it should create them or not
	pm = NULL;
	this->initialize();
}

void PlayerSObj::initialize() {
	// Configuration options
	jumpDist = CM::get()->find_config_as_float("JUMP_DIST");
	movDamp = CM::get()->find_config_as_int("MOV_DAMP");
	chargeForce = CM::get()->find_config_as_float("CHARGE_FORCE");
	swordDamage = CM::get()->find_config_as_int("SWORD_DAMAGE");
	chargeDamage = CM::get()->find_config_as_int("CHARGE_DAMAGE");
	chargeUpdate = CM::get()->find_config_as_float("CHARGE_UPDATE");

	if(SOM::get()->debugFlag) DC::get()->print("Initialized new PlayerSObj %d\n", this->getId());

	Point_t pos = Point_t(0, 5, 10);
	Box bxVol = CM::get()->find_config_as_box("BOX_CUBE");//Box(-10, 0, -10, 20, 20, 20);

	if (pm != NULL) delete pm;
	pm = new PhysicsModel(pos, Rot_t(), CM::get()->find_config_as_float("PLAYER_MASS"));
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

	// Avoids button holding to keep applying ability
	newJump = true; // any jump at this point is a new jump
	newAttack = true; // same here
	newCharge = true; // yes yes, we get the idea

	appliedJumpForce = false;
	firedeath = false;
	attacking = false;
	gravityTimer = 0;
	charging = false;
	charge = 0.0;
	damage = 0;
	modelAnimationState = IDLE;
	ready = false;
}

PlayerSObj::~PlayerSObj(void) {
	delete pm;
}

bool PlayerSObj::update() {
	//gravity
#if 0
	gravityTimer++;
	static char cdir = 'v';
	if(gravityTimer == 500) {
		PE::get()->setGravDir(DOWN);
		cdir = 'o';
	} else if(gravityTimer == 1000) {
		PE::get()->setGravDir(NONE);
		cdir = '>';
	} else if(gravityTimer > 1500) {
		PE::get()->setGravDir(EAST);
		gravityTimer = 0;
		cdir = 'v';
	}
	DC::get()->print(CONSOLE, "%c Gravity timer = %d     \r", cdir, gravityTimer);
#endif
	if (istat.start && !ready) {
		ready = true;
	}

	float yDist = 0.f;
	if (istat.quit) {
		return true; // delete me!
	}
	
	if(this->health > 0)
	{
		firedeath = false;

		attacking = istat.attack && newAttack;
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

		if (istat.specialPower && !getFlag(IS_FALLING)) // holding down increases the charge
		{
			charge+=chargeUpdate;
			if(charge > 13) charge = 13.f;
		}
		else
		{
			// If we accumulated some charge, fire!
			if (charge > 0.f)
			{
				Vec3f up = (PE::get()->getGravVec() * -1);
				pm->applyForce(up * (chargeForce * charge));
				charging = true;
			}

			charge = 0.f;
		}

		/*if (istat.specialPower && newCharge && !getFlag(IS_FALLING)) {
			// CHARGE!!!
			// todo for now up, should be forward (or up + forward?)
			Vec3f up = (PE::get()->getGravVec() * -1);
			pm->applyForce(up * chargeForce);
			charging = true;
		}
		newCharge = !istat.specialPower;*/

		damage = charging ? chargeDamage : attacking ? swordDamage : 0;
#if 1
		Rot_t rt = pm->ref->getRot();
		float yaw = rt.y + istat.rotHoriz,
			  pitch = rt.x + istat.rotVert;
		if (yaw > M_TAU || yaw < -M_TAU) yaw = 0;
		if (pitch > M_TAU || pitch < -M_TAU) pitch = 0;
		pm->ref->setRot(Rot_t(0, yaw, 0));
#else
		float yaw;
		if(istat.forwardDist > 0) {
			yaw = istat.rotAngle;
			pm->ref->setRot(Rot_t(0, yaw, 0));
		} else {
			yaw = pm->ref->getRot().y;
		}
#endif
	
		int divBy = movDamp;
		float rawRight = istat.rightDist / divBy;
		float rawForward = istat.forwardDist / divBy;
		float computedRight = ((rawForward * sin(yaw)) + (rawRight * sin(yaw + (float)M_PI / 2.f)));
		float computedForward = ((rawForward * cos(yaw)) + (rawRight * cos(yaw + (float)M_PI / 2.f)));
		pm->applyForce(Vec3f(computedRight, yDist, computedForward));	
		
		// change animation according to state
		if(pm->vel.x <= 0.25 && pm->vel.x >= -0.25 && pm->vel.z <= 0.25 && pm->vel.z >= -0.25) {
			this->setAnimationState(IDLE);
		} else {
			this->setAnimationState(WALK);
		}
	} else {
		damage = 0; // you can't kill things if you're dead xD

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
	state->charge = charge;
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
	charging = false;
}

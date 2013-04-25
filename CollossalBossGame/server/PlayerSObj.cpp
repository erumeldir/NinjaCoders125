#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "WallSObj.h"
#include "defs.h"
#include "PhysicsEngine.h"

PlayerSObj::PlayerSObj(uint id) : ServerObject(id) {
	// Configuration options
	jumpDist = CM::get()->find_config_as_float("JUMP_DIST");
	movDamp = CM::get()->find_config_as_int("MOV_DAMP");


	if(SOM::get()->debugFlag) DC::get()->print("Created new PlayerSObj %d\n", id);

	Point_t pos = Point_t(0, 5, 10);
	Box bxVol = CM::get()->find_config_as_box("BOX_CUBE");//Box(-10, 0, -10, 20, 20, 20);

	//pm = new PhysicsModel(Point_t(-50,0,150), Rot_t(), 5);
	pm = new PhysicsModel(pos, Rot_t(), CM::get()->find_config_as_float("PLAYER_MASS"), bxVol);
	lastCollision = pos;
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

	newJump = true; // any jump at this point is a new jump
	newAttack = true; // same here
	appliedJumpForce = false;
	attacking = false;
	gravityTimer = 0;
	modelAnimationState = IDLE;
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


	float yDist = 0.f;
	if (istat.quit) {
		return true; // delete me!
	}
	
	if(this->health > 0)
	{
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
		float computedRight = ((rawForward * sin(yaw)) + (rawRight * sin(yaw + M_PI / 2.f)));
		float computedForward = ((rawForward * cos(yaw)) + (rawRight * cos(yaw + M_PI / 2.f)));
		/*
		uint dir = PE::get()->getGravDir();
		switch(dir) {
		case EAST:
		case WEST:
			pm->applyForce(Vec3f(yDist, computedForward, computedRight));
			break;
		case NORTH:
		case SOUTH:
			pm->applyForce(Vec3f(computedRight, computedForward, yDist));
			break;
		case UP:
		case DOWN:
		default:
			pm->applyForce(Vec3f(computedRight, yDist, computedForward));
			break;
		}
		*/
		pm->applyForce(Vec3f(computedRight, yDist, computedForward));
		if(pm->vel.x <= 0.25 && pm->vel.x >= -0.25 && pm->vel.z <= 0.25 && pm->vel.z >= -0.25) {
			this->setAnimationState(IDLE);
		} else {
			this->setAnimationState(WALK);
		}
	}
	return false;
}

int PlayerSObj::serialize(char * buf) {
	PlayerState *state = (PlayerState*)buf;
	state->modelNum = MDL_1;
	state->health = health;
	DC::get()->print("CURRENT MODEL STATE %d\n",this->modelAnimationState);
	state->animationstate = this->modelAnimationState;
	return pm->ref->serialize(buf + sizeof(PlayerState)) + sizeof(PlayerState);
}

void PlayerSObj::deserialize(char* newInput)
{
	inputstatus* newStatus = reinterpret_cast<inputstatus*>(newInput);
	istat = *newStatus;
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

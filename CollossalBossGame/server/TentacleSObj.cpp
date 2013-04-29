#include "TentacleSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include <time.h>

TentacleSObj::TentacleSObj(uint id, Model modelNum, Point_t pos, Rot_t rot, MonsterSObj* master) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new TentacleObj %d\n", id);
	overlord = master;
	overlord->addTentacle(this);
	Box bxVol = CM::get()->find_config_as_box("BOX_MONSTER");
	this->modelNum = modelNum;
	this->health = CM::get()->find_config_as_int("INIT_HEALTH");
	pm = new PhysicsModel(pos, rot, 50*CM::get()->find_config_as_float("PLAYER_MASS"));
	pm->addBox(bxVol);
	//this->updatableBoxIndex = pm->addBox(updatableBox);
	attackCounter = 0;
	this->setFlag(IS_STATIC, 1);
	modelAnimationState = T_IDLE;

	srand((uint)time(NULL)); // initialize our random number generator

	// Configuration options
	attackBuffer = CM::get()->find_config_as_int("TENTACLE_ATTACK_BUF");
	attackFrames = CM::get()->find_config_as_int("TENTACLE_ATTACK_FRAMES");
	pushForce = CM::get()->find_config_as_int("TENTACLE_PUSH_FORCE");
}


TentacleSObj::~TentacleSObj(void)
{
	delete pm;
}

bool TentacleSObj::update() {
	//changing collision boxes
	//updatableBox.y = -updatableBox.y;
	//pm->updateBox(this->updatableBoxIndex,this->updatableBox);

	attackCounter++;

	// this emulates an attack

	// start attacking!
	if (attackCounter > attackBuffer){
		this->setFlag(IS_HARMFUL, 1);
		modelAnimationState = T_SWEEP;
	}

	/* Cycle logic:
	 * CYCLE*1/2 = The tentacle is on one side
	 * CYCLE*3/2 = tentacle is on the other side
	 * CYCLE*2 = when the tentacle is back at the default position
	 */

	if (modelAnimationState == T_SWEEP)
	{
		switch (attackCounter - attackBuffer)
		{
			case CYCLE/2:		//we flip direction
				sweepingZPositive = false;
				break;
			case CYCLE/2 * 3:	//reversing direction the other way
				sweepingZPositive = true;
				break;
			case CYCLE*2:		//we're ending
				attackCounter = 0;
				this->setFlag(IS_HARMFUL, 0);
				attackBuffer = rand() % 40;
				attackFrames = rand() % 15;
				modelAnimationState = T_IDLE;
				break;
			default:			//we're sweeping normally
				Box updatedTip = this->getPhysicsModel()->colBoxes[2];
				Box updatedMiddle = this->getPhysicsModel()->colBoxes[1];
				int newX = updatedMiddle.x, newZ = updatedMiddle.z;
				if (attackCounter - attackBuffer > CYCLE/2 &&
					attackCounter - attackBuffer < CYCLE/2 * 3) {
					newZ = updatedMiddle.z - 2;
					updatedTip.x = updatedTip.x - 3;
				} else {
					newZ = updatedMiddle.z + 2;
					updatedTip.x = updatedTip.x + 3;
				}
				
				if (!sweepingZPositive) {
					newX = updatedMiddle.x - 7;
					updatedTip.x = updatedTip.x - 10;
				}
				else {
					newX = updatedMiddle.x + 7;
					updatedTip.x = updatedTip.x + 10;
				}	
				updatedMiddle = Box (newX, updatedMiddle.y, newZ, updatedMiddle.w,updatedMiddle.h, updatedMiddle.l);

				this->getPhysicsModel()->updateBox(1, updatedMiddle);

		}

	}

	if (health <= 0) {
		health = 0;
		overlord->removeTentacle(this);
		return true; // I died!
		//health = 0;
		// fancy animation 
		// just dont attack
		//attackBuffer = 0;
		//attackFrames = 0;
	}

		return false;
}

int TentacleSObj::serialize(char * buf) {
	TentacleState *state = (TentacleState*)buf;
	state->modelNum = this->modelNum;
	//state->health = health;
	state->animationState = this->modelAnimationState;
	return pm->ref->serialize(buf + sizeof(TentacleState)) + sizeof(TentacleState);
}

void TentacleSObj::onCollision(ServerObject *obj, const Vec3f &collisionNormal) {
	// if I collided against the player, AND they're attacking me, loose health
	string s = typeid(*obj).name();

	// if the monster is attacking, it pushes everything off it on the last attack frame
	if (attackCounter == (attackBuffer + attackFrames))
	{
		Vec3f up = Vec3f(0, 1, 0);
		obj->getPhysicsModel()->applyForce((up + collisionNormal)*(float)pushForce);
	}

	if(!s.compare("class PlayerSObj")) 
	{	
		PlayerSObj* player = reinterpret_cast<PlayerSObj*>(obj);
		if(player->attacking && player->getHealth() > 0) 
		{
			health-=3;
			player->attacking = false;
		}
		if(this->health < 0) health = 0;
		if(this->health > 100) health = 100;
	}
}
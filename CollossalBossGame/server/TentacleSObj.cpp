#include "TentacleSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "defs.h"
#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include "PhysicsEngine.h"
#include <time.h>
#include <random>

TentacleSObj::TentacleSObj(uint id, Model modelNum, Point_t pos, Quat_t rot, MonsterSObj* master) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new TentacleObj %d\n", id);
	overlord = master;
	overlord->addTentacle(this);
	Box bxVol = CM::get()->find_config_as_box("BOX_MONSTER");
	this->modelNum = modelNum;
	this->health = CM::get()->find_config_as_int("INIT_HEALTH");
	pm = new PhysicsModel(pos, rot, 50*CM::get()->find_config_as_float("PLAYER_MASS"));
	pm->addBox(bxVol);
	//this does not take rotation into account. Hopefully that doesn't matter?
	if (rot.x == 0 && rot.y == 0 && rot.z == 0)
	{
		pm->updateBox(0, *(new Box(-10, 10, 0, 20, 20, 50)));
		pm->addBox(*(new Box(-10, -10, -50, 20, 20, 150)));
		pm->addBox(*(new Box(-10, -10, -200, 20, 20, 105)));
	} else {
		pm->updateBox(0, *(new Box(-10, -10, 0, 20, 20, 50)));
		pm->addBox(*(new Box(-10, -10, 50, 20, 20, 150)));
		pm->addBox(*(new Box(-10, -10, 200, 20, 20, 105)));

	}
	//this->updatableBoxIndex = pm->addBox(updatableBox);
	
	this->setFlag(IS_STATIC, 1);
	modelAnimationState = T_IDLE;
	
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1,50);
	attackCounter = distribution(generator);
	
	//srand((uint)time(NULL)); // initialize our random number generator

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
	if (attackCounter > attackBuffer && !(attackCounter % CYCLE)){
		this->setFlag(IS_HARMFUL, 1);
		modelAnimationState = T_SWEEP;
	}

	/* Cycle logic:
	 * CYCLE*1/2 = The tentacle is on one side
	 * CYCLE*3/2 = tentacle is on the other side
	 * CYCLE*2 = when the tentacle is back at the default position
	 */

	if (modelAnimationState != T_IDLE)
	{
		Box middle = this->getPhysicsModel()->colBoxes.at(1);
		Box tip = this->getPhysicsModel()->colBoxes.at(2);
		Vec3f pos;
		/*if (attackCounter - attackBuffer < 13) {
			middle.z = middle.z - 4;
			tip.z = tip.z - 10;
			middle.x = middle.x + 4;
			tip.x = tip.x + 10;
		} else if (attackCounter - attackBuffer < 23) {
			middle.z = middle.z + 4;
			tip.z = tip.z + 10;
			middle.x = middle.x - 4;
			tip.x = tip.x - 10;
		}else if (attackCounter - attackBuffer < 35) {
			middle.z = middle.z - 4;
			tip.z = tip.z - 10;
			middle.x = middle.x + 4;
			tip.x = tip.x + 10;
		}else if (attackCounter - attackBuffer < 50) {
			middle.z = middle.z + 4;
			tip.z = tip.z + 10;
			middle.x = middle.x - 4;
			tip.x = tip.x - 10;
		} else {
			attackCounter = 0;
			this->setFlag(IS_HARMFUL, 0);
			attackBuffer = rand() % 40;
			attackFrames = rand() % 15;
			modelAnimationState = T_IDLE;
		}
		*/
		this->getPhysicsModel()->updateBox(1, middle);
		this->getPhysicsModel()->updateBox(2, tip);
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

	if (SOM::get()->collisionMode)
	{
		CollisionState *collState = (CollisionState*)(buf + sizeof(TentacleState));

		vector<Box> objBoxes = pm->colBoxes;

		collState->totalBoxes = min(objBoxes.size(), maxBoxes);

		for (int i=0; i<collState->totalBoxes; i++)
		{
			collState->boxes[i] = objBoxes[i] + pm->ref->getPos(); // copying applyPhysics
		}

		return pm->ref->serialize(buf + sizeof(TentacleState) + sizeof(CollisionState)) + sizeof(TentacleState) + sizeof(CollisionState);
	}
	else
	{
		return pm->ref->serialize(buf + sizeof(TentacleState)) + sizeof(TentacleState);
	}
}

void TentacleSObj::onCollision(ServerObject *obj, const Vec3f &collisionNormal) {
	// if I collided against the player, AND they're attacking me, loose health
	string s = typeid(*obj).name();

	// if the monster is attacking, it pushes everything off it on the last attack frame
	if (attackCounter == (attackBuffer + attackFrames))
	{
		Vec3f up = (PE::get()->getGravVec() * -1);
		obj->getPhysicsModel()->applyForce((up + collisionNormal)*(float)pushForce);
	}

	if(!s.compare("class PlayerSObj")) 
	{	
		PlayerSObj* player = reinterpret_cast<PlayerSObj*>(obj);
		//if(player->attacking && player->getHealth() > 0) 
		//{
			health-= player->damage;
		//	player->attacking = false;
		//}
		if(this->health < 0) health = 0;
		if(this->health > 100) health = 100;
	}
}
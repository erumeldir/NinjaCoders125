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
		pm->updateBox(0, *(	new Box(-10, -10,	-50,  30, 30,	130)));
		pm->addBox(*(		new Box(-10,  0,	-180, 30, 60,	150)));
		pm->addBox(*(		new Box(-10, -10,	-230, 20, 20,	50)));
	} else {
		pm->updateBox(0, *(	new Box(-10, 10,	50,   30, 30,	130)));
		pm->addBox(*(		new Box(-10,  0,	180,  30, 60,	150)));
		pm->addBox(*(		new Box(-10, -10,	230, 20, 20,	50)));

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
	
	if (attackCounter > attackBuffer && !( (attackCounter - attackBuffer) % CYCLE)){
		if (this->getFlag(IS_HARMFUL))
		{
			this->setFlag(IS_HARMFUL, 0);
			attackBuffer = rand() % 40;
			attackFrames = - rand() % 15;
			modelAnimationState = T_IDLE;
		} else {
			float angle = this->angleToNearestPlayer();
			if (angle != -1.f)
			{
				Vec3f axis = Vec3f(0,0,1);
				Vec4f qAngle = Vec4f(axis,angle);
				this->getPhysicsModel()->ref->rotate(qAngle);
				this->setFlag(IS_HARMFUL, 1);
				modelAnimationState = T_SLAM;
			}
		}
	}
	
	/* Cycle logic:
	 * CYCLE*1/2 = The tentacle is extended
	 * CYCLE = when the tentacle is back at the default position
	 */
	Box base = this->getPhysicsModel()->colBoxes.at(0);
	Box middle = this->getPhysicsModel()->colBoxes.at(1);
	Box tip = this->getPhysicsModel()->colBoxes.at(2);
	Vec3f changePosT = Vec3f(), changeProportionT = Vec3f();
	Vec3f changePosM = Vec3f(), changeProportionM = Vec3f();

	if (modelAnimationState == T_IDLE)
	{
		if ((attackCounter - attackBuffer)%CYCLE < CYCLE/2) {
			int i;
			/*middle.h = middle.h + 4;
			middle.y = middle.y + 4;*/
		} else if ((attackCounter - attackBuffer)%CYCLE < CYCLE){
			/*middle.h = middle.h - 4;
			middle.y = middle.y - 4;*/
		}

	} else {
		
		if ((attackCounter - attackBuffer)%CYCLE < CYCLE/2) {
			//changing the tip
			if ((attackCounter - attackBuffer)%CYCLE < CYCLE/4)
			{
				//decrease y
				changePosT.y -= 7;
				//increase height (h)
				changeProportionT.y += 3;
			} else {
				//increase y
				changePosT.y += 7;
				//decrease height (h)
				changeProportionT.y -= 3;
			}
			//decrease depth (l)
			changeProportionT.z  -= 1;
			//increase z
			changePosT.z += 6;

			changeProportionM.y += 2;
			changePosM.y -= 2;
			//tip.z = tip.z + 6;
			//middle.l = middle.l - 2;
		} else if ((attackCounter - attackBuffer)%CYCLE < CYCLE) {
			//changing the tip
			if ((attackCounter - attackBuffer)%CYCLE < 3*CYCLE/4)
			{
				//decrease y
				changePosT.y -= 7;
				//increase height (h)
				changeProportionT.y += 3;
			} else {
				//increase y
				changePosT.y += 7;
				//decrease height (h)
				changeProportionT.y -= 3;
			}
			//increase z
			changePosT.z -= 6;
			//increase depth (l, associated with z)
			changeProportionT.x += 1;
			
			changeProportionM.y -= 2;
			changePosM.y += 2;
			//tip.z = tip.z - 6;
			//middle.l = middle.l + 2;
		} 
	}
	
	//get the actual axis
	Vec4f axis = this->getPhysicsModel()->ref->getRot();

	changePosT = axis.rotateToThisAxis(changePosT);
	changeProportionT = axis.rotateToThisAxis(changeProportionT);
	changePosM = axis.rotateToThisAxis(changePosM);
	changeProportionM = axis.rotateToThisAxis(changeProportionM);
	
	tip.x += changePosT.x;
	tip.y += changePosT.y;
	tip.z += changePosT.z;

	tip.w += changeProportionT.x;
	tip.h += changeProportionT.y;
	tip.l += changeProportionT.z;

	middle.x += changePosM.x;
	middle.y += changePosM.y;
	middle.z += changePosM.z;

	middle.w += changeProportionM.x;
	middle.h += changeProportionM.y;
	middle.l += changeProportionM.z;
	
	this->getPhysicsModel()->updateBox(0, base);
	this->getPhysicsModel()->updateBox(1, middle);
	this->getPhysicsModel()->updateBox(2, tip);
	
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

/**
 * Checks if there's a player we can smash, if so
 * returns the angle we need to roll before we attack.
 * If no player is within range, we return -1.
 * Author: Haro
 */
float TentacleSObj::angleToNearestPlayer()
{
	float angle = -1.f;

	// Find player with minimum distance to me
	vector<ServerObject *> players;
	SOM::get()->findObjects(OBJ_PLAYER, &players);

	#define TENTACLE_LENGTH 300

	float minDist = TENTACLE_LENGTH;
	float currDist;
	Vec3f difference;

	for(vector<ServerObject *>::iterator it = players.begin(); it != players.end(); ++it) {
		difference = (*it)->getPhysicsModel()->ref->getPos() - this->getPhysicsModel()->ref->getPos();
		currDist = magnitude(difference);
		if (currDist < minDist) {
			minDist = currDist;
		}
	}

	if (minDist < TENTACLE_LENGTH)
	{
		// ignoring z... this is with respect to the y axis (since the tentacle smashes DOWN)
		angle = atan2(difference.x, -1*difference.y);
		//DC::get()->print("CAN I SMASH A PLAYER?.....YES!!! angle is: %f          \r", angle*180/M_PI);
	}
	/*else
	{
		DC::get()->print("CAN I SMASH A PLAYER?.....NO....                       \r");
	}*/

	return angle;
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
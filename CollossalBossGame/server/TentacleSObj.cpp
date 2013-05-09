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

	/////////////// Collision Boxes /////////////////

	// This only works for the north and south walls
	// but adding more walls should be pretty easy =D
	// just modify this axis (add more if/else)
	//Vec3f axis;
	//if (rot.x == 0 && rot.y == 0 && rot.z == 0)
	//{
	//	axis = Vec3f(0, 0, -1);
	//}
	//else if (rot.x == 1 && rot.y == 0 && rot.z == 0)
	//{
	//	axis = Vec3f(0, 0, 1);
	//}

	/*boxDims[0] = CM::get()->find_config_as_point("DIM_MONSTER1"); 
	boxDims[1] = CM::get()->find_config_as_point("DIM_MONSTER2");
	boxDims[2] = CM::get()->find_config_as_point("DIM_MONSTER3");*/

	idleBoxes[0] = CM::get()->find_config_as_box("BOX_TENT_BASE"); 
	idleBoxes[1] = CM::get()->find_config_as_box("BOX_TENT_MID");
	idleBoxes[2] = CM::get()->find_config_as_box("BOX_TENT_TIP");

	for (int i=0; i<3; i++) {
		assert(pm->addBox(idleBoxes[i]) == i && "Your physics model is out of sync with the rest of the world...");
	}

	// Add your first box
	//Vec3f initPos = CM::get()->find_config_as_point("INIT_TENTACLE_POS");

	//Box boxes[3];
	//boxes[0] = Box(initPos.x, initPos.y, initPos.z, boxDims[0].x, boxDims[0].y, boxDims[0].z);
	//pm->addBox(boxes[0]);
	//
	//// Now add the rest!
	//for (int i=1; i<3; i++)
	//{
	//	boxes[i] = Box(	boxes[i-1].x + (axis.x*boxes[i-1].w), 
	//					boxes[i-1].y + (axis.y*boxes[i-1].h), 
	//					boxes[i-1].z + (axis.z*boxes[i-1].l),
	//					boxDims[i].x, boxDims[i].y, boxDims[i].z);
	//	assert(pm->addBox(boxes[i]) == i && "Your physics model is out of sync with the rest of the world...");
	//}

	this->setFlag(IS_STATIC, 1);
	modelAnimationState = T_IDLE;
	
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1,50);
	attackCounter = distribution(generator);
	
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
	attackCounter++;

	// start attacking!
	if (attackCounter > attackBuffer && !( (attackCounter - attackBuffer) % CYCLE)){
		if (this->getFlag(IS_HARMFUL))
		{
			this->setFlag(IS_HARMFUL, 0);
			attackBuffer = rand() % 40;
			attackFrames = - rand() % 15;
			modelAnimationState = T_IDLE;
			this->getPhysicsModel()->ref->setRot(lastRotation);
		} else {
			float angle = this->angleToNearestPlayer();
			if (angle != -1.f)
			{
				Vec3f axis = Vec3f(0,0,1);
				Vec4f qAngle = Vec4f(axis,angle);
				lastRotation = this->getPhysicsModel()->ref->getRot();
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
	bool reset = false;

	if (modelAnimationState == T_IDLE)
	{
		static int i = 0;
		// reset your state
		if (i==0) {
			Box origBase = idleBoxes[0];
			Box origMiddle = idleBoxes[1];
			Box origTip = idleBoxes[2];

			Vec3f newPosB = Vec3f(origBase.x, origBase.y, origBase.z);
			Vec3f newProportionB = Vec3f(origBase.w, origBase.h, origBase.l);

			Vec3f newPosM = Vec3f(origMiddle.x, origMiddle.y, origMiddle.z);
			Vec3f newProportionM = Vec3f(origMiddle.w, origMiddle.h, origMiddle.l);

			Vec3f newPosT = Vec3f(origTip.x, origTip.y, origTip.z);
			Vec3f newProportionT = Vec3f(origTip.w, origTip.h, origTip.l);
			
			//get the actual axis
			Vec4f axis = this->getPhysicsModel()->ref->getRot();

			newPosB = axis.rotateToThisAxis(newPosB);
			newProportionB = axis.rotateToThisAxis(newProportionB);
			newPosM = axis.rotateToThisAxis(newPosM);
			newProportionM = axis.rotateToThisAxis(newProportionM);
			newPosT = axis.rotateToThisAxis(newPosT);
			newProportionT = axis.rotateToThisAxis(newProportionT);
			
			base.x = newPosB.x;
			base.y = newPosB.y;
			base.z = newPosB.z;

			base.w = newProportionB.x;
			base.h = newProportionB.y;
			base.l = newProportionB.z;

			middle.x = newPosM.x;
			middle.y = newPosM.y;
			middle.z = newPosM.z;

			middle.w = newProportionM.x;
			middle.h = newProportionM.y;
			middle.l = newProportionM.z;

			tip.x = newPosT.x;
			tip.y = newPosT.y;
			tip.z = newPosT.z;

			tip.w = newProportionT.x;
			tip.h = newProportionT.y;
			tip.l = newProportionT.z;

			pm->colBoxes[0] = base;
			pm->colBoxes[1] = middle;
			pm->colBoxes[2] = tip;	

			reset = true; // todo cleanup this!

			/*middle.w = boxDims[1].x;
			middle.h = boxDims[1].y;
			middle.l = boxDims[1].z;
			middle.y = -45;

			tip.w = boxDims[2].x;
			tip.h = boxDims[2].y;
			tip.l = boxDims[2].z;
			tip.y = -15;*/
			/*changeProportionM.y=30;
			changePosM.y=-45;
			changePosT.y=-15;*/
		}
		if(i < 15) {
			/*middle.h+=10;
			  middle.y--;
			  tip.y++;*/
			changeProportionM.y+=7;
			changePosM.y--;
			changePosT.y++;
		}
		else {
			/*middle.h-=10;
			  middle.y++;
			  tip.y--;*/
			changeProportionM.y-=7;
			changePosM.y++;
			changePosT.y--;
		}

		i= (i + 1) % 31;

	} else { // SLAM
		Vec3f pos;
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
		/*pm->colBoxes[0] = base;
		pm->colBoxes[1] = middle;
		pm->colBoxes[2] = tip;*/
	}
	
	if (!reset)
	{
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

		pm->colBoxes[0] = base;
		pm->colBoxes[1] = middle;
		pm->colBoxes[2] = tip;
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
	}
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
		health-= player->damage;
		if(this->health < 0) health = 0;
		if(this->health > 100) health = 100;
	}
}
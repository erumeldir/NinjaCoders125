#include "RageSObj.h"
#include "ServerObjectManager.h"

int RageSObj::lifetime;

RageSObj::RageSObj(uint id, Point_t pos, Box bx) : ServerObject(id)
{
	if(SOM::get()->debugFlag) DC::get()->print("Created new RageSObj %d\n", id);
	this->modelNum = (Model)-1; // these guys are invisible
	pm = new PhysicsModel(pos, Quat_t(), 1);
	pm->addBox(bx);
	age = 0;
	this->setFlag(IS_STATIC, 1);
	lifetime = 20;
}


RageSObj::~RageSObj(void)
{
	delete pm;
}

#define GROW_UNIT 8.0
bool RageSObj::update()
{
	// Grow our box!
	Box bx = this->getPhysicsModel()->colBoxes.at(0);
	Vec4f axis = this->getPhysicsModel()->ref->getRot();
	Vec3f changePos = Vec3f(), changeProportion = Vec3f();

	changeProportion += Vec3f(GROW_UNIT,GROW_UNIT,GROW_UNIT);
	changePos -= Vec3f(GROW_UNIT/2.f, GROW_UNIT/2.f, GROW_UNIT/2.f);

	changeProportion = axis.rotateToThisAxis(changeProportion);
	changePos = axis.rotateToThisAxis(changePos);

	bx.setRelSize(changeProportion);
	bx.setRelPos(changePos);

	pm->colBoxes[0] = *(bx.fix());

	age++;
	return age >= lifetime;
}

int RageSObj::serialize(char * buf) {
	ObjectState *state = (ObjectState*)buf;
	state->modelNum = modelNum;

	if (SOM::get()->collisionMode)
	{
		CollisionState *collState = (CollisionState*)(buf + sizeof(ObjectState));

		vector<Box> objBoxes = pm->colBoxes;

		collState->totalBoxes = min(objBoxes.size(), maxBoxes);

		for (int i=0; i<collState->totalBoxes; i++)
		{
			collState->boxes[i] = objBoxes[i] + pm->ref->getPos(); // copying applyPhysics
		}

		return pm->ref->serialize(buf + sizeof(ObjectState) + sizeof(CollisionState)) + sizeof(ObjectState) + sizeof(CollisionState);
	}
	else
	{
		return pm->ref->serialize(buf + sizeof(ObjectState)) + sizeof(ObjectState);
	}
}
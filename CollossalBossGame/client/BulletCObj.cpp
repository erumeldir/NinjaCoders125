#include "BulletCObj.h"
#include "ClientEngine.h"
#include "ClientObjectManager.h"
#include <math.h>
#include <Windows.h>
#include "RenderEngine.h"
#include "NetworkData.h"

BulletCObj::BulletCObj(uint id, char *serializedData) :
	ClientObject(id)
{
	if (COM::get()->debugFlag) DC::get()->print("Created new TestObject %d\n", id);

	ObjectState *state = (ObjectState*)serializedData;

	rm = new RenderModel(Point_t(),Quat_t(), state->modelNum);
	deserialize(serializedData);
}


BulletCObj::~BulletCObj(void)
{
	// delete xctrl;
	delete rm;
}

bool BulletCObj::update() {
	return false;
}

void BulletCObj::deserialize(char* newState) {
	ObjectState *state = (ObjectState*)newState;

	if (COM::get()->collisionMode)
	{
		CollisionState *collState = (CollisionState*)(newState + sizeof(ObjectState));

		rm->colBoxes.clear();
		for (int i=0; i<collState->totalBoxes; i++)
		{
			rm->colBoxes.push_back(collState->boxes[i]);
		}

		rm->getFrameOfRef()->deserialize(newState + sizeof(ObjectState) + sizeof(CollisionState));
	}
	else
	{
		rm->getFrameOfRef()->deserialize(newState + sizeof(ObjectState));
	}
}

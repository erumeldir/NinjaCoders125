#include "TentacleCObj.h"
#include "ClientObjectManager.h"
#include "RenderEngine.h"

TentacleCObj::TentacleCObj(uint id, char *data) : ClientObject(id, OBJ_TENTACLE)
{
	if (COM::get()->debugFlag) DC::get()->print("Created new TentacleCObj %d\n", id);
	TentacleState *state = (TentacleState*)data;
	rm = new RenderModel(Point_t(), Quat_t(), state->modelNum);
	smoking = new SmokeEffect();
	RE::get()->addParticleEffect(smoking);
}

TentacleCObj::~TentacleCObj(void)
{
	delete smoking;
	delete rm;
}

RenderModel* TentacleCObj::getBox() {
	//1. get the box model
	//2. return the model
	return box;
}

bool TentacleCObj::update() {
	static float density = .000002;
	static float densityCounter = .000002;
	smoking->setPosition(this->rm->getFrameOfRef()->getPos());
	smoking->update(.33);
	RE::get()->startFog(density);
	if(density != 0) densityCounter+=.000002;
	if(densityCounter < .01) density+=.000002;
	if(densityCounter >= .01) density-=.000002;
	if(densityCounter == .000002) density = 0;
	return false;
}

void TentacleCObj::deserialize(char* newState) {
	TentacleState *state = (TentacleState*)newState;
	this->getRenderModel()->setModelState(state->animationState);

	if (COM::get()->collisionMode)
	{
		CollisionState *collState = (CollisionState*)(newState + sizeof(TentacleState));

		rm->colBoxes.clear();
		for (int i=0; i<collState->totalBoxes; i++)
		{
			rm->colBoxes.push_back(collState->boxes[i]);
		}

		rm->getFrameOfRef()->deserialize(newState + sizeof(TentacleState) + sizeof(CollisionState));
	}
	else
	{
		rm->getFrameOfRef()->deserialize(newState + sizeof(TentacleState));
	}
}
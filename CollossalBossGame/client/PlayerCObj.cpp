#include "PlayerCObj.h"
#include "RenderEngine.h"
#include "NetworkData.h"
#include "ClientObjectManager.h"
#define M_PI 3.14159 // TODO We should move this out somewhere

PlayerCObj::PlayerCObj(uint id, char *data) :
	ClientObject(id)
{
	DC::get()->print("Created new PlayerCObj %d\n", id);
	rm = new RenderModel(Point_t(300,500,0),Rot_t(0,0,M_PI), MDL_0);
}

PlayerCObj::~PlayerCObj(void)
{
	delete rm;
}

bool PlayerCObj::update() {
	//Move the camera to follow the player
	if(COM::get()->player_id == getId()) {
		Point_t objPos = rm->getFrameOfRef()->getPos();
		Rot_t objDir = rm->getFrameOfRef()->getRot();
		/*int factor = 100;
		Point_t camPos(objPos.x, objPos.y, objPos.z - factor);
		RE::get()->setCameraInfo(objPos, camPos, Point_t(0, 1, 0));*/
		//RE::get()->getCamera()->forward(objPos.z);
		RE::get()->getCamera()->setYaw(objDir.y);
		RE::get()->getCamera()->setZ(objPos.z);
		RE::get()->getCamera()->setX(objPos.x);
		//RE::get()->getCamera()->yaw(objDir.y);
		
		RE::get()->updateCamera();
	}
	return false;
}

void PlayerCObj::deserialize(char* newState) {
	ObjectState *state = (ObjectState*)newState;
	rm->getFrameOfRef()->deserialize(newState + sizeof(ObjectState));
}
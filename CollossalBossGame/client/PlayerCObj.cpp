#include "PlayerCObj.h"
#include "RenderEngine.h"
#include "NetworkData.h"
#include "ClientObjectManager.h"
#include "ClientEngine.h"
#include "defs.h"
#include <math.h>

PlayerCObj::PlayerCObj(uint id, char *data) :
	ClientObject(id)
{
	DC::get()->print("Created new PlayerCObj %d\n", id);
	rm = new RenderModel(Point_t(300,500,0),Rot_t(0,0,M_PI), MDL_0);
	cameraPitch = 0;
}

PlayerCObj::~PlayerCObj(void)
{
	delete rm;
}

bool PlayerCObj::update() {
	//Move the camera to follow the player
	if(COM::get()->player_id == getId()) {
		XboxController *xctrl = CE::getController();
		if(xctrl->isConnected()) {
			
			if(xctrl->getState().Gamepad.bLeftTrigger) {
				cameraPitch = 0;
			} else if(fabs((double)xctrl->getState().Gamepad.sThumbRY) > DEADZONE) {
				cameraPitch += atan(((double)xctrl->getState().Gamepad.sThumbRY / (JOY_MAX * 8)));
				if (cameraPitch > M_PI / 2) {
					cameraPitch = M_PI / 2;
				} else if(cameraPitch < -M_PI / 4) {
					cameraPitch = -M_PI / 4;
				}
			}

			// This is horrendous, server should tell us
			if (xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				RE::get()->hudText = "PEW! PEW!";
			}
			else
			{
				RE::get()->hudText = "You are not pew pew-ing!";
			}
		}
		Point_t objPos = rm->getFrameOfRef()->getPos();
		Rot_t objDir = rm->getFrameOfRef()->getRot();
		DC::get()->print("Pitch: %f (controller input = %f)\n", cameraPitch, atan(((double)xctrl->getState().Gamepad.sThumbRY / (JOY_MAX * 8))));
		objDir.x = cameraPitch;
		RE::get()->updateCamera(objPos, objDir);
	}
	return false;
}

void PlayerCObj::deserialize(char* newState) {
	ObjectState *state = (ObjectState*)newState;
	rm->getFrameOfRef()->deserialize(newState + sizeof(ObjectState));
}
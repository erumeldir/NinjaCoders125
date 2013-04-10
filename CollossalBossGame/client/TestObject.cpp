#include "TestObject.h"
#include "ClientEngine.h"
#include <math.h>
#define M_PI 3.14159

TestObject::TestObject(uint id) :
	ClientObject(id)
{
	printf("Created new TestObject %d\n", id);
	xctrl = new XboxController(1); // For now, we can decide later if we want to change the id
	if(!xctrl->isConnected()) {
		printf("Error: Controller %d is not connected\n", id % 4);
	}
	rm = new RenderModel(Point_t(300,500,0),Rot_t(0,0,M_PI));
}


TestObject::~TestObject(void)
{
	delete xctrl;
	delete rm;
}

bool TestObject::update() {
	if(xctrl->isConnected()) {
		if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			printf("A is pressed                                               \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			printf("B is pressed                                               \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			printf("X is pressed                                               \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			printf("Y is pressed                                               \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_START) {
			printf("start > is pressed                                         \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
			printf("< back is pressed                                          \r");
			CE::get()->exit();
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
			printf("UP is pressed                                              \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			printf("LEFT is pressed                                            \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
			printf("DOWN is pressed                                            \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
			printf("RIGHT is pressed                                           \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
			printf("Left thumbstick is pressed                                 \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			printf("Right thumbstick is pressed                                \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			printf("Left shoulder is pressed                                   \r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			printf("Right shoulder is pressed                                  \r");
		} else if(xctrl->getState().Gamepad.bLeftTrigger) {
			printf("Left trigger is pressed                                    \r");
		} else if(xctrl->getState().Gamepad.bRightTrigger) {
			printf("Right trigger is pressed                                   \r");
		} else {
			//32767 = 7fff
			
			printf("Left stick (%4.4d,%4.4d), Right stick (%4.4d,%4.4d)\r",
				xctrl->getState().Gamepad.sThumbLX, xctrl->getState().Gamepad.sThumbLY,
				xctrl->getState().Gamepad.sThumbRX, xctrl->getState().Gamepad.sThumbRY);
		}
		float x = xctrl->getState().Gamepad.sThumbLX,
				y = xctrl->getState().Gamepad.sThumbLY;
		float x2 = xctrl->getState().Gamepad.sThumbRX,
				y2 = xctrl->getState().Gamepad.sThumbRY;

		if(fabs(x) > DEADZONE || fabs(y) > DEADZONE) {
			rm->getFrameOfRef()->setRot(Rot_t(0,0,M_PI + atan2(x / DEADZONE,y / DEADZONE)));
		} else if(fabs(x2) > DEADZONE || fabs(y2) > DEADZONE) {
			rm->getFrameOfRef()->setRot(Rot_t(0,0,M_PI + atan2(x2 / DEADZONE,y2 / DEADZONE)));
		}
		if(fabs(x2) > DEADZONE || fabs(y2) > DEADZONE) {
			Point_t pos = rm->getFrameOfRef()->getPos();
			rm->getFrameOfRef()->setPos(Point_t(pos.x + x2 / DIV, pos.y - y2 / DIV, 0));
			xctrl->vibrate(MAX_VIB, MAX_VIB);
		} else {
			xctrl->vibrate(0, 0);
		}
	} else {
		printf("Error: Controller is not connected                             \r");
	}
	return false;
}
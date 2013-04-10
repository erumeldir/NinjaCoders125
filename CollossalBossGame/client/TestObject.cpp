#include "TestObject.h"


TestObject::TestObject(uint id, Frame newCenter) :
	ClientObject(id)
{
	printf("Created new TestObject %d\n", id);
	center = newCenter;
	xctrl = new XboxController(1); // For now, we can decide later if we want to change the id
	if(!xctrl->isConnected()) {
		printf("Error: Controller %d is not connected\n", id % 4);
	}
}


TestObject::~TestObject(void)
{
	delete xctrl;
}

bool TestObject::update() {
	if(xctrl->isConnected()) {
		if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			printf("A is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			printf("B is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			printf("X is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			printf("Y is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_START) {
			printf("start > is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
			printf("< back is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
			printf("UP is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			printf("LEFT is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
			printf("DOWN is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
			printf("RIGHT is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
			printf("Left thumbstick is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			printf("Right thumbstick is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			printf("Left shoulder is pressed\r");
		} else if(xctrl->getState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			printf("Right shoulder is pressed\r");
		} else if(xctrl->getState().Gamepad.bLeftTrigger) {
			printf("Left trigger is pressed\r");
		} else if(xctrl->getState().Gamepad.bRightTrigger) {
			printf("Right trigger is pressed\r");
		} else {
			printf("Left stick (%3d,%3d), Right stick (%3d,%3d)\r",
				xctrl->getState().Gamepad.sThumbLX, xctrl->getState().Gamepad.sThumbLY,
				xctrl->getState().Gamepad.sThumbRX, xctrl->getState().Gamepad.sThumbRY);
		}
	} else {
		printf("Error: Controller is not connected\r");
	}
	return false;
}
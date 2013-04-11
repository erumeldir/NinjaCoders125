#include "XboxController.h"
#include "Action.h"
#include "ClientNetworkManager.h"
#include <stdio.h>
/*
 * XBOX Controller
 * Class for handling controller input
 * Code thanks to http://www.codeproject.com/Articles/26949/Xbox-360-Controller-Input-in-C-with-XInput
 */
XboxController::XboxController(int player)
{
    // Set the Controller Number
    controllerNum = player - 1;
}

XboxController::~XboxController(void)
{
}

XINPUT_STATE XboxController::getState()
{
    // Zeroise the state
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    // Get the state
    XInputGetState(controllerNum, &state);

    return state;
}

void XboxController::sendInput() {
	// Get the input and send it to the server
	controllerstatus cstat;
	SecureZeroMemory(&cstat, sizeof(cstat));
	if(isConnected()) {
		//cstat.A = true;
		if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			printf("A is pressed                                               \r");
			cstat.A = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			printf("B is pressed                                               \r");
			cstat.B = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			printf("X is pressed                                               \r");
			cstat.X = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			printf("Y is pressed                                               \r");
			cstat.Y = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_START) {
			printf("start > is pressed                                         \r");
			cstat.Start = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
			printf("< back is pressed                                          \r");
			cstat.Back = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
			printf("UP is pressed                                              \r");
			cstat.UP = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			printf("LEFT is pressed                                            \r");
			cstat.Left = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
			printf("DOWN is pressed                                            \r");
			cstat.Down = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
			printf("RIGHT is pressed                                           \r");
			cstat.Right = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
			printf("Left thumbstick is pressed                                 \r");
			cstat.LPress = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			printf("Right thumbstick is pressed                                \r");
			cstat.RPress = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			printf("Left shoulder is pressed                                   \r");
			cstat.LShoulder = true;
		} else if(getState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			printf("Right shoulder is pressed                                  \r");
			cstat.RShoulder = true;
		} else if(getState().Gamepad.bLeftTrigger) {
			printf("Left trigger is pressed                                    \r");
			cstat.LTrigger = true;
		} else if(getState().Gamepad.bRightTrigger) {
			printf("Right trigger is pressed                                   \r");
			cstat.RTrigger = true;
		} else {
			//32767 = 7fff
			
			printf("Left stick (%4.4d,%4.4d), Right stick (%4.4d,%4.4d)\r",
				getState().Gamepad.sThumbLX, getState().Gamepad.sThumbLY,
				getState().Gamepad.sThumbRX, getState().Gamepad.sThumbRY);
		}
		float x = getState().Gamepad.sThumbLX,
				y = getState().Gamepad.sThumbLY;
		float x2 = getState().Gamepad.sThumbRX,
				y2 = getState().Gamepad.sThumbRY;
		cstat.x1 = x;
		cstat.y1 = y;
		cstat.x2 = x2;
		cstat.y2 = y2;

		ClientNetworkManager::get()->sendData(reinterpret_cast<char*>(&cstat), sizeof(controllerstatus));
	}
}

bool XboxController::isConnected()
{
    // Zeroise the state
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    // Get the state
    DWORD result = XInputGetState(controllerNum, &state);

    if(result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void XboxController::vibrate(int leftVal, int rightVal)
{
    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    // Vibrate the controller
    XInputSetState(controllerNum, &Vibration);
}

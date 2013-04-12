#include "ClientNetworkManager.h"
#include "XboxController.h"
#include "Action.h"
#include <stdio.h>
#include <math.h>

#define M_PI 3.14159

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
	inputstatus istat;
	SecureZeroMemory(&istat, sizeof(istat));
	if(isConnected()) {
		XINPUT_GAMEPAD gamepad = getState().Gamepad;

		// Map input to actions
		istat.jump = gamepad.wButtons & XINPUT_GAMEPAD_A;
		istat.specialPower = gamepad.wButtons & XINPUT_GAMEPAD_B;
		istat.quit = gamepad.wButtons & XINPUT_GAMEPAD_BACK;
		istat.attack = gamepad.bRightTrigger; 

		// Get joystick positions
		float x = gamepad.sThumbLX,
				y = gamepad.sThumbLY;
		float x2 = gamepad.sThumbRX,
				y2 = gamepad.sThumbRY;

		// Set position
		//istat.xDist = fabs(x) > DEADZONE ? x / DIV : 0;
		//istat.yDist = fabs(y) > DEADZONE ? y / DIV : 0;
		if(fabs(x) > DEADZONE || fabs(y) > DEADZONE) {
			istat.xDist = x / DIV;
			istat.yDist = y / DIV;
		} else {
			istat.xDist = 0;
			istat.yDist = 0;
		}
		// Set rotation
		istat.rotAngle = 0;
		if(fabs(x2) > DEADZONE || fabs(y2) > DEADZONE) {
			istat.rotAngle = M_PI + atan2(x2 / DEADZONE, y2 / DEADZONE);
		} else if(fabs(x) > DEADZONE || fabs(y) > DEADZONE) {
			istat.rotAngle = M_PI + atan2(x / DEADZONE, y / DEADZONE);
		}
		 
		// I don't think we need this extra memcpy...?
		//controllerstatus cs;
		//memcpy(&cs, reinterpret_cast<char*>(&cstat), sizeof(cs));

		// TODO: Should be the player object id, which we'll get from the server after connecting
		ClientNetworkManager::get()->sendData(reinterpret_cast<char*>(&istat), sizeof(inputstatus), 0);
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

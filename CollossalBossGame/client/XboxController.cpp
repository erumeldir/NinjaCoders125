#include "ClientNetworkManager.h"
#include "ClientObjectManager.h"
#include "ClientEngine.h"
#include "XboxController.h"
#include "Action.h"
#include "defs.h"
#include <stdio.h>
#include <math.h>

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
		istat.jump =			(gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
		istat.specialPower =	(gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
		istat.quit =			(gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
		istat.start =			(gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
		istat.attack =			(gamepad.bRightTrigger) != 0;
		istat.camLock =			(gamepad.bLeftTrigger);


		// Get joystick positions
		float x = gamepad.sThumbLX,
		      y = gamepad.sThumbLY;
		float x2 = gamepad.sThumbRX,
		      y2 = gamepad.sThumbRY;

		// Set position
		if(fabs(x) > DEADZONE || fabs(y) > DEADZONE) {
			istat.rightDist = x / DIV;
			istat.forwardDist = y / DIV;
		} else {
			istat.rightDist = 0;
			istat.forwardDist = 0;
		}
		// Set rotation
		istat.rotAngle = atan2(x / DEADZONE, y / DEADZONE);
		istat.rotHoriz = 0;
		istat.rotVert = 0;

		if (fabs(x2) > DEADZONE)
		{
			istat.rotHoriz = atan(x2 / (JOY_MAX * 16));
		}
		if (fabs(y2) > DEADZONE)
		{
			istat.rotVert = atan(y2 / (JOY_MAX * 16));
		}

		bool lshb = (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
		bool rshb = (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
		if(istat.start || istat.quit || lshb || rshb) {
			GameData gd;
			gd.clear();
			gd.start = istat.start;
			gd.hardreset = istat.quit;
			gd.left = lshb;
			gd.right = rshb;
			gd.playerid = COM::get()->player_id;
			ClientNetworkManager::get()->sendData(GAMESTATE_MANAGER, reinterpret_cast<char*>(&gd), sizeof(GameData), COM::get()->player_id);
		}
	}
	if(CE::get()->state.currentState < GAME_RUNNING) {
		SecureZeroMemory(&istat, sizeof(istat));
	}
	//Send the input data, zero'd if nothing is there
	ClientNetworkManager::get()->sendData(OBJECT_MANAGER, reinterpret_cast<char*>(&istat), sizeof(inputstatus), COM::get()->player_id);
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

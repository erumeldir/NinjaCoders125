#pragma once
/*
 * XBOX Controller
 * Class for handling controller input
 * Code thanks to http://www.codeproject.com/Articles/26949/Xbox-360-Controller-Input-in-C-with-XInput
 */

// We need the Windows Header and the XInput Header
#include <windows.h>
#include <XInput.h>


// Now, the XInput Library
// NOTE: COMMENT THIS OUT IF YOU ARE NOT USING
// A COMPILER THAT SUPPORTS THIS METHOD OF LINKING LIBRARIES
#pragma comment(lib, "XInput.lib")

//Useful defines
#define JOY_MAX 0x7fff
#define DEADZONE JOY_MAX / 2
#define DIV JOY_MAX * 2	//Divide by this to get an acceptable motion speed
#define MAX_VIB 0xFFFF

class XboxController
{
public:
	XboxController(int player);
	virtual ~XboxController(void);

    XINPUT_STATE getState();
	void sendInput();
    bool isConnected();
    void vibrate(int leftVal = 0, int rightVal = 0);
private:
    XINPUT_STATE state;
    int controllerNum;
};

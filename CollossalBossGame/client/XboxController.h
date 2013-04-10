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

class XboxController
{
public:
	XboxController(int player);
	virtual ~XboxController(void);

    XINPUT_STATE getState();
    bool isConnected();
    void vibrate(int leftVal = 0, int rightVal = 0);
private:
    XINPUT_STATE state;
    int controllerNum;
};

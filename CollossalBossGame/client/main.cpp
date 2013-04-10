/*
 * main.cpp
 */

#include "ClientEngine.h"
#include "GameClient.h"
#include "defs.h"

#include <windows.h>
#include <windowsx.h>

GameClient * client;

void clientLoop()
{
    while(true)
    {
        //do game stuff
        client->update();
    }
}




int main(int argc, char **argv) {
	/*
	client = new GameClient();
	clientLoop();
	*/
	CE::init();
	CE::get()->run();
	CE::clean();

	return CE::getExitStatus();
}


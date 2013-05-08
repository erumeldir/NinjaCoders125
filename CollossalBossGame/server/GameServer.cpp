#include "GameServer.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
#include "ServerWorldManager.h"
#include "PhysicsEngine.h"
#include "game.h"
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <time.h> 
using namespace std;

#define TICK 1000/30 // 30 times per second (in ms)

GameServer::GameServer(void)
{

}

/* the main function on the server side, initalizes the server loop

*/
int main()
{
	// CM::get();
	// cout << CM::get()->find_config("asdf") << endl;
	// system("pause");
	DC::init("serverLog.txt");
	SOM::init();
	PE::init();

	// Keep track of how long our updates take
	time_t beginLoopTimer;
	time_t endLoopTimer;
	double totalLoopTime;

	//Create game objects
	gameInit();

	//Main server loop
	while(true) 
    {
		// Get timestamp
		time(&beginLoopTimer);

		// Get input from client
        ServerNetworkManager::get()->update();

		// Update state
		SOM::get()->update();

		// Send state to client
		ServerWorldManager::get()->sendState();
		SOM::get()->sendState();

		// Wait until next clock tick
		time(&endLoopTimer);
		totalLoopTime = difftime(endLoopTimer, beginLoopTimer)  * 1000; // in ms
		// Be sure to set debug to the right thing!
		if (totalLoopTime < TICK) {
			Sleep((DWORD)(TICK - totalLoopTime));
		}
		else
		{
			int tick = TICK;
			DC::get()->print("WARNING!!! total loop time %f is greater than tick time: %d\n...NOTE: this might mean a client is connecting\n", totalLoopTime, tick);
		}
		
    }

	SOM::clean();
	PE::clean();
	DC::clean();
}

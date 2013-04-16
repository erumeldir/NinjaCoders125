#include "GameServer.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
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

	// Keep track of how long our updates take
	time_t beginLoopTimer;
	time_t endLoopTimer;
	double totalLoopTime;
	
	DC::get()->print("%s %d: Made it here\n",__FILE__,__LINE__);

	//Create game objects
	gameInit();

	//Main server loop
	while(true) 
    {
		// Get timestamp
		time(&beginLoopTimer);
	DC::get()->print("%s %d: Made it here\n",__FILE__,__LINE__);

		// Get input from client
        ServerNetworkManager::get()->update();
	DC::get()->print("%s %d: Made it here\n",__FILE__,__LINE__);

		// Update state
		SOM::get()->update();
	DC::get()->print("%s %d: Made it here\n",__FILE__,__LINE__);

		// Send state to client
		SOM::get()->sendState();
	DC::get()->print("%s %d: Made it here\n",__FILE__,__LINE__);

		// Wait until next clock tick
		time(&endLoopTimer);
	DC::get()->print("%s %d: Made it here\n",__FILE__,__LINE__);
		totalLoopTime = difftime(endLoopTimer, beginLoopTimer)  * 1000; // in ms
	DC::get()->print("%s %d: Made it here\n",__FILE__,__LINE__);
		// Be sure to set debug to the right thing!
		if (totalLoopTime < TICK) {
			Sleep(TICK - totalLoopTime);
		}
		else
		{
			// TODO: Print to error console
			DC::get()->print("ERROR!!! total loop time %d is greater than tick time: %d\n", totalLoopTime, TICK);
		}
	DC::get()->print("%s %d: Made it here\n",__FILE__,__LINE__);
		
    }

	SOM::clean();
	DC::clean();
}

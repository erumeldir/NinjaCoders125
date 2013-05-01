#include "GameServer.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"
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
	
	int dir = 0;
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
		
		//Bryan's manipulation of boxes
		ServerObject * base = SOM::get()->find(2);
		ServerObject * middle = SOM::get()->find(1);
		ServerObject * tip = SOM::get()->find(0);

		if (dir >= 50) {
			DC::get()->print("Reset!\n");
			dir = 0;
		} 

		/*Stages:
			0-13 : winding up (13) <--- x
			13-23 : swipe (10)		--> x
			23-35 : swipe back (12) <---
			35-50 : going back (15) -->
		 */
		/*
		Vec3f pos;
		int z;
		if (dir < 13 || (dir >= 23 && dir < 35) ) {
			z = 1;
		} else {
			z = -1;
		}
		if (dir < 13) {
			pos = Vec3f(
				middle->getPhysicsModel()->ref->getPos().x,
				middle->getPhysicsModel()->ref->getPos().y,
				middle->getPhysicsModel()->ref->getPos().z - 4);
			middle->getPhysicsModel()->ref->setPos(pos);

			pos = Vec3f(
				tip->getPhysicsModel()->ref->getPos().x,
				tip->getPhysicsModel()->ref->getPos().y,
				tip->getPhysicsModel()->ref->getPos().z - 10);
			tip->getPhysicsModel()->ref->setPos(pos);
		} else if (dir < 23) {
			pos = Vec3f(
				middle->getPhysicsModel()->ref->getPos().x,
				middle->getPhysicsModel()->ref->getPos().y,
				middle->getPhysicsModel()->ref->getPos().z+ 4);
			middle->getPhysicsModel()->ref->setPos(pos);
			pos = Vec3f(
				tip->getPhysicsModel()->ref->getPos().x,
				tip->getPhysicsModel()->ref->getPos().y,
				tip->getPhysicsModel()->ref->getPos().z+ 10);
			tip->getPhysicsModel()->ref->setPos(pos);
		}else if (dir < 35) {
			pos = Vec3f(
				middle->getPhysicsModel()->ref->getPos().x,
				middle->getPhysicsModel()->ref->getPos().y,
				middle->getPhysicsModel()->ref->getPos().z - 4);
			middle->getPhysicsModel()->ref->setPos(pos);
			pos = Vec3f(
				tip->getPhysicsModel()->ref->getPos().x,
				tip->getPhysicsModel()->ref->getPos().y,
				tip->getPhysicsModel()->ref->getPos().z - 10);
			tip->getPhysicsModel()->ref->setPos(pos);
		}else if (dir < 50) {
			pos = Vec3f(
				middle->getPhysicsModel()->ref->getPos().x,
				middle->getPhysicsModel()->ref->getPos().y,
				middle->getPhysicsModel()->ref->getPos().z+ 4);
			middle->getPhysicsModel()->ref->setPos(pos);
			pos = Vec3f(
				tip->getPhysicsModel()->ref->getPos().x,
				tip->getPhysicsModel()->ref->getPos().y,
				tip->getPhysicsModel()->ref->getPos().z+10);
			tip->getPhysicsModel()->ref->setPos(pos);
		}

		
		if (dir < 13) {
			pos = Vec3f(
				middle->getPhysicsModel()->ref->getPos().x + 4,
				middle->getPhysicsModel()->ref->getPos().y,
				middle->getPhysicsModel()->ref->getPos().z);
			middle->getPhysicsModel()->ref->setPos(pos);

			pos = Vec3f(
				tip->getPhysicsModel()->ref->getPos().x + 10,
				tip->getPhysicsModel()->ref->getPos().y,
				tip->getPhysicsModel()->ref->getPos().z );
			tip->getPhysicsModel()->ref->setPos(pos);
		} else if (dir < 23) {
			pos = Vec3f(
				middle->getPhysicsModel()->ref->getPos().x - 4,
				middle->getPhysicsModel()->ref->getPos().y,
				middle->getPhysicsModel()->ref->getPos().z);
			middle->getPhysicsModel()->ref->setPos(pos);
			pos = Vec3f(
				tip->getPhysicsModel()->ref->getPos().x - 10,
				tip->getPhysicsModel()->ref->getPos().y,
				tip->getPhysicsModel()->ref->getPos().z);
			tip->getPhysicsModel()->ref->setPos(pos);
		}else if (dir < 35) {
			pos = Vec3f(
				middle->getPhysicsModel()->ref->getPos().x + 4,
				middle->getPhysicsModel()->ref->getPos().y,
				middle->getPhysicsModel()->ref->getPos().z);
			middle->getPhysicsModel()->ref->setPos(pos);
			pos = Vec3f(
				tip->getPhysicsModel()->ref->getPos().x + 10,
				tip->getPhysicsModel()->ref->getPos().y,
				tip->getPhysicsModel()->ref->getPos().z);
			tip->getPhysicsModel()->ref->setPos(pos);
		}else if (dir < 50) {
			pos = Vec3f(
				middle->getPhysicsModel()->ref->getPos().x - 4,
				middle->getPhysicsModel()->ref->getPos().y,
				middle->getPhysicsModel()->ref->getPos().z);
			middle->getPhysicsModel()->ref->setPos(pos);
			pos = Vec3f(
				tip->getPhysicsModel()->ref->getPos().x - 10,
				tip->getPhysicsModel()->ref->getPos().y,
				tip->getPhysicsModel()->ref->getPos().z);
			tip->getPhysicsModel()->ref->setPos(pos);
		}
		
		dir++;*/
    }

	SOM::clean();
	PE::clean();
	DC::clean();
}

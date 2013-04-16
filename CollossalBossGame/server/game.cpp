/*
 * game.cpp
 * All game initialization should be added here.  Basically, initial game
 * objects should be created in this file and added to the ServerObjectManager
 * appropriately.
 */
#include "defs.h"
#include "ServerObjectManager.h"

//Objects we can build
#include "TestSObj.h"

void gameInit() {
	ServerObjectManager *som = SOM::get();
	//Create all non-player game objects
	TestSObj *obj0 = new TestSObj(som->genId(), MDL_0, Point_t(50, 0, 100)),
			 *obj1 = new TestSObj(som->genId(), MDL_1, Point_t(-50, 0, 100)),
			 *obj2 = new TestSObj(som->genId(), MDL_2, Point_t(0, 0, 100));
	som->add(obj0);
	som->add(obj1);
	som->add(obj2);
}

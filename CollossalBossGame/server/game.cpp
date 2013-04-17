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
	TestSObj *obj0 = new TestSObj(som->genId(), MDL_1, Point_t(10, 0, 100), TEST_WEST),
			 *obj1 = new TestSObj(som->genId(), MDL_1, Point_t(-10, 0, 100), TEST_EAST),
			 *obj2 = new TestSObj(som->genId(), MDL_2, Point_t(0, 0, 100), TEST_SOUTH),
			 *obj3 = new TestSObj(som->genId(), MDL_2, Point_t(100, 0, 100));
	som->add(obj0);
	som->add(obj1);
	som->add(obj2);
	som->add(obj3);
}

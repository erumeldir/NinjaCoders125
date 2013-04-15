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
	//Create all non-player game objects
	TestSObj *obj0 = new TestSObj(),
			 *obj1 = new TestSObj();
	SOM::get()->add(obj0);
}

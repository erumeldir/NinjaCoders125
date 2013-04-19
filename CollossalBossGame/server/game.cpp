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
#include "WallSObj.h"

void gameInit() {
	ServerObjectManager *som = SOM::get();
	//Create all non-player game objects
	TestSObj *obj0 = new TestSObj(som->genId(), MDL_1, Point_t(-50, 5, 100), TEST_WEST),
			 *obj1 = new TestSObj(som->genId(), MDL_1, Point_t(50, 5, 80), TEST_EAST),
			 *obj2 = new TestSObj(som->genId(), MDL_2, Point_t(0, 0, 100), TEST_SOUTH),
			 *obj3 = new TestSObj(som->genId(), MDL_2, Point_t(50, 10, 50));
	som->add(obj0);
	som->add(obj1);
	som->add(obj2);
	som->add(obj3);
#if 1
	for (int i=0; i< 10; i++)
	{
		//som->add(new TestSObj(som->genId(), MDL_1, Point_t(), TEST_WEST));
	}

	// Build arena (todo maybe these should be planeObjects? ArenaObjects?)
	WallSObj *floor = new WallSObj(som->genId(), MDL_3, Point_t(), Rot_t()),
			 *ceiling = new WallSObj(som->genId(), MDL_3, Point_t(0.f, 150.f, 0.f), Rot_t(0.f, 0.f, M_PI)),
			 *rightWall = new WallSObj(som->genId(), MDL_3, Point_t(75.f, 75.f, 0.f), Rot_t(0.f, 0.f, M_PI/2)),
			 *leftWall = new WallSObj(som->genId(), MDL_3, Point_t(-75.f, 75.f, 0.f), Rot_t(0.f, 0.f, -M_PI/2));
			 //*frontWall = new WallSObj(som->genId(), MDL_3, Point_t(0, 75, 75), Rot_t(0, 0, -M_PI/2));
			// *backWall = new WallSObj(som->genId(), MDL_3, Point_t(-75, 75, 0), Rot_t(0, 0, -M_PI/2));
	
	som->add(floor);
	som->add(ceiling);
	som->add(rightWall);
	som->add(leftWall);
	//som->add(frontWall);
#endif
}

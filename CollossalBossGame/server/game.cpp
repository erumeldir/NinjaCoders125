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
	obj0->setFlag(IS_HEALTHY, 1);
	obj1->setFlag(IS_HARMFUL, 1);
	obj2->setFlag(IS_HARMFUL, 1);
	obj3->setFlag(IS_HEALTHY, 1);
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
	WallSObj //*floor = new WallSObj(som->genId(), MDL_3, Point_t(), Rot_t(), Vec3f(0, 1, 0)),
			 *ceiling = new WallSObj(som->genId(), MDL_3, Point_t(0.f, 150.f, 0.f), Rot_t(0.f, 0.f, M_PI), Vec3f(0, -1, 0)),
			 *rightWall = new WallSObj(som->genId(), MDL_3, Point_t(75.f, 75.f, 0.f), Rot_t(0.f, 0.f, M_PI/2), Vec3f(-1, 0, 0)),
			 *leftWall = new WallSObj(som->genId(), MDL_3, Point_t(-75.f, 75.f, 0.f), Rot_t(0.f, 0.f, -M_PI/2), Vec3f(1, 0, 0));
			 //*frontWall = new WallSObj(som->genId(), MDL_3, Point_t(0, 75, 75), Rot_t(0, 0, -M_PI/2));
			// *backWall = new WallSObj(som->genId(), MDL_3, Point_t(-75, 75, 0), Rot_t(0, 0, -M_PI/2));

	WallSObj *floor1 = new WallSObj(som->genId(), MDL_3, Point_t(-150, 0, 0), Rot_t(), Vec3f(0, 1, 0));
	WallSObj *floor2 = new WallSObj(som->genId(), MDL_3, Point_t(0, 0, 0), Rot_t(), Vec3f(0, 1, 0));
	WallSObj *floor3 = new WallSObj(som->genId(), MDL_3, Point_t(150, 0, 0), Rot_t(), Vec3f(0, 1, 0));
	WallSObj *floor4 = new WallSObj(som->genId(), MDL_3, Point_t(0, 0, 150), Rot_t(), Vec3f(0, 1, 0));
	WallSObj *floor5 = new WallSObj(som->genId(), MDL_3, Point_t(0, 0, 0), Rot_t(), Vec3f(0, 1, 0));
	WallSObj *floor6 = new WallSObj(som->genId(), MDL_3, Point_t(0, 0, -150), Rot_t(), Vec3f(0, 1, 0));

	ceiling->setFlag(IS_WALL, 1);
	rightWall->setFlag(IS_WALL, 1);
	leftWall->setFlag(IS_WALL, 1);
	som->add(floor1);
	som->add(floor2);
	som->add(floor3);
	som->add(floor4);
	som->add(floor5);
	som->add(floor6);
	som->add(ceiling);
	som->add(rightWall);
	som->add(leftWall);
	//som->add(frontWall);
#endif
}

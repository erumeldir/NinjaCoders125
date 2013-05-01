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
#include "MonsterSObj.h"
#include "PhysicsEngine.h"
#include "MonsterSObj.h"
#include "TentacleSObj.h"
#include "ConfigurationManager.h"

/*
 * buildRoom()
 * Builds our arena!
 */
void buildRoom() {
	// Get config measurements
	int width = CM::get()->find_config_as_int("ROOM_WIDTH");
	int height = CM::get()->find_config_as_int("ROOM_HEIGHT");
	int depth = CM::get()->find_config_as_int("ROOM_DEPTH");

	ServerObjectManager *som = SOM::get();

	WallSObj *floor, *ceiling,
			 *north, *south,
			 *east, *west;

	floor   = new WallSObj(som->genId(), MDL_FLOOR, Point_t(), DOWN, Vec3f(2.5f, 1.f, 2.5f));
	ceiling = new WallSObj(som->genId(), MDL_CEILING, Point_t(0.f, (float)height, 0.f), UP, Vec3f(2.5f, 1.f, 2.5f));
	north   = new WallSObj(som->genId(), MDL_NORTH_WALL, Point_t(0.f, (float)height/2.f, (float)-depth/2.f), NORTH, Vec3f(2.5f, 2.5f, 1.f));
	south   = new WallSObj(som->genId(), MDL_SOUTH_WALL, Point_t(0.f, (float)height/2.f, (float)depth/2.f), SOUTH, Vec3f(2.5f, 2.5f, 1.f));
	east    = new WallSObj(som->genId(), MDL_EAST_WALL, Point_t((float)width/2.f, (float)height/2.f, 0.f), EAST, Vec3f(1.f, 2.5f, 2.5f));
	west    = new WallSObj(som->genId(), MDL_WEST_WALL, Point_t((float)-width/2.f, (float)height/2.f, 0.f), WEST, Vec3f(1.f, 2.5f, 2.5f));
	
	som->add(floor);
	som->add(ceiling);
	som->add(east);
	som->add(west);
	som->add(north);
	som->add(south);
}

void gameInit() {
	ServerObjectManager *som = SOM::get();
	
	float xBase = 0, yBase = 10, zBase = -300;
	TestSObj* box10 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase, 0, zBase + 350),Rot_t(0,0,0));
	som->add(box10);
	box10->setFlag(IS_STATIC, 1);
	TestSObj* box11 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase, 0, zBase + 450),Rot_t(0,0,0));
	som->add(box11);
	box11->setFlag(IS_STATIC, 1);
	TestSObj* box12 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase, 0, zBase + 550),Rot_t(0,0,0));
	som->add(box12);
	box12->setFlag(IS_STATIC, 1);

	buildRoom();

	MonsterSObj* monster = new MonsterSObj(som->genId());
//	TentacleSObj* tentacleLeft = new TentacleSObj(som->genId(), MDL_TENTACLE_1, Point_t(0, 100, 300), Rot_t(), monster);

	TentacleSObj* tentacleRight = new TentacleSObj(som->genId(), MDL_TENTACLE_1, Point_t(0, 160, 300), Rot_t(0,0,0), monster);
	//som->add(tentacleLeft);
	som->add(tentacleRight);
	som->add(monster);
	
	/*TestSObj* box13 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 70, yBase, zBase + 50),Rot_t(0,0,0));
	som->add(box13);
	TestSObj* box14 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 90, yBase, zBase + 50),Rot_t(0,0,0));
	som->add(box14);
	TestSObj* box15 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 110, yBase, zBase + 50),Rot_t(0,0,0));
	som->add(box15);
	TestSObj* box16 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 130, yBase, zBase + 50),Rot_t(0,0,0));
	som->add(box16);

	TestSObj* box20 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 10, yBase, zBase + 70),Rot_t(0,0,0));
	som->add(box20);
	TestSObj* box21 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 30, yBase, zBase + 70),Rot_t(0,0,0));
	som->add(box21);
	TestSObj* box22 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 50, yBase, zBase + 70),Rot_t(0,0,0));
	som->add(box22);
	TestSObj* box23 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 70, yBase, zBase + 70),Rot_t(0,0,0));
	som->add(box23);
	TestSObj* box24 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 90, yBase, zBase + 70),Rot_t(0,0,0));
	som->add(box24);
	TestSObj* box25 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 110, yBase, zBase + 70),Rot_t(0,0,0));
	som->add(box25);
	TestSObj* box26 = new TestSObj(som->genId(), MDL_TEST_BOX, Point_t(xBase + 130, yBase, zBase + 70),Rot_t(0,0,0));
	som->add(box26);*/

}


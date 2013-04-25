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

void buildRoom(Point_t center, int w, int h, int l) {
#define WIDTH 150
	ServerObjectManager *som = SOM::get();

	if(w < 1) w = 1;
	if(l < 1) l = 1;
	if(h < 1) h = 1;
	int x, y, z;
	center.x -= w * WIDTH / 2.0f;
	center.y -= h * WIDTH / 2.0f;
	center.z -= l * WIDTH / 2.0f;
	WallSObj *floor, *ceiling,
			 *north, *south,
			 *east, *west;
	DC::get()->print("Making room with bottom-corner at (%f,%f,%f)\n", center.x, center.y, center.z);
#if 1	
	floor   = new WallSObj(som->genId(), MDL_3, Point_t(), WALL_DOWN);
	ceiling = new WallSObj(som->genId(), MDL_3, Point_t(0,WIDTH,0), WALL_UP);
	north   = new WallSObj(som->genId(), MDL_3, Point_t(0,WIDTH/2,-WIDTH/2), WALL_NORTH);
	south   = new WallSObj(som->genId(), MDL_3, Point_t(0,WIDTH/2,WIDTH/2), WALL_SOUTH);
	east    = new WallSObj(som->genId(), MDL_3, Point_t(WIDTH/2,WIDTH/2,0), WALL_EAST);
	west    = new WallSObj(som->genId(), MDL_3, Point_t(-WIDTH/2,WIDTH/2,0), WALL_WEST);
	
	som->add(floor);
	//som->add(ceiling);
	//som->add(east);
	//som->add(west);
	//som->add(north);
	//som->add(south);
	PE::get()->setLimits(-WIDTH / 2, 0, -WIDTH / 2,
						  WIDTH / 2, WIDTH, WIDTH / 2);
#else
	//floor/ceiling
	for(x = 0; x < w; ++x) {
		for(z = 0; z < l; ++z) {
			floor   = new WallSObj(som->genId(), MDL_3, Point_t(center.x + x * WIDTH, center.y,             center.z + z * WIDTH), WALL_DOWN);
			ceiling = new WallSObj(som->genId(), MDL_3, Point_t(center.x + x * WIDTH, center.y + h * WIDTH, center.z + z * WIDTH), WALL_UP);
			som->add(floor);
			som->add(ceiling);
		}
	}
	//north/south
	for(x = 0; x < w; ++x) {
		for(y = 0; y < h; ++y) {
			north   = new WallSObj(som->genId(), MDL_3, Point_t(center.x + x * WIDTH, center.y + y * WIDTH, center.z),             WALL_NORTH);
			south   = new WallSObj(som->genId(), MDL_3, Point_t(center.x + x * WIDTH, center.y + y * WIDTH, center.z + l * WIDTH), WALL_SOUTH);
			som->add(north);
			som->add(south);
		}
	}
	//east/west
	for(z = 0; z < l; ++z) {
		for(y = 0; y < h; ++y) {
			east    = new WallSObj(som->genId(), MDL_3, Point_t(center.x + w * WIDTH, center.y + y * WIDTH, center.z + z * WIDTH), WALL_EAST);
			west    = new WallSObj(som->genId(), MDL_3, Point_t(center.x,             center.y + y * WIDTH, center.z + z * WIDTH), WALL_WEST);
			som->add(east);
			som->add(west);
		}
	}
#endif

	MonsterSObj* tentacleLeft = new MonsterSObj(som->genId(), MDL_5, Point_t(0, 0, 0), Rot_t(0, 0, 0));
	//tentacleLeft->setFlag(IS_HARMFUL, 1);
	som->add(tentacleLeft);
}


void gameInit() {
#if 0
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

	WallSObj *floor,// = new WallSObj(som->genId(), MDL_3, Point_t(), Rot_t(), Vec3f(0, 1, 0)),
			 *ceiling = new WallSObj(som->genId(), MDL_3, Point_t(0.f, 40.f, 0.f), WALL_UP), //Rot_t(0.f, 0.f, M_PI), Vec3f(0, -1, 0)),
			 *rightWall = new WallSObj(som->genId(), MDL_3, Point_t(75.f, 75.f, 0.f), WALL_EAST), //Rot_t(0.f, 0.f, M_PI/2), Vec3f(-1, 0, 0)),
			 *leftWall = new WallSObj(som->genId(), MDL_3, Point_t(0.f, 75.f, 0.f), WALL_WEST), //Rot_t(0.f, 0.f, -M_PI/2), Vec3f(1, 0, 0));
			 *backWall = new WallSObj(som->genId(), MDL_3, Point_t(0, 75, 75), WALL_NORTH), //Rot_t(0, 0, -M_PI/2), Vec3f(0, 0, -1)),
			 *frontWall = new WallSObj(som->genId(), MDL_3, Point_t(-75, 75, 0), WALL_SOUTH); //Rot_t(0, 0, -M_PI/2), Vec3f(0, 0, 1));

for(int x = -1; x < 2; ++x) {
	for(int z = -1; z < 2; ++ z) {
		floor = new WallSObj(som->genId(), MDL_3, Point_t(x * 150, 0, z * 150), WALL_DOWN); //Rot_t(), Vec3f(0, 1, 0));
		som->add(floor);

	}
}
	//ceiling->setFlag(IS_WALL,1);
	rightWall->setFlag(IS_WALL,1);
	leftWall->setFlag(IS_WALL,1);
	floor->setFlag(IS_WALL,1);
	som->add(floor);
	//som->add(ceiling);
	som->add(rightWall);
	som->add(leftWall);
	som->add(frontWall);
	som->add(backWall);
#endif
#endif

	buildRoom(Point_t(0, WIDTH / 2, 0), 1, 1, 1);
}

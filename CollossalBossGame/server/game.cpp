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

/*
 * buildRoom()
 * Builds six walls centered about the specified point, of width w, height h, and length l
 * TODO: Currently just builds six walls centered around (0,75,0)
 */
void buildRoom(Point_t center, int w, int h, int l) {
#define WIDTH 1265 //150
#define HEIGHT 290
#define DEPTH 724
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

	floor   = new WallSObj(som->genId(), MDL_FLOOR, Point_t(), DOWN, Vec3f(2.5f, 1.f, 2.5f));
	ceiling = new WallSObj(som->genId(), MDL_CEILING, Point_t(0,HEIGHT,0), UP, Vec3f(2.5f, 1.f, 2.5f));
	north   = new WallSObj(som->genId(), MDL_NORTH_WALL, Point_t(0,HEIGHT/2,-DEPTH/2), NORTH, Vec3f(2.5f, 2.5f, 1.f));
	south   = new WallSObj(som->genId(), MDL_SOUTH_WALL, Point_t(0,HEIGHT/2,DEPTH/2), SOUTH, Vec3f(2.5f, 2.5f, 1.f));
	east    = new WallSObj(som->genId(), MDL_EAST_WALL, Point_t(WIDTH/2,HEIGHT/2,0), EAST, Vec3f(1.f, 2.5f, 2.5f));
	west    = new WallSObj(som->genId(), MDL_WEST_WALL, Point_t(-WIDTH/2,HEIGHT/2,0), WEST, Vec3f(1.f, 2.5f, 2.5f));
	
	som->add(floor);
	som->add(ceiling);
	som->add(east);
	som->add(west);
	som->add(north);
	som->add(south);

	// todo ask nathan if we still need this
	PE::get()->setLimits(-WIDTH / 2, 0, -WIDTH / 2,
						  WIDTH / 2, WIDTH, WIDTH / 2);
}


void gameInit() {
	ServerObjectManager *som = SOM::get();

	buildRoom(Point_t(0, WIDTH / 2, 0), 1, 1, 1);

	MonsterSObj* monster = new MonsterSObj(som->genId());
	TentacleSObj* tentacleLeft = new TentacleSObj(som->genId(), MDL_TENTACLE_1, Point_t(-20, 100, 300), Rot_t(), monster);
	TentacleSObj* tentacleRight = new TentacleSObj(som->genId(), MDL_TENTACLE_2, Point_t(-20, 100, -300), Rot_t(M_PI,0,0), monster);
	som->add(tentacleLeft);
	som->add(tentacleRight);
	som->add(monster);

	TestSObj* pyramid = new TestSObj(som->genId(), MDL_TEST_PYRAMID, Point_t(100,0,100), Rot_t());
	som->add(pyramid);

}

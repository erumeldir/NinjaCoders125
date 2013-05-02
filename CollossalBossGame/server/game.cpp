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
#include "WorldSObj.h"
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

	floor   = new WallSObj(som->genId(), MDL_FLOOR, Point_t(), DOWN);
	ceiling = new WallSObj(som->genId(), MDL_CEILING, Point_t(0.f, (float)height, 0.f), UP);
	north   = new WallSObj(som->genId(), MDL_NORTH_WALL, Point_t(0.f, (float)height/2.f, (float)-depth/2.f), NORTH);
	south   = new WallSObj(som->genId(), MDL_SOUTH_WALL, Point_t(0.f, (float)height/2.f, (float)depth/2.f), SOUTH);
	east    = new WallSObj(som->genId(), MDL_EAST_WALL, Point_t((float)width/2.f, (float)height/2.f, 0.f), EAST);
	west    = new WallSObj(som->genId(), MDL_WEST_WALL, Point_t((float)-width/2.f, (float)height/2.f, 0.f), WEST);
	
	som->add(floor);
	som->add(ceiling);
	som->add(east);
	som->add(west);
	som->add(north);
	som->add(south);
}


void gameInit() {
	ServerObjectManager *som = SOM::get();

	
	buildRoom();

	//This object manages the world state
	WorldSObj *wobj = new WorldSObj(som->genId());
	som->add(wobj);

	MonsterSObj* monster = new MonsterSObj(som->genId());
	TentacleSObj* tentacleLeft = new TentacleSObj(som->genId(), MDL_TENTACLE_1, Point_t(-20, 100, 300), Quat_t(), monster);
	TentacleSObj* tentacleRight = new TentacleSObj(som->genId(), MDL_TENTACLE_2, Point_t(-20, 100, -300), Quat_t(Vec3f(1,0,0),(float)M_PI), monster);

	som->add(tentacleLeft);
	som->add(tentacleRight);
	som->add(monster);
}

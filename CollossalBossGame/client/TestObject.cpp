#include "TestObject.h"
#include "ClientEngine.h"
#include <math.h>
#include <Windows.h>
#include "Action.h"
#define M_PI 3.14159

TestObject::TestObject(uint id) :
	ClientObject(id)
{
	printf("Created new TestObject %d\n", id);
	/*
	xctrl = new XboxController(1); // For now, we can decide later if we want to change the id
	if(!xctrl->isConnected()) {
		printf("Error: Controller %d is not connected\n", id % 4);
	}
	*/
	rm = new RenderModel(Point_t(300,500,0),Rot_t(0,0,M_PI));

	// Initialize input status
	istat.attack = false;
	istat.jump = false;
	istat.quit = false;
	istat.specialPower = false;
	istat.rotAngle = 0.0;
	istat.xDist = 0.0;
	istat.yDist = 0.0;
}


TestObject::~TestObject(void)
{
	// delete xctrl;
	delete rm;
}

bool TestObject::update() {
	if (istat.quit) {
		CE::get()->exit();
	}
	if (istat.attack) {
		printf("ATTACKING!!!                                               \r");
	}
	if (istat.jump) {
		printf("JUMPING!                                                   \r");
	}
	if (istat.specialPower) {
		printf("SPECIAL POWER!!!!!                                         \r");
	}
	
	rm->getFrameOfRef()->setRot(Rot_t(0, 0, istat.rotAngle));
	Point_t pos = rm->getFrameOfRef()->getPos();
	rm->getFrameOfRef()->setPos(Point_t(pos.x + istat.xDist, pos.y - istat.yDist, 0));

	// TODO Note: Should we vibrate the controller from here? like...from the player object? 

	return false;
}
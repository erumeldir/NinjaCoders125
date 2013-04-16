#include "TestObject.h"
#include "ClientEngine.h"
#include <math.h>
#include <Windows.h>
#include "RenderEngine.h"
#define M_PI 3.14159


TestObject::TestObject(uint id, char *data, const char * filename) :
	ClientObject(id)
{
	DC::get()->print("Created new TestObject %d\n", id);
	/*
	xctrl = new XboxController(1); // For now, we can decide later if we want to change the id
	if(!xctrl->isConnected()) {
		DC::get()->print("Error: Controller %d is not connected\n", id % 4);
	}
	*/
	rm = new RenderModel(Point_t(),Rot_t(), filename);
	deserialize(data);

	// Initialize input status
	/*istat.attack = false;
	istat.jump = false;
	istat.quit = false;
	istat.specialPower = false;
	istat.rotAngle = 0.0;
	istat.xDist = 0.0;
	istat.yDist = 0.0;*/
}


TestObject::~TestObject(void)
{
	// delete xctrl;
	delete rm;
}

bool TestObject::update() {
	if(getId() == 0) {
	//RE::get()->setCameraPos(rm->getFrameOfRef()->getPos(), rm->getFrameOfRef()->getRot());
	Point_t objPos = rm->getFrameOfRef()->getPos();
	Rot_t objDir = rm->getFrameOfRef()->getRot();
	int factor = 100;
	Point_t camPos(objPos.x, objPos.y, objPos.z - factor);
	RE::get()->setCameraInfo(objPos, camPos, Point_t(0, 1, 0));
	}
	return false;
}

void TestObject::deserialize(char* newState) {
	rm->getFrameOfRef()->deserialize(newState);
}
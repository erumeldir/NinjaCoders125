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
}


TestObject::~TestObject(void)
{
	// delete xctrl;
	delete rm;
}

bool TestObject::update() {
	if(cstat.A) {
		printf("A is pressed                                               \r");
	} else if(cstat.B) {
		printf("B is pressed                                               \r");
	} else if(cstat.X) {
		printf("X is pressed                                               \r");
	} else if(cstat.Y) {
		printf("Y is pressed                                               \r");
	} else if(cstat.Start) {
		printf("start > is pressed                                         \r");
	} else if(cstat.Back) {
		printf("< back is pressed                                          \r");
		CE::get()->exit();
	} else if(cstat.UP) {
		printf("UP is pressed                                              \r");
	} else if(cstat.Left) {
		printf("LEFT is pressed                                            \r");
	} else if(cstat.Down) {
		printf("DOWN is pressed                                            \r");
	} else if(cstat.Right) {
		printf("RIGHT is pressed                                           \r");
	} else if(cstat.LPress) {
		printf("Left thumbstick is pressed                                 \r");
	} else if(cstat.RPress) {
		printf("Right thumbstick is pressed                                \r");
	} else if(cstat.LShoulder) {
		printf("Left shoulder is pressed                                   \r");
	} else if(cstat.RShoulder) {
		printf("Right shoulder is pressed                                  \r");
	} else if(cstat.LTrigger) {
		printf("Left trigger is pressed                                    \r");
	} else if(cstat.RTrigger) {
		printf("Right trigger is pressed                                   \r");
	} else {
		//32767 = 7fff
		/*printf("Left stick (%4.4d,%4.4d), Right stick (%4.4d,%4.4d)\r",
			cstat.x1, cstat.y1,
			cstat.x2, cstat.y2);*/
	}
	float x = cstat.x1;
	float y = cstat.y1;
	float x2 = cstat.x2;
	float y2 = cstat.y2;
	if(fabs(x) > DEADZONE || fabs(y) > DEADZONE) {
		rm->getFrameOfRef()->setRot(Rot_t(0,0,M_PI + atan2(x / DEADZONE,y / DEADZONE)));
	} else if(fabs(x2) > DEADZONE || fabs(y2) > DEADZONE) {
		rm->getFrameOfRef()->setRot(Rot_t(0,0,M_PI + atan2(x2 / DEADZONE,y2 / DEADZONE)));
	}
	if(fabs(x2) > DEADZONE || fabs(y2) > DEADZONE) {
		Point_t pos = rm->getFrameOfRef()->getPos();
		rm->getFrameOfRef()->setPos(Point_t(pos.x + x2 / DIV, pos.y - y2 / DIV, 0));
		//xctrl->vibrate(MAX_VIB, MAX_VIB);
	} else {
		//xctrl->vibrate(0, 0);
	}
	return false;
}
#pragma once
#include "ClientObject.h"
#include "Frame.h"
#include "Action.h"
#include "XboxController.h"


class TestObject : public ClientObject
{
public:
	TestObject(uint id);
	virtual ~TestObject(void);

	virtual bool update();

	virtual RenderModel* getRenderModel() { return rm; }

	controllerstatus cstat;

private:
	RenderModel *rm;
	// XboxController *xctrl;
};


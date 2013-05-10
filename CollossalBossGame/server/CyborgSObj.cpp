#include "CyborgSObj.h"
#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"

CyborgSObj::CyborgSObj(uint id, uint clientId) : PlayerSObj(id, clientId, CHAR_CLASS_CYBORG)
{
	// Other re-initializations (things that don't depend on parameters, like config)
	this->initialize();
}


CyborgSObj::~CyborgSObj(void)
{
}

void CyborgSObj::initialize() {
	// Configuration options
	PlayerSObj::initialize();
	

}

void CyborgSObj::releaseCharge() {
	pm->applyForce(rotate(Vec3f(0, chargeForce * charge, chargeForce * charge), pm->ref->getRot()));
	charging = true;
}
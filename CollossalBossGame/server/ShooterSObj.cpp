#include "ShooterSObj.h"
#include "BulletSObj.h"
#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include "ServerObjectManager.h"

ShooterSObj::ShooterSObj(uint id, uint clientId) : PlayerSObj(id, clientId, CHAR_CLASS_SHOOTER)
{
	// Other re-initializations (things that don't depend on parameters, like config)
	this->initialize();
}


ShooterSObj::~ShooterSObj(void)
{
}

void ShooterSObj::initialize() {
	// Configuration options
	PlayerSObj::initialize();
}

void ShooterSObj::releaseCharge() {
	BulletSObj * bso = new BulletSObj(SOM::get()->genId(), (Model)-1, this->pm->ref->getPos(), rotate(Vec3f(0, chargeForce * charge, chargeForce * charge), pm->ref->getRot()));
	SOM::get()->add(bso);
}
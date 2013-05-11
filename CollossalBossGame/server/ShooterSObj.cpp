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
	Vec3f position;
	position.x = this->pm->ref->getPos().x;
	position.y = 10 + this->pm->ref->getPos().y;
	position.z = this->pm->ref->getPos().z;
	Vec3f force = rotate(Vec3f(0, chargeForce * charge, chargeForce * charge), pm->ref->getRot());
	BulletSObj * bso = new BulletSObj(SOM::get()->genId(), (Model)-1/*MDL_TEST_BOX*/, position, force, 1);
	SOM::get()->add(bso);
}
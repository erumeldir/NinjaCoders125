#include "ServerObject.h"


ServerObject::ServerObject(uint id) {
	this->id = id;
	flags = 0;

	//Set default flags
	setFlag(IS_FALLING, true);
}


ServerObject::~ServerObject(void) {
}

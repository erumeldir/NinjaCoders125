#include "ServerObject.h"


ServerObject::ServerObject(uint id) {
	this->id = id;
}


ServerObject::~ServerObject(void) {
}

void ServerObject::deserialize(char* newInput) {
	// TODO: add to error console
	printf("ERROR! Trying to deserialize input for a server object that doesn't take input from the client\n");
}
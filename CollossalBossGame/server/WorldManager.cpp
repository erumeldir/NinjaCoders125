#pragma once
#include "WorldManager.h"
#include "ServerObjectManager.h"
#include "EventManager.h"

void manageEvent(EventTypes evt, void * obj) {
	ServerObject * sobj = (ServerObject *)(obj);
	switch(evt) {
		case EVENT_DEATH:
			WorldManager::get()->event_death(sobj);
			break;
		case EVENT_RESET:
			WorldManager::get()->event_reset(sobj);
			break;
		case EVENT_CONNECTION:
			WorldManager::get()->event_connection(sobj);
			break;
		case EVENT_DISCONNECT:
			WorldManager::get()->event_connection(sobj);
			break;
		default:
			// Print Debug Statement - Event not handled by this object.
			break;
	}
}

WorldManager WorldManager::world;

WorldManager::~WorldManager() {}

WorldManager::WorldManager() {
	deathCount = 0;
	resetCount = 0;
	EventManager::get()->registerHandler(&manageEvent);
}

void WorldManager::init() {
	deathCount = 0;
	resetCount = 0;
}

WorldManager * WorldManager::get() {
	return &world;
}

void WorldManager::event_reset(ServerObject * obj) {
	if(deathCount == totalPlayerCount) {
		resetCount++;
	}
	if(resetCount == totalPlayerCount) {
		deathCount = 0;
		resetCount = 0;
		// Fire some function to reset the positions of all server objects.
		SOM::get()->reset();
	}
}

void WorldManager::event_death(ServerObject * obj) {
	deathCount++;
	assert((deathCount <= totalPlayerCount) && "Implementation Error");
}

void WorldManager::event_connection(ServerObject * obj) {
	totalPlayerCount++;
}

void WorldManager::event_disconnect(ServerObject * obj) {
	totalPlayerCount--;
	assert((totalPlayerCount < 0) && "Implementation Error");
}


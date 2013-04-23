#pragma once
#include "EventManager.h"
#include "ServerObjectManager.h"

// TODO Franklin Low Priority: Make the EventManager more robust.

EventManager EventManager::em;

EventManager::~EventManager() {}

EventManager::EventManager() {
	handlers.clear();
}

void EventManager::init() {
	
}

EventManager * EventManager::get() {
	return &em;
}

void EventManager::registerHandler(eventHandler handler) {
	handlers.push_back(handler);
}

void EventManager::fireEvent(EventTypes evt, void* obj) {
	for(int i = 0; i < handlers.size(); i++) {
		handlers[i](evt, obj);
	}
}
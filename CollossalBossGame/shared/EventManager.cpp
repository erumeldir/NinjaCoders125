#pragma once
/* Event Manager
 * Author: Franklin
 */

#include "EventManager.h"

void EventManager::registerHandler(EventType evt, eventHandler handler) {
	handlers.find(evt)->second.push_back(handler);
}

void EventManager::fireEvent(EventType evt, EventData * data, void* obj) {
	vector<eventHandler> * handles = &handlers.find(evt)->second;
	for(uint i = 0; i < handles->size(); i++) {
		(*handles)[i](data, obj);
	}
}
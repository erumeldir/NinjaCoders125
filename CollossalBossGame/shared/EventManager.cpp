#pragma once
/* Event Manager
 * Author: Franklin
 */

#include "EventManager.h"

EventManager * EventManager::em;
bool EventManager::initialized = false;

EventManager::EventManager() {
	handlers = new map<EventType, vector<CBGEventHandler*>*>();
	// Low Priority: Could optimize by assigning EventTypes numbers and using a for loop.
	handlers->insert(pair<EventType, vector<CBGEventHandler *>*>(EVENT_PLAYER_DEATH, new vector<CBGEventHandler *>()));
	handlers->insert(pair<EventType, vector<CBGEventHandler *>*>(EVENT_MONSTER_DEATH, new vector<CBGEventHandler *>()));
	handlers->insert(pair<EventType, vector<CBGEventHandler *>*>(EVENT_RESET, new vector<CBGEventHandler *>()));
	handlers->insert(pair<EventType, vector<CBGEventHandler *>*>(EVENT_HARD_RESET, new vector<CBGEventHandler *>()));
	handlers->insert(pair<EventType, vector<CBGEventHandler *>*>(EVENT_CONNECTION, new vector<CBGEventHandler *>()));
	handlers->insert(pair<EventType, vector<CBGEventHandler *>*>(EVENT_MONSTER_SPAWN, new vector<CBGEventHandler *>()));
	handlers->insert(pair<EventType, vector<CBGEventHandler *>*>(EVENT_DISCONNECT, new vector<CBGEventHandler *>()));
}

EventManager::~EventManager() {
	map<EventType, vector<CBGEventHandler*>*>::iterator it = handlers->begin();
	for(; it != handlers->end(); ++it) {
		delete it->second;
	}
	handlers->clear();
	delete handlers;
}

EventManager * EventManager::get() {
	if(!initialized) {
		init();
		initialized = true;
	}
	return em;
}

void EventManager::registerHandler(EventType evt, CBGEventHandler * handleobj) {
	vector<CBGEventHandler *>* v = handlers->find(evt)->second;
	v->push_back(handleobj);
}

void EventManager::fireEvent(EventType evt, EventData * data, void* obj) {
	vector<CBGEventHandler *> * handles = handlers->find(evt)->second;
	vector<int> rm;
	for(uint i = 0; i < handles->size(); i++) {
		CBGEventHandler * handle = (*handles)[i];
		if(handle == NULL) {
			rm.push_back(i);
			continue;
		}
		switch(evt) {
		case EVENT_PLAYER_DEATH:
			handle->event_player_death(data, obj);
			break;
		case EVENT_MONSTER_DEATH:
			handle->event_monster_death(data, obj);
			break;
		case EVENT_RESET:
			handle->event_reset(data, obj);
			break;
		case EVENT_CONNECTION:
			handle->event_connection(data, obj);
			break;
		case EVENT_MONSTER_SPAWN:
			handle->event_monster_spawn(data, obj);
			break;
		case EVENT_DISCONNECT:
			handle->event_disconnect(data, obj);
			break;
		default:
			assert(false && "Firing new event");	
		}
	}
	// Clean up any objects that may have been deleted.
	for(int i = 0; i < rm.size(); i++) {
		handles->erase(handles->begin()+rm[i]);
	}
}

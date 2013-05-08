#pragma once
#include "defs.h"
#include <assert.h>
#include <vector>
#include <map>
#include "CBGEventHandler.h"

enum EventType {
	EVENT_PLAYER_DEATH,
	EVENT_MONSTER_DEATH,
	EVENT_RESET,
	EVENT_CONNECTION,
	EVENT_MONSTER_SPAWN,
	EVENT_DISCONNECT,
	EVENT_HARD_RESET
};

typedef void (*eventHandler)(EventData * data, void * obj	);

class EventManager
{
public:
	static EventManager * get();
	static void init() { if(!initialized) em = new EventManager(); initialized = true; }
	static void clean() { delete em; }

	void registerHandler(EventType evt, CBGEventHandler * handleobj);
	void fireEvent(EventType evt, EventData * data, void* obj);

private:
	EventManager();
	~EventManager();
	static EventManager * em;
	static bool initialized;

	map<EventType, vector<CBGEventHandler *>*> * handlers;

};
typedef EventManager EM;
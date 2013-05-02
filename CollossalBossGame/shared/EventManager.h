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
	EVENT_DISCONNECT
};

typedef void (*eventHandler)(EventData * data, void * obj	);

class EventManager
{
public:
	static EventManager * get();

	void registerHandler(EventType evt, CBGEventHandler * handleobj);
	void fireEvent(EventType evt, EventData * data, void* obj);

private:
	static EventManager * em;
	static bool initialized;
	map<EventType, vector<CBGEventHandler *>*> * handlers;
	EventManager();
	~EventManager();
	static void init();
};
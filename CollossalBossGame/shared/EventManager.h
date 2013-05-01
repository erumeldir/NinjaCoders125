#pragma once
#include "defs.h"
#include <assert.h>
#include <vector>
#include <map>

enum EventType {
	EVENT_PLAYER_DEATH,
	EVENT_MONSTER_DEATH,
	EVENT_RESET,
	EVENT_CONNECTION,
	EVENT_MONSTER_SPAWN,
	EVENT_DISCONNECT
};

struct EventData {
	int value;
};

typedef void (*eventHandler)(EventData * data, void*);

class EventManager
{
public:
	EventManager() { handlers.clear(); }
	~EventManager() { }
	void init() { }
	static EventManager * get() { return &em; }

	void registerHandler(EventType evt, eventHandler handler);
	void fireEvent(EventType evt, EventData * data, void* obj);

private:
	static EventManager em;
	map<EventType, vector<eventHandler>> handlers;
};
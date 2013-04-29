#pragma once
#include "defs.h"
#include <assert.h>
#include <vector>

enum EventTypes {
	EVENT_PLAYER_DEATH = 1,
	EVENT_MONSTER_DEATH = 2,
	EVENT_RESET = 4,
	EVENT_CONNECTION = 8,
	EVENT_MONSTER_SPAWN = 16,
	EVENT_DISCONNECT = 32
};

typedef void (*eventHandler)(EventTypes, void*);

class EventManager
{
public:
	EventManager();
	~EventManager();
	void init();
	static EventManager * get();

	void registerHandler(eventHandler handler);
	void fireEvent(EventTypes evt, void* obj);

private:
	static EventManager em;
	vector<eventHandler> handlers;
};
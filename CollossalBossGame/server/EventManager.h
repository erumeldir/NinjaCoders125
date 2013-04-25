#pragma once
#include "defs.h"
#include <assert.h>
#include <vector>

enum EventTypes {
	EVENT_DEATH = 1,
	EVENT_RESET = 2,
	EVENT_CONNECTION = 4,
	EVENT_DISCONNECT = 8
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
#pragma once
#include "defs.h"

class CBGEventHandler
{
public:
	CBGEventHandler(void) { }
	virtual ~CBGEventHandler(void) { }

	virtual void register_handlers() = 0;

	virtual void event_player_death(EventData * data, void* obj) { }
	virtual void event_monster_death(EventData * data, void* obj) { }
	virtual void event_reset(EventData * data, void* obj) { }
	virtual void event_connection(EventData * data, void* obj) { }
	virtual void event_monster_spawn(EventData * data, void* obj) { }
	virtual void event_disconnect(EventData * data, void* obj) { }
};


#pragma once
#include "defs.h"
#include <assert.h>
#include "NetworkData.h"
#include "ServerObject.h"
#include "EventManager.h"
#include "CBGEventHandler.h"
#include "WorldState.h"

class ServerWorldManager : public CBGEventHandler
{
public:
	ServerWorldManager();
	~ServerWorldManager(void);
	void init();
	static ServerWorldManager * get();
	
	void register_handlers();

	void event_player_death(EventData * data, void * obj);
	void event_monster_death(EventData * data, void * obj);
	void event_reset(EventData * data, void * obj);
	void event_hard_reset(EventData * data, void * obj);
	void event_connection(EventData * data, void * obj);
	void event_monster_spawn(EventData * data, void * obj);
	void event_disconnect(EventData * data, void * obj);

	void sendState();
	void updateState(char * buf);

private:
	static ServerWorldManager world; // Statics ftw.

	WorldState ws;

};

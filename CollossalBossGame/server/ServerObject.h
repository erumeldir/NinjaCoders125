#pragma once
#include "defs.h"
#include "PhysicsModel.h"
#include <assert.h>
#include "NetworkData.h"

class ServerObject
{
public:
	ServerObject(uint id);
	virtual ~ServerObject(void);

	//To create a server object, you must implement these methods
	virtual bool update() = 0;							//Perform logic update
	virtual PhysicsModel *getPhysicsModel() = 0;		//Get this object's physics model
	virtual int serialize(char * buf) = 0;				//Write all pertinent information to the specified buffer
	virtual ObjectType getType() = 0;					//Get object type for client-side creation
	virtual void onCollision(ServerObject *obj) = 0;	//Perform any logic ops on collision

	// Can be overriden, but should be only used by objects that receive
	// input from the client (a.k.a. playerobjects)
	virtual void deserialize(char* newState) {
		// TODO: add to error console
		// DC::get()->print("ERROR! Trying to deserialize input for a server object that doesn't take input from the client\n");
		assert(false && "ERROR! Trying to deserialize input for a server object that doesn't take input from the client\n");
	}

	inline uint getId() { return id; }
	inline uint getFlag(uint flag) { return GET_FLAG(flags, flag); }
	inline void setFlag(uint flag, uint val) { flags = SET_FLAG(flags, flag, val); }

private:
	uint id;
	uint flags;
};


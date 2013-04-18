#include "ServerNetworkManager.h"
#include "ServerObjectManager.h"
#include "PhysicsEngine.h"

ServerObjectManager *ServerObjectManager::som;

ServerObjectManager::ServerObjectManager(void) {
	curId = 0;
}


ServerObjectManager::~ServerObjectManager(void)
{
	for(map<uint, ServerObject *>::iterator it = mObjs.begin();
			it != mObjs.end();
			++it) {
		delete it->second;
	}
	mObjs.clear();
}

/*
 * Updates all objects
 *  logic
 *  physics
 *  collision checks
 *  deletes objects requesting deletion
 *  removes objects requesting removal
 *  adds objects requesting addtion
 *  tracks objects removed but not deleted
 *  adds changed objects to the server with appropriate commands
 */

void ServerObjectManager::update() {
	list<uint> lsObjsToDelete;
	list<uint>::iterator idIter;
	list<ServerObject *> lsObjsThatMoved;
	list<ServerObject *>::iterator objIter;
	for(map<uint, ServerObject *>::iterator it = mObjs.begin();
			it != mObjs.end();
			++it) {
		//Update object (game logic)
		if(it->second->update()) {
			//Object requested that it be deleted
			lsObjsToDelete.push_back(it->second->getId());
			continue;
		}

		//Update physics
		if(PE::get()->applyPhysics(it->second->getPhysicsModel())) {
			//Add this object to the list of objects that have moved
			lsObjsThatMoved.push_back(it->second);
		} else {
			//Perform initial layer of collision checks against objects
			// that have moved, if this object has not moved
			for(objIter = lsObjsThatMoved.begin(); objIter != lsObjsThatMoved.end(); ++objIter) {
				PE::get()->applyPhysics(*objIter, it->second);
			}
		}
	}

	//Remove objects queued for deleting BEFORE collision checks occur
	for(idIter = lsObjsToDelete.begin(); idIter != lsObjsToDelete.end(); ++idIter) {
		map<uint, ServerObject *>::iterator itObj = mObjs.find(*idIter);
		if(itObj != mObjs.end()) {
			lsObjsToSend.push_back(pair<CommandTypes,ServerObject*>(CMD_DELETE,itObj->second));
			//ServerObject *obj = itObj->second;
			mObjs.erase(itObj);
			//delete obj;	We don't perform this until the object is sent
		}
	}
	lsObjsToDelete.clear();

	//Perform final layer of collision checks
	for(objIter = lsObjsThatMoved.begin(); objIter != lsObjsThatMoved.end(); ++objIter) {
		for(map<uint, ServerObject *>::iterator it = mObjs.begin();
				it != mObjs.end();
				++it) {
			//When we reach the same object, we are done
			if(it->first == (*objIter)->getId()) {
				//Send this object to the server
				lsObjsToSend.push_back(pair<CommandTypes,ServerObject*>(CMD_UPDATE,*objIter));
				break;
			} else {
				PE::get()->applyPhysics(*objIter, it->second);
			}
		}
	}

	//Remove objects requested for removal, but not deletion
	for(idIter = lsObjsRemoveQueue.begin(); idIter != lsObjsRemoveQueue.end(); ++idIter) {
		mObjs.erase(*idIter);
	}
	lsObjsRemoveQueue.clear();

	//Add objects requested for addition
	for(objIter = lsObjsAddQueue.begin(); objIter != lsObjsAddQueue.end(); ++objIter) {
		mObjs.insert(pair<uint,ServerObject*>((*objIter)->getId(),*objIter));
		lsObjsToSend.push_back(pair<CommandTypes,ServerObject*>(CMD_CREATE,*objIter));
	}
	lsObjsAddQueue.clear();
}

/**
 * Sends the object states to the clients.
 * Author: Haro
 * Modified by: Nathan
 */
void ServerObjectManager::sendState()
{
	char *buf;
	int datalen;
	int totalData = 0;
	for(list<pair<CommandTypes,ServerObject*> >::iterator it = lsObjsToSend.begin();
			it != lsObjsToSend.end(); ++it) {
		//Initialize the buffer with the object header
		buf = SNM::get()->getSendBuffer();
		switch(it->first) {
		case CMD_UPDATE:
		case CMD_CREATE: {
			//Fill out the header
			CreateHeader *h = (CreateHeader*)buf;
			h->type = it->second->getType();

			//Serialize the object
			datalen = it->second->serialize(buf + sizeof(CreateHeader)) + sizeof(CreateHeader);
			totalData += datalen;
			SNM::get()->sendToAll(ACTION_EVENT, it->second->getId(), it->first, datalen);
			break;
			}
		case CMD_DELETE:
			datalen = 0;
			SNM::get()->sendToAll(ACTION_EVENT, it->second->getId(), it->first, datalen);
			delete it->second;	//We are finally done with this object
		}
	}
	lsObjsToSend.clear();
	//DC::get()->print("Total data sent to client is %d\n", totalData);
	SNM::get()->getSendBuffer();
	SNM::get()->sendToAll(COMPLETE, 0);
/*
	for(map<uint, ServerObject *>::iterator it = mObjs.begin();
			it != mObjs.end();
			++it) {
		// If object changed...
		int datalen = it->second->serialize(ServerNetworkManager::get()->getSendBuffer());
		ServerNetworkManager::get()->sendToAll(ACTION_EVENT, it->second->getId(), datalen);
	}
*/
}

uint ServerObjectManager::genId() {
	if(vFreeIds.size() == 0) {
		return curId++;
	}

	//Recycle an existing id
	uint id = vFreeIds[vFreeIds.size() - 1];
	vFreeIds.pop_back();
	return id;
}

void ServerObjectManager::freeId(uint id) {
	//Mark this ID as being ready for recycling
	vFreeIds.push_back(id);
}

void ServerObjectManager::add(ServerObject *obj) {
	//mObjs.insert(pair<uint,ServerObject*>(obj->getId(), obj));
	lsObjsAddQueue.push_back(obj);
}

ServerObject *ServerObjectManager::find(uint id) {
	map<uint, ServerObject*>::iterator res = mObjs.find(id);
	if(res != mObjs.end()) {
		return res->second;
	}
	return NULL;
}

void ServerObjectManager::remove(uint id) {
	//mObjs.erase(id);
	lsObjsRemoveQueue.push_back(id);
}

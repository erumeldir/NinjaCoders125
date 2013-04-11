#include "ServerNetworkManager.h"
#include "ServerObjectManager.h"
#include "PhysicsEngine.h"
#include "NetworkData.h"

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
 * Performs logic updates
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
			ServerObject *obj = itObj->second;
			mObjs.erase(itObj);
			delete obj;
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
				break;
			} else {
				PE::get()->applyPhysics(*objIter, it->second);
			}
		}
	}

	//Remove objects requested for removal, but not deletion
	for(idIter = lsObjsRemoved.begin(); idIter != lsObjsRemoved.end(); ++idIter) {
		mObjs.erase(*idIter);
	}
	lsObjsToDelete.clear();

	//Add objects requested for addition
	for(objIter = lsObjsAdded.begin(); objIter != lsObjsAdded.end(); ++objIter) {
		mObjs.insert(pair<uint,ServerObject*>((*objIter)->getId(),*objIter));
	}
	lsObjsAdded.clear();
}

/**
 * Sends the object states to the clients.
 * Author: Haro
 */
void ServerObjectManager::sendState()
{
	for(map<uint, ServerObject *>::iterator it = mObjs.begin();
			it != mObjs.end();
			++it) {
		// If object changed...
		pair<int, char*> data = it->second->serialize();
		Packet packet;
		packet.packet_type = ACTION_EVENT;
		packet.object_id = it->second->getId();
		memcpy(packet.packet_data, data.second, data.first);
		char packet_data[104];
		packet.serialize(packet_data);

		ServerNetworkManager::get()->sendToAll(packet_data, data.first);
	}
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
	lsObjsAdded.push_back(obj);
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
	lsObjsRemoved.push_back(id);
}

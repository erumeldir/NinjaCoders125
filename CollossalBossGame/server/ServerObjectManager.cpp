#include "ServerNetworkManager.h"
#include "ServerObjectManager.h"
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
#include <list>

void ServerObjectManager::update() {
	list<uint> lsObjsToDelete;
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

		//Perform initial layer of collision checks
	}
	//Remove objects queued for deleting BEFORE collision checks occur
	for(list<uint>::iterator itDel = lsObjsToDelete.begin();
			itDel != lsObjsToDelete.end();
			++itDel) {
		map<uint, ServerObject *>::iterator itObj = mObjs.find(*itDel);
		ServerObject *obj = itObj->second;
		mObjs.erase(itObj);
		delete obj;
	}
	lsObjsToDelete.clear();

	//Perform final layer of collision checks

	//Remove objects requested for removal, but not deletion

	//Add objects requested for addition

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
	mObjs.insert(pair<uint,ServerObject*>(obj->getId(), obj));
}

ServerObject *ServerObjectManager::find(uint id) {
	map<uint, ServerObject*>::iterator res = mObjs.find(id);
	if(res != mObjs.end()) {
		return res->second;
	}
	return NULL;
}

void ServerObjectManager::remove(uint id) {
	mObjs.erase(id);
}

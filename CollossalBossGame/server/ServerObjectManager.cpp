#include "ServerObjectManager.h"

ServerObjectManager *ServerObjectManager::com;

ServerObjectManager::ServerObjectManager(void) {
	curId = 0;
}


ServerObjectManager::~ServerObjectManager(void)
{
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

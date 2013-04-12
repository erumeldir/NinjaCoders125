#include "ClientObjectManager.h"
#include "RenderEngine.h"
#include "DebugConsole.h"

ClientObjectManager *ClientObjectManager::com;

ClientObjectManager::ClientObjectManager(void) {
	curId = 0;
}


ClientObjectManager::~ClientObjectManager(void) {
	for(map<uint, ClientObject *>::iterator it = mObjs.begin();
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

void ClientObjectManager::update() {
	list<uint> lsObjsToDelete;
	for(map<uint, ClientObject *>::iterator it = mObjs.begin();
			it != mObjs.end();
			++it) {
		//Update object (game logic)
		if(it->second->update()) {
			//Object requested that it be deleted
			lsObjsToDelete.push_back(it->second->getId());
			continue;
		}

		//Give objects to the render engine for rendering
		RE::get()->renderThis(it->second);

		//Update physics

		//Perform initial layer of collision checks
	}
	//Remove objects queued for deleting BEFORE collision checks occur
	for(list<uint>::iterator itDel = lsObjsToDelete.begin();
			itDel != lsObjsToDelete.end();
			++itDel) {
		map<uint, ClientObject *>::iterator itObj = mObjs.find(*itDel);
		ClientObject *obj = itObj->second;
		mObjs.erase(itObj);
		delete obj;
	}
	lsObjsToDelete.clear();

	//Perform final layer of collision checks

	//Remove objects requested for removal, but not deletion

	//Add objects requested for addition

}

uint ClientObjectManager::genId() {
	if(vFreeIds.size() == 0) {
		return curId++;
	}

	//Recycle an existing id
	uint id = vFreeIds[vFreeIds.size() - 1];
	vFreeIds.pop_back();
	return id;
}

void ClientObjectManager::freeId(uint id) {
	//Mark this ID as being ready for recycling
	vFreeIds.push_back(id);
}

void ClientObjectManager::add(ClientObject *obj) {
	mObjs.insert(pair<uint,ClientObject*>(obj->getId(), obj));
}


ClientObject *ClientObjectManager::find(uint id) {
	map<uint, ClientObject*>::iterator res = mObjs.find(id);
	if(res != mObjs.end()) {
		return res->second;
	}
	return NULL;
}

void ClientObjectManager::remove(uint id) {
	mObjs.erase(id);
}

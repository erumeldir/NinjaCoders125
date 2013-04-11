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
 * Performs logic updates
 */
#include <list>

void ServerObjectManager::update() {
	list<uint> lsObjsToDelete;
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

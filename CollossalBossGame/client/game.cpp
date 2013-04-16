/*
 * Write all game and test code here
 */
#include "game.h"
#include "TestObject.h"
#include "ClientObjectManager.h"
#include "PlayerCObj.h"

void gameInit() {
#if 0
	COM *com = COM::get();
	TestObject   *obj0 = new TestObject(com->genId(), "tiny.x");
				 *obj1 = new TestObject(com->genId(), "bones_all.x");
			/*	 *obj2 = new TestObject(com->genId()),
				 *obj3 = new TestObject(com->genId()),
				 *obj4 = new TestObject(com->genId());*/
	//PlayerCObj	*player = new PlayerCObj(com->genId());
	com->add(obj0);
	//com->add(obj1);
	/*com->add(obj1);
	com->add(obj4);
	com->add(obj3);
	com->add(obj2);*/
#endif
}

void gameClean() {
}

#pragma once

#include <vector>
#include <map>

using namespace std;

#include "defs.h"
#include "ClientObject.h"

class ClientObjectManager
{
public:
	static void init() { com = new ClientObjectManager(); }
	static ClientObjectManager *get() { return com; }
	static void clean() { delete com; }

	void update();

	//Object handling
	uint genId();
	void freeId(uint id);
	void add(ClientObject *obj);

private:
	ClientObjectManager(void);
	virtual ~ClientObjectManager(void);

	static ClientObjectManager *com;

	map<uint, ClientObject *> mObjs;
	vector<uint> vFreeIds;
	int curId;
};
typedef ClientObjectManager COM;

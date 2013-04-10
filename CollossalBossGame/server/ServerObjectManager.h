#pragma once

#include <vector>
#include <map>

using namespace std;

#include "defs.h"
#include "ServerObject.h"

class ServerObjectManager
{
public:
	static void init() { som = new ServerObjectManager(); }
	static ServerObjectManager *get() { return som; }
	static void clean() { delete som; }

	void update();

	//Object handling
	uint genId();
	void freeId(uint id);
	void add(ServerObject *obj);
	ServerObject *find(uint id);
	void remove(uint id);

private:
	ServerObjectManager(void);
	virtual ~ServerObjectManager(void);

	static ServerObjectManager *som;

	map<uint, ServerObject *> mObjs;
	vector<uint> vFreeIds;
	int curId;
};
typedef ServerObjectManager SOM;

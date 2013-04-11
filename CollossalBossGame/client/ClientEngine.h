/*
 * ClientEngine.h
 * Defines the main game engine for the client and the server.
 */
#ifndef CLIENT_ENGINE_H
#define CLIENT_ENGINE_H

#include "defs.h"
#include "XboxController.h"

class ClientEngine {
public:
	static void init() { ce = new ClientEngine(); }
	static ClientEngine *get() { return ce; }
	static void clean() { delete ce; }
	static int getExitStatus() { return exitStatus; }

	void run();
	void exit(int i = 0);

private:
	//Constructors/destructors are private
	ClientEngine();
	virtual ~ClientEngine();

	bool isRunning;
	XboxController * xctrl;

	static int exitStatus;
	static ClientEngine *ce;	//instance
};
typedef ClientEngine CE;

#endif

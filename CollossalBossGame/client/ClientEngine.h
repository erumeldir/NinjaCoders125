/*
 * ClientEngine.h
 * Defines the main game engine for the client and the server.
 */
#ifndef CLIENT_ENGINE_H
#define CLIENT_ENGINE_H

class ClientEngine {
public:
	static void init() { ce = new ClientEngine(); }
	static ClientEngine *get() { return ce; }
	static void clean() { delete ce; }

	void run();

private:
	//Constructors/destructors are private
	ClientEngine();
	virtual ~ClientEngine();

	bool isRunning;

	static ClientEngine *ce;	//instance
};

#endif

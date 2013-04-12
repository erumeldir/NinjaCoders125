#pragma once

#include <stdarg.h>
#include <fstream>
using namespace std;

class DebugConsole
{
public:
	static void init(const char* logfile) { dc = new DebugConsole(logfile); }
	static DebugConsole *get() { return dc; }
	static void clean() { delete dc; }

	void enable()	{ enScreen = true; }
	void disable()  { enScreen = false; }
	
	void print(const char *szFormat, ...);
	void print(double f);
	void print(int i);
	void print(char c);

private:
	DebugConsole(const char* logfile);
	virtual ~DebugConsole(void);

	bool enScreen;
	bool enFile;

	void print_str(char *s);

	ofstream fout;

	static DebugConsole *dc;
};
typedef DebugConsole DC;

#pragma once

#include <stdarg.h>
#include <fstream>
using namespace std;

#define MAX_FLAGS_SET   0xFFFFFFFF
#define ALL_USER_LOGS	0x0000FFFF	//Any additional logs can be given bits 0x0 through 0x8000 (16 possible extra log files)
#define ALL_BASE_LOGS   0x000F0000	//Four possible base logs
#define CONSOLE			0x00010000
#define LOGFILE			0x00020000
#define TIMESTAMP		0x10000000
#define ALL_FLAGS		(TIMESTAMP | ALL_BASE_LOGS | ALL_USER_LOGS)
#define ALL_LOGS		(ALL_BASE_LOGS | ALL_USER_LOGS)
#define DEFAULT_FLAGS   ALL_FLAGS

class DebugConsole
{
public:
	static void init(const char* logfile) { dc = new DebugConsole(logfile); }
	static DebugConsole *get() { return dc; }
	static void clean() { delete dc; }
	
	//Manual flags
	void print(int flags, const char *szFormat, ...);
	void print(int flags, double f);
	void print(int flags, int i);
	void print(int flags, char c);

	//Default flags
	void print(const char *szFormat, ...);
	//Not affected by TIMESTAMP flags
	inline void print(double f)	{ print(DEFAULT_FLAGS, f); }
	inline void print(int i)	{ print(DEFAULT_FLAGS, i); }
	inline void print(char c)	{ print(DEFAULT_FLAGS, c); }

private:
	DebugConsole(const char* logfile);
	virtual ~DebugConsole(void);

	bool enFile;

	void print_str(int flags, char *s);
	void printAll(int flags, const char *szFormat, va_list vl);

	ofstream fout;

	static DebugConsole *dc;
};
typedef DebugConsole DC;

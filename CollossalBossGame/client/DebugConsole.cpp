#include "DebugConsole.h"
#include <cstring>

//Static members
DebugConsole *DebugConsole::dc;

DebugConsole::DebugConsole(const char *logfile)
{
	fout.open(logfile);
	enFile = !fout.fail();
	enScreen = true;
}


DebugConsole::~DebugConsole(void)
{
	fout.close();
}

//Code based on design by http://msdn.microsoft.com/en-us/library/fxhdxye9%28v=vs.80%29.aspx
void DebugConsole::print(const char *szFormat, ...) {
	//Process the format string
	va_list vl;
	va_start(vl, szFormat);

	int l = strlen(szFormat);

	va_end(vl);
}


void DebugConsole::print(float f) {
	if(enFile) {
		fout << f;
	}
}

void DebugConsole::print(int i) {
	if(enFile) {
		fout << i;
	}
}

void DebugConsole::print(char c) {
	if(enFile) {
		fout << c;
	}
}

void DebugConsole::print(char *s) {
	if(enFile) {
		fout << s;
	}
}
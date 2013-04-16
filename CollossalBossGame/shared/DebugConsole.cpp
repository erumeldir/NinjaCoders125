#include "DebugConsole.h"
#include "ConfigurationManager.h"
#include <cstring>
#include <Windows.h>

//Static members
DebugConsole *DebugConsole::dc;

DebugConsole::DebugConsole(const char *logfile)
{
	fout.open(logfile);
	enFile = !fout.fail();
}


DebugConsole::~DebugConsole(void)
{
	fout.close();
}

void DebugConsole::print(const char *szFormat, ...) {
	va_list vl;
	va_start(vl, szFormat);
	printAll(DEFAULT_FLAGS, szFormat, vl);
	va_end(vl);
}

void DebugConsole::print(int flags, const char *szFormat, ...) {
	va_list vl;
	va_start(vl, szFormat);
	printAll(flags, szFormat, vl);
	va_end(vl);
}

//Code based on design by http://msdn.microsoft.com/en-us/library/fxhdxye9%28v=vs.80%29.aspx
void DebugConsole::printAll(int flags, const char *szFormat, va_list vl) {
	//Perform initial print functions
	if(flags & TIMESTAMP) {
		SYSTEMTIME st;
		GetLocalTime(&st);
		print(flags, st.wHour);
		print(flags, ':');
		print(flags, st.wMinute);
		print(flags, ':');
		print(flags, st.wSecond);
		print(flags, ':');
		print(flags, st.wMilliseconds);
		print(flags, ' ');
		if(st.wMilliseconds < 100) {
			print(flags, ' ');
			if(st.wMilliseconds < 10) {
				print(flags, ' ');
			}
		}
	}

	//Process the format string
	int l = strlen(szFormat);
    char *str = (char*)malloc(strlen(szFormat) + 1);
    memcpy(str, szFormat, strlen(szFormat) + 1);
    char *ptrStart, *ptrEnd;

    for(ptrStart = str, ptrEnd = str; *ptrEnd != 0; ++ptrEnd) {
        if(*ptrEnd == '%') {
            *ptrEnd++ = 0;
			print_str(flags,ptrStart);
            switch(*ptrEnd) {
            case 'd':
				print(flags,va_arg(vl,int));
                break;
            case 'f':
				print(flags,va_arg(vl,double));
                break;
            case 'c':
				print(flags,va_arg(vl,char));
                break;
            case 's':
				print_str(flags,va_arg(vl,char*));
                break;
            default:
				print('%');
                break;
            }
            ptrStart = ptrEnd + 1;
        }
    }
	print_str(flags,ptrStart);
    free(str);
	fout.flush();
}


void DebugConsole::print(int flags, double f) {
	if(enFile && flags & LOGFILE) {
		fout << f;
	}
	if(flags & CONSOLE) {
		printf("%f", f);
	}
}

void DebugConsole::print(int flags, int i) {
	if(enFile && flags & LOGFILE) {
		fout << i;
	}
	if(flags & CONSOLE) {
		printf("%d", i);
	}
}

void DebugConsole::print(int flags, char c) {
	if(enFile && flags & LOGFILE) {
		fout << c;
	}
	if(flags & CONSOLE) {
		printf("%c", c);
	}
}

void DebugConsole::print_str(int flags, char *s) {
	if(enFile && flags & LOGFILE) {
		fout << s;
	}
	if(flags & CONSOLE) {
		printf("%s", s);
	}
}
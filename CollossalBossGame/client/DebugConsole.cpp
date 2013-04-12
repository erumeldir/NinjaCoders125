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
    char *str = (char*)malloc(strlen(szFormat) + 1);
    memcpy(str, szFormat, strlen(szFormat) + 1);
    char *ptrStart, *ptrEnd;

    for(ptrStart = str, ptrEnd = str; *ptrEnd != 0; ++ptrEnd) {
        if(*ptrEnd == '%') {
            *ptrEnd++ = 0;
			print_str(ptrStart);
            switch(*ptrEnd) {
            case 'd':
				print(va_arg(vl,int));
                break;
            case 'f':
				print(va_arg(vl,double));
                break;
            case 'c':
				print(va_arg(vl,char));
                break;
            case 's':
				print_str(va_arg(vl,char*));
                break;
            default:
				print('%');
                break;
            }
            ptrStart = ptrEnd + 1;
        }
    }
	print_str(ptrStart);
    free(str);

	va_end(vl);
}


void DebugConsole::print(double f) {
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

void DebugConsole::print_str(char *s) {
	if(enFile) {
		fout << s;
	}
}
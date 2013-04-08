/*
 * main.cpp
 */

#include "ClientEngine.h"
#include "defs.h"

int main(int argc, char **argv) {
	CE::init();
	CE::get()->run();
	CE::clean();

	return CE::getExitStatus();
}

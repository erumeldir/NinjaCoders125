/*
 * main.cpp
 */

#include "ClientEngine.h"
#include <stdio.h>

int main(int argc, char **argv) {
	printf("Hello world!\n");

	ClientEngine::init();
	ClientEngine::get()->run();
	ClientEngine::clean();
	return 0;
}

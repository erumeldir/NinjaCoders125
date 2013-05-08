#pragma once

/*
 * HMap
 * This class is used for reading in and storing information about heightmaps.
 */
#include <stdlib.h>
#include <windows.h>
using namespace std;

class HMap
{
public:
	HMap(char * filename, float scale = 1.0f);
	virtual ~HMap(void);

	// Number of units wide
	int getWidth() { return _w; }

	// Number of units long
	int getLength() { return _l; }

	// returns a long int with rgba data for point (x, y)
	float getHeightAt(int i, int j);

private:
	int _w, _l;

	// pixel data
	float * _hdata;
};

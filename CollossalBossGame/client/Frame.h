/*
 * Frame.h
 * This defines a frame of reference
 */
#pragma once
#include "defs.h"
#include <list>
using namespace std;

class Frame {
public:
	Frame(void);
	virtual ~Frame(void);

private:
	Point_t pos;
	Rot_t   rot;
	list<Frame> lsAttachedFrames;
};


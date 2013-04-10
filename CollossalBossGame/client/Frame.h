/*
 * Frame.h
 * This defines a frame of reference
 *
 * Author(s): Nathan, Haro, Suman
 */
#pragma once
#include "defs.h"
#include <list>
using namespace std;

class Frame {
public:
	Frame();
	Frame(Point_t pos, Rot_t rot);
	virtual ~Frame(void);
	void addChild(Frame* newChild);
	
	Point_t pos;
	Rot_t   rot;
private:
	list<Frame*> lsAttachedFrames;
};


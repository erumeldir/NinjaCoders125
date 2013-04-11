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
	
	void translate(const Vec3f &dv);
	void rotate(const Vec3f &dr);

	void setPos(Point_t pt) { pos = pt; }
	void setRot(Rot_t rt) { rot = rt; }
	Point_t getPos() { return pos; }
	Rot_t getRot() { return rot; }
private:
	Point_t pos;
	Rot_t rot;
	list<Frame*> lsAttachedFrames;
};


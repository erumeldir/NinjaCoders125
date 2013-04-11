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

struct stateMessage {
	Point_t pos;
	Rot_t rot;
};

class Frame {
public:
	Frame();
	Frame(const Point_t &pos, const Rot_t &rot);
	virtual ~Frame(void);
	void addChild(Frame* newChild);
	
	void translate(const Vec3f &dv);
	void rotate(const Vec3f &dr);

	void setPos(const Point_t &pt) { pos = pt; }
	void setRot(const Rot_t &rt) { rot = rt; }
	Point_t getPos() { return pos; }
	Rot_t getRot() { return rot; }

	pair<int, char*> serialize();
	void deserialize(char* newState);
	stateMessage st;
private:
	Point_t pos;
	Rot_t rot;
	list<Frame*> lsAttachedFrames;
};


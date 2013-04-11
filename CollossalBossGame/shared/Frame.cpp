#include "Frame.h"

Frame::Frame()
{
	this->pos = Vec3f();
	this->rot = Vec3f();
}

Frame::Frame(const Point_t &pos, const Rot_t &rot)
{
	this->pos = pos;
	this->rot = rot;
}


Frame::~Frame(void)
{
}

void Frame::addChild(Frame* newChild)
{
	lsAttachedFrames.push_back(newChild);
}


void Frame::translate(const Vec3f &dv) {
	pos.x += dv.x;
	pos.y += dv.y;
	pos.z += dv.z;
}

void Frame::rotate(const Vec3f &dr) {
	rot.x += dr.x;
	rot.y += dr.y;
	rot.z += dr.z;
}

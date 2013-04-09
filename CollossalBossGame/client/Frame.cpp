#include "Frame.h"

Frame::Frame()
{
	this->pos = Vec3f();
	this->rot = Vec3f();
}

Frame::Frame(Point_t pos, Rot_t rot)
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
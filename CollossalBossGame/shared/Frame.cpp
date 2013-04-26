#include "Frame.h"

Frame::Frame()
{
	this->pos = Vec3f();
	this->rot = Vec3f();
}

Frame::Frame(const Point_t &pos, const Quat_t &rot)
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

void Frame::rotate(const Quat_t &dr) {
	rot *= dr;
	rot.normalize();
}

int Frame::serialize(char * buf) {
	st.pos = pos;
	st.rot = rot;
	memcpy(buf, reinterpret_cast<char *>(&st), sizeof(st));
	return sizeof(st);
}

void Frame::deserialize(char* newState) {
	stateMessage* state = reinterpret_cast<stateMessage *>(newState);
	this->pos = state->pos;
	this->rot = state->rot;
}

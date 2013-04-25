/*
 * RenderModel.h
 * Contains the information necessary for rendering
 *
 * Authors: Nathan, Haro, Suman, Bryan
 */

#pragma once

#include "Frame.h"
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

#include <d3dx9.h>
#include "NetworkData.h"


class RenderModel
{
public:
	RenderModel(Point_t pos, Rot_t rot, Model modelNum, Vec3f scale);
	virtual ~RenderModel(void);
	virtual void render();
	virtual void setModelState(int state_id);
	Frame *getFrameOfRef() { return ref; }
private:
	Frame *ref;	//The skeleton
	Vec3f scale;
	int modelId;
	int modelState;
};

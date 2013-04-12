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
class RenderModel
{
public:
	RenderModel(Point_t pos, Rot_t rot);
	virtual ~RenderModel(void);

	virtual void render();
	Frame *getFrameOfRef() { return ref; }
private:
	LPDIRECT3DVERTEXBUFFER9 vbuf;
	Frame *ref;	//The skeleton
	Point_t verts[3];
	int skeletonGraphicId;
};

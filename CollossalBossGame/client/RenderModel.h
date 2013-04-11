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
};

 /*
#ifndef RENDER_MODEL_H
#define RENDER_MODEL_H
#include "defs.h"

#define CUSTOMFVF (D3DFVF_XYZRHW)

struct RENDERVERTEX
{
	FLOAT x, y, z, rhw; // from the D3DFVF_XYZRHW flag
	//rhw is a perspective flag. not sure why it's a float
};

class RenderModel {
public:
	RenderModel();
	~RenderModel();

	void update();
	void render();

private:
	RENDERVERTEX vertexes[];


}




#endif

*/

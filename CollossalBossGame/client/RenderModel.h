/*
 * RenderModel.h
 * Contains the information necessary for rendering
 *
 * Author: Bryan
 */

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
/* 
 * Models.h
 * Defining general model information
 *
 * Authors: Bryan
 */

#include "RenderEngine.h"

#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
	//rhw is a perspective flag. not sure why it's a float
	DWORD color;    // from the D3DFVF_DIFFUSE flag
};

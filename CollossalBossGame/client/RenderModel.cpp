/*
 * RenderModel.cpp
 * Contains the information necessary for rendering
 *
 * Author: Bryan
 */
#include "RenderModel.h"
#include "RenderEngine.h"

RenderModel::RenderModel(Point_t pos, Rot_t rot, const char * filename)
{
	//Create the reference frame
	ref = new Frame(pos, rot);

	if (!RE::get()->loadModel(filename, &modelId))
		DC::get()->print("Didn't load the model!");
}

RenderModel::~RenderModel(void)
{
}

void RenderModel::render() {
	Point_t pos = ref->getPos();
	Rot_t rot = ref->getRot();
	//Get translation/rotation matrix
	D3DXMATRIX trans, rotX, rotY, rotZ;
	D3DXMatrixIdentity(&trans);
	D3DXMatrixIdentity(&rotX);
	D3DXMatrixIdentity(&rotY);
	D3DXMatrixIdentity(&rotZ);

	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	D3DXMatrixRotationY(&rotX, rot.x);
	D3DXMatrixRotationY(&rotY, rot.y);
	D3DXMatrixRotationY(&rotZ, rot.z);

	//Render
	RE::get()->animate(modelId, RE::get()->getViewOffset() * trans * rotX * rotY * rotZ);
}


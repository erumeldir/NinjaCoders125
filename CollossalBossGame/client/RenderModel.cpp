/*
 * RenderModel.cpp
 * Contains the information necessary for rendering
 *
 * Author: Bryan
 */
#include "RenderModel.h"
#include "RenderEngine.h"
#include <CString>

/*RenderModel with const char
 * On bryan's system, loadModel can't be directly fed a const char so this
 * code converts a const char * to a char *, loads,
 * and then safely removes the allocated array. 
 *
 * Author: Bryan
 */
RenderModel::RenderModel(Point_t pos, Rot_t rot, const char * filename)
{
	string name = filename;
	char * nameAr = new char[name.size() + 1];
	strcpy (nameAr, filename);

	ref = new Frame(pos, rot);
	if (!RE::get()->loadModel(nameAr, &modelId))
		DC::get()->print("Didn't load the model from a constant string!");

	delete[] nameAr;
}

/* Render Model with char *
 *  loads the model, a conversion from const char * to char * doesn't
 *  work on bryan's system, but here's a version with the char *
 *  incase that would be useful (reading the model locations
 *  from a file for instance)
 *
 * Author: Bryan
 */
RenderModel::RenderModel(Point_t pos, Rot_t rot, char * filename)
{
	//Create the reference frame
	ref = new Frame(pos, rot);
	if (!RenderEngine::get()->loadModel(filename, &modelId))
		DC::get()->print("Didn't load the model from a char *!");
}

RenderModel::~RenderModel(void)
{
	DC::get()->print("A model with no inputs was created");
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


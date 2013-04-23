/*
 * RenderModel.cpp
 * Contains the information necessary for rendering
 *
 * Author: Bryan
 */
#include "RenderModel.h"
#include "RenderEngine.h"
#include "ConfigurationManager.h"
#include <sstream>

RenderModel::RenderModel(Point_t pos, Rot_t rot, Model modelNum)
{
	//Create the reference frame
	ref = new Frame(pos, rot);
	char *filename = NULL;
	switch(modelNum) {
	case MDL_0:
		filename = CM::get()->find_config("MODEL_0");
		break;
	case MDL_1:
		filename = CM::get()->find_config("MODEL_1");
		break;
	case MDL_2:
		filename = CM::get()->find_config("MODEL_2");
		break;
	case MDL_3:
		filename = CM::get()->find_config("MODEL_3");
		break;
	case MDL_4:
		filename = CM::get()->find_config("MODEL_4");
		break;
	case MDL_5:
		filename = CM::get()->find_config("MODEL_5");
		break;
	case MDL_6:
		filename = CM::get()->find_config("MODEL_6");
		break;
	case MDL_7:
		filename = CM::get()->find_config("MODEL_7");
		break;
	case MDL_8:
		filename = CM::get()->find_config("MODEL_8");
		break;
	case MDL_9:
		filename = CM::get()->find_config("MODEL_9");
		break;
	case MDL_10:
		filename = CM::get()->find_config("MODEL_10");
		break;
	case MDL_11:
		filename = CM::get()->find_config("MODEL_11");
		break;
	case MDL_12:
		filename = CM::get()->find_config("MODEL_12");
		break;
	case MDL_13:
		filename = CM::get()->find_config("MODEL_13");
		break;
	case MDL_14:
		filename = CM::get()->find_config("MODEL_14");
		break;
	case MDL_15:
		filename = CM::get()->find_config("MODEL_15");
		break;
	case MDL_16:
		filename = CM::get()->find_config("MODEL_16");
		break;
	case MDL_17:
		filename = CM::get()->find_config("MODEL_17");
		break;
	case MDL_18:
		filename = CM::get()->find_config("MODEL_18");
		break;
	case MDL_19:
		filename = CM::get()->find_config("MODEL_19");
		break;
	case MDL_20:
		filename = CM::get()->find_config("MODEL_20");
		break;
	case MDL_21:
		filename = CM::get()->find_config("MODEL_21");
		break;
	case MDL_22:
		filename = CM::get()->find_config("MODEL_22");
		break;
	case MDL_23:
		filename = CM::get()->find_config("MODEL_23");
		break;
	case MDL_24:
		filename = CM::get()->find_config("MODEL_24");
		break;
	default:
		if(modelNum > NUM_MDLS) DC::get()->print("ERROR: Model %d not known\n", modelNum);
	}

	if(filename != NULL) {
		if (HRESULT hr = !RE::get()->loadModel(filename, &modelId)) {
			DC::get()->print("Didn't load the model!\n");
		} else {
			DC::get()->print("Successfully loaded model %d\n",modelNum);
		}
		D3DXVECTOR3 mdlMin, mdlMax, sphereCenter;
		float rad;
		int numMesh;
		RE::get()->getAnim()->GetBoundingShapes(modelId,&mdlMin,&mdlMax,&sphereCenter,&rad,&numMesh);
	/*	DC::get()->print("Bounding box for model %d = (%f,%f,%f:%f,%f,%f); center = (%f,%f,%f); rad = %f; num meshes = %d\n",
			modelNum,
			mdlMin.x - sphereCenter.x, mdlMin.y - sphereCenter.y, mdlMin.z - sphereCenter.z,
			mdlMax.x - mdlMin.x, mdlMax.y - mdlMin.y, mdlMax.z - mdlMin.z,
			sphereCenter.x, sphereCenter.y, sphereCenter.z,
			rad, numMesh);
	*/
	}
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
	D3DXMatrixRotationX(&rotX, rot.x);
	D3DXMatrixRotationY(&rotY, rot.y);
	D3DXMatrixRotationZ(&rotZ, rot.z);

	//DC::get()->print("(%f,%f,%f), (%f,%f,%f)\n", pos.x, pos.y, pos.z, rot.x, rot.y, rot.z);

	//Render
	RE::get()->animate(modelId, rotX * rotY * rotZ * trans);
}


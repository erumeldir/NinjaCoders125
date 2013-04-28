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

RenderModel::RenderModel(Point_t pos, Rot_t rot, Model modelNum, int state, Vec3f scale)
{
	//Create the reference frame
	ref = new Frame(pos, rot);
	this->scale = scale;
	char *filename = NULL;
	bool isInvisible = false;
	switch(modelNum) {
	case -1: 
		//container object
		isInvisible = true;
		break;
	case MDL_TENTACLE_1:
		filename = CM::get()->find_config("MODEL_TENTACLE1");
		break;
	case MDL_TENTACLE_2:
		filename = CM::get()->find_config("MODEL_TENTACLE2");
		break;
	case MDL_TENTACLE_3:
		filename = CM::get()->find_config("MODEL_TENTACLE3");
		break;
	case MDL_TENTACLE_4:
		filename = CM::get()->find_config("MODEL_TENTACLE4");
		break;
	case MDL_TENTACLE_5:
		filename = CM::get()->find_config("MODEL_TENTACLE5");
		break;
	case MDL_FLOOR:
		filename = CM::get()->find_config("MODEL_FLOOR");
		break;
	case MDL_CEILING:
		filename = CM::get()->find_config("MODEL_CEILING");
		break;
	case MDL_EAST_WALL:
		filename = CM::get()->find_config("MODEL_EAST_WALL");
		break;
	case MDL_WEST_WALL:
		filename = CM::get()->find_config("MODEL_WEST_WALL");
		break;
	case MDL_NORTH_WALL:
		filename = CM::get()->find_config("MODEL_NORTH_WALL");
		break;
	case MDL_SOUTH_WALL:
		filename = CM::get()->find_config("MODEL_SOUTH_WALL");
		break;
	case MDL_PLAYER:
		filename = CM::get()->find_config("MODEL_PLAYER");
		break;
	case MDL_TEST_BOX:
		filename = CM::get()->find_config("MODEL_TEST_BOX");
		break;
	case MDL_TEST_PYRAMID:
		filename = CM::get()->find_config("MODEL_TEST_PYRAMID");
		break;
	case MDL_TEST_PLANE:
		filename = CM::get()->find_config("MODEL_TEST_PLANE");
		break;
	case MDL_TEST_BALL:
		filename = CM::get()->find_config("MODEL_TEST_BALL");
		break;
	default:
		if(modelNum > NUM_MDLS) DC::get()->print("ERROR: Model %d not known\n", modelNum);
	}

	if(filename != NULL && !isInvisible) {
		if (HRESULT hr = !RE::get()->loadModel(filename, &modelId)) {
			DC::get()->print("Didn't load the model!\n");
		} else {
			if (RE::get()->debugFlag) DC::get()->print("Successfully loaded model %d\n",modelNum);
			this->setModelState(state);
		}
	//	D3DXVECTOR3 mdlMin, mdlMax, sphereCenter;
	//	float rad;
	//	int numMesh;
	//	RE::get()->getAnim()->GetBoundingShapes(modelId,&mdlMin,&mdlMax,&sphereCenter,&rad,&numMesh);
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
	// negative id are for invisible/container objects
	if(modelId >= 0)
	{
		Point_t pos = ref->getPos();
		Rot_t rot = ref->getRot();

		//Get translation/rotation matrix
		D3DXMATRIX trans, rotX, rotY, rotZ, scaleMat;
		D3DXMatrixIdentity(&trans);
		D3DXMatrixIdentity(&rotX);
		D3DXMatrixIdentity(&rotY);
		D3DXMatrixIdentity(&rotZ);

		D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
		D3DXMatrixRotationX(&rotX, rot.x);
		D3DXMatrixRotationY(&rotY, rot.y);
		D3DXMatrixRotationZ(&rotZ, rot.z);

		D3DXMatrixScaling(&scaleMat,scale.x,scale.y,scale.z);  

		//DC::get()->print("(%f,%f,%f), (%f,%f,%f)\n", pos.x, pos.y, pos.z, rot.x, rot.y, rot.z);

		//Render
		DC::get()->print("ANIMATION STATE %d\n", modelState);
		RE::get()->getAnim()->ChangeAnimationSet(modelId, this->modelState);
		RE::get()->animate(modelId, scaleMat * rotX * rotY * rotZ * trans);
	}
}

void RenderModel::setModelState(int state_id) {
	this->modelState = state_id;
}

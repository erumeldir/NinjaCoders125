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

	if (!RenderEngine::loadModel(filename, &skeletonGraphicId))
		DC::get()->print("Didn't load the model!");
}

RenderModel::~RenderModel(void)
{
}

void RenderModel::render() {
	//put in render call here
	//TODO: change matrix
	RenderEngine::animate(skeletonGraphicId, RE::get()->getWorld());
}


 /////////////////////////////////////////////////
//
// File: "CAllocateHierarchy.cpp"
//
// Author: Jason Jurecka
//
// Creation Date: June 9, 2003
//
// Purpose: This is an Allocation class that is
//		used with the D3DXLoadMeshHierarchyFromX
//		function. It handles the Creation and Deletion
//		of Frames and Mesh Containers. The overloaded
//		functions are callbacks, so there is no need
//		to call any of the functions in written code
//		just pass an Object of this class to the function
/////////////////////////////////////////////////
#include "CAllocateHierarchy.h"

HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	//Convert the frame
	LPFRAME pFrame = (LPFRAME)pFrameToFree;

	// Delete the name
	SAFE_DELETE_ARRAY(pFrame->Name)
	
	// Delete the frame
	SAFE_DELETE(pFrame)

	// Returns an HRESULT so give it the AOk result
	return S_OK; 
}

HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	//Convert to my derived struct type
	LPMESHCONTAINER pMeshContainer = (LPMESHCONTAINER)pMeshContainerBase;
	
	// if there is a name
	SAFE_DELETE_ARRAY(pMeshContainer->Name)

	//if there are materials
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials9)

	//Release the textures
	if(pMeshContainer->ppTextures)
		for(UINT i = 0; i < pMeshContainer->NumMaterials; ++i)
				SAFE_RELEASE(pMeshContainer->ppTextures[i]);

	//if there are textures
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures)

	// if there is adjacency data
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency) 
	
	// if there are bone parts
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsets)
	
	//if there are frame matrices
	SAFE_DELETE_ARRAY(pMeshContainer->ppFrameMatrices)
	
	SAFE_DELETE_ARRAY(pMeshContainer->pAttributeTable)
	
	//if there is a copy of the mesh here
	SAFE_RELEASE(pMeshContainer->pSkinMesh)
	
	//if there is a mesh
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh)
	
	// if there is skin information
	SAFE_RELEASE(pMeshContainer->pSkinInfo)
	
	//Delete the mesh container
	SAFE_DELETE(pMeshContainer)
	
	// Returns an HRESULT so give it the AOk result
	return S_OK;
}

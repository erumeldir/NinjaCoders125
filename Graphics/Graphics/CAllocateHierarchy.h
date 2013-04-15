/////////////////////////////////////////////////
//
// File: "CAllocateHierarchy.h"
//
// Author: Jason Jurecka
//
// Creation Date: June 9, 2003
//
// Purpose: This is an Allocation class that is
//		used with the D3DXLoadMeshHierarchyFromX
//		function. It handles the Creation and Deletion
//		of Frames and Mesh Containers. The overloaded
//		functions are callbacks so there is no need
//		to call any of the functions in written code
//		just pass an Object of this class to the function
/////////////////////////////////////////////////

#ifndef _ALLOCATE_HIERARCHY_H_
#define _ALLOCATE_HIERARCHY_H_

#include "DerivedStructs.h"
#include "Macros.h"
#include <atlstr.h>

class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:

	// Create a frame
	//1. The name of the frame
	//2. The output new frame
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
	{    
		// Create a frame
		// Using my drived struct here
		LPFRAME pFrame = new FRAME;
		ZeroMemory(pFrame, sizeof(FRAME));

		// Inicilize the passed in frame
		*ppNewFrame = NULL;

		// Put the name in the frame
		if(Name)
		{
			int nNameSize = strlen(CStringA(Name))+1;
			pFrame->Name = new char[nNameSize];
			memcpy(pFrame->Name, Name, nNameSize*sizeof(char));
		}
		else
			pFrame->Name = NULL;
	
		// Inicilize the rest of the frame
		pFrame->pFrameFirstChild = NULL;
		pFrame->pFrameSibling = NULL;
		pFrame->pMeshContainer = NULL;
		D3DXMatrixIdentity(&pFrame->matCombined);
		D3DXMatrixIdentity(&pFrame->TransformationMatrix);

		// Set the output frame to the one that we have
		*ppNewFrame = (LPD3DXFRAME)pFrame;

		// It no longer points to the frame
		pFrame = NULL;

		// Returns an HRESULT so give it the AOk result
		return S_OK;
	}

	// Create a Mesh Container
	//1. Name of the Mesh
	//2. The mesh Data
	//3. that materials of the mesh
	//4. the effects on the mesh
	//5. the number of meterials in the mesh
	//6. the adjacency array for the mesh
	//7. the skin information for the mesh
	//8. the output mesh container
	
	STDMETHOD(CreateMeshContainer)(THIS_ 
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer) 
{

	// Create a Temp mesh contianer
	// Using my drived struct here
	LPMESHCONTAINER pMeshContainer = new MESHCONTAINER;
	ZeroMemory(pMeshContainer, sizeof(MESHCONTAINER));

	// Inicialize passed in Container
	*ppNewMeshContainer = NULL;

	if(Name)
	{
		// Put in the name
		int nNameSize = strlen(CStringA(Name))+1;
		pMeshContainer->Name = new char[nNameSize];
		memcpy(pMeshContainer->Name, Name, nNameSize*sizeof(char));
	}
	else
		pMeshContainer->Name = NULL;

	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	
	// Get the number of Faces for adjacency
	DWORD dwFaces = pMeshData->pMesh->GetNumFaces();

	//Get Initcilize all the other data
	pMeshContainer->NumMaterials = NumMaterials;

	//Create the arrays for the materials and the textures
	pMeshContainer->pMaterials9 = new D3DMATERIAL9[pMeshContainer->NumMaterials];

	// Multiply by 3 because there are three adjacent triangles
	pMeshContainer->pAdjacency = new DWORD[dwFaces*3];
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * dwFaces*3);

	
	//Get the device to use
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;// Direct3D Rendering device
	pMeshData->pMesh->GetDevice(&pd3dDevice);

	pMeshData->pMesh->CloneMeshFVF(D3DXMESH_MANAGED, 
		pMeshData->pMesh->GetFVF(), pd3dDevice, 
		&pMeshContainer->MeshData.pMesh);
	
	pMeshContainer->ppTextures  = new LPDIRECT3DTEXTURE9[NumMaterials];
	for(DWORD dw = 0; dw < NumMaterials; ++dw)
	{
		pMeshContainer->ppTextures [dw] = NULL;

		if(pMaterials[dw].pTextureFilename && strlen(pMaterials[dw].pTextureFilename) > 0)
		{
			//POSSIBLE ERROR: typecast for pMaterials[dw].pTextureFilename. It wants a unicode, we're presenting ascii
			if(FAILED(D3DXCreateTextureFromFile(pd3dDevice, 
				(LPCWSTR)pMaterials[dw].pTextureFilename, &pMeshContainer->ppTextures[dw])))
					pMeshContainer->ppTextures [dw] = NULL;
		}
	}

	//Release the device
	SAFE_RELEASE(pd3dDevice);

	if(pSkinInfo)
	{
		// first save off the SkinInfo and original mesh data
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		// Will need an array of offset matrices to move the vertices from 
		//	the figure space to the bone's space
		UINT uBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsets = new D3DXMATRIX[uBones];

		//Create the arrays for the bones and the frame matrices
		pMeshContainer->ppFrameMatrices = new D3DXMATRIX*[uBones];

		// get each of the bone offset matrices so that we don't need to 
		//	get them later
		for (UINT i = 0; i < uBones; i++)
			pMeshContainer->pBoneOffsets[i] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
	}
	else
	
	{
		pMeshContainer->pSkinInfo = NULL;
		pMeshContainer->pBoneOffsets = NULL;
		pMeshContainer->pSkinMesh = NULL;
		pMeshContainer->ppFrameMatrices = NULL;
	}

	pMeshContainer->pMaterials = NULL;
	pMeshContainer->pEffects = NULL;

	//pMeshContainer->MeshData.pMesh->OptimizeInplace(
	//	D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT,
	//	pMeshContainer->pAdjacency,NULL,NULL,NULL);

	// Set the output mesh container to the temp one
	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

	// Returns an HRESULT so give it the AOk result
	return S_OK;
}

	// Destroy a frame
	//1. The frame to delete
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	
	// Destroy a mesh container
	//1. The container to destroy
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};

#endif
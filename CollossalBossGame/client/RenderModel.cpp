#include "RenderModel.h"
#include "RenderEngine.h"



#define CUSTOMFVF (D3DFVF_XYZRHW)

struct RENDERVERTEX
{
	FLOAT x, y, z, rhw; // from the D3DFVF_XYZRHW flag
	//rhw is a perspective flag. not sure why it's a float
};

RenderModel::RenderModel(Point_t pos, Rot_t rot)
{
	//Create the reference frame
	ref = new Frame(pos, rot);
	verts[0] = Point_t(-100.f, -100.f, 0.f);
	verts[1] = Point_t(100.f, -100.f, 0.0f);
	verts[2] = Point_t(0.f, 100.f, 0.0f);

	//now initialize any models we want to have first
	// create three vertices using the RENDERVERTEX struct built earlier
	RENDERVERTEX vertices[] =
	{
		{ verts[0].x, verts[0].y, verts[0].z, 1.0f, },
		{ verts[1].x, verts[1].y, verts[1].z, 1.0f, },
		{ verts[2].x, verts[2].y, verts[2].z, 1.0f, },
	};

	// create the vertex and store the pointer into vertexBuffer, which is created globally
	RE::get()->direct3dDevice->CreateVertexBuffer(
		3*sizeof(RENDERVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&vbuf,
		NULL
	);

	VOID* vertexInfo; // the void pointer

	vbuf->Lock(0, 0, (void**)&vertexInfo, 0); // lock the vertex buffer
	memcpy(vertexInfo, vertices, sizeof(vertices)); // copy the vertices to the locked buffer
	vbuf->Unlock(); // unlock the vertex buffer
}


RenderModel::~RenderModel(void)
{
}

void RenderModel::render() {
	printf("Pos: %d, %d, %d                                       \r",ref->getPos().x, ref->getPos().y, ref->getPos().z); 
		// select which vertex format we are using
	RE::get()->direct3dDevice->SetFVF(CUSTOMFVF);

	//while the queue has objects, update
	//TODO: Object queue

	//TODO: while queue has stuff, loop
	//messy code: want to get the model info, then store that.
	float cosT = cos(ref->getRot().z),
		  sinT = sin(ref->getRot().z);
	Point_t pt0, pt1, pt2;
	
	pt0.x = (verts[0].x) * cosT - (verts[0].y) * sinT + ref->getPos().x;
	pt0.y = (verts[0].y) * cosT + (verts[0].x) * sinT + ref->getPos().y;
	pt0.z = 0;
	pt1.x = (verts[1].x) * cosT - (verts[1].y) * sinT + ref->getPos().x;
	pt1.y = (verts[1].y) * cosT + (verts[1].x) * sinT + ref->getPos().y;
	pt1.z = 0;
	pt2.x = (verts[2].x) * cosT - (verts[2].y) * sinT + ref->getPos().x;
	pt2.y = (verts[2].y) * cosT + (verts[2].x) * sinT + ref->getPos().y;
	pt2.z = 0;

	RENDERVERTEX vertices[] =
	{
		{ pt0.x, pt0.y, pt0.z, 1.0f, },
		{ pt1.x, pt1.y, pt1.z, 1.0f, },
		{ pt2.x, pt2.y, pt2.z, 1.0f, },
	};

	RE::get()->direct3dDevice->CreateVertexBuffer(
		3*sizeof(RENDERVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&vbuf,
		NULL
	);

	VOID* vertexInfo; // the void pointer

	vbuf->Lock(0, 0, (void**)&vertexInfo, 0); // lock the vertex buffer
	memcpy(vertexInfo, vertices, sizeof(vertices)); // copy the vertices to the locked buffer
	vbuf->Unlock(); // unlock the vertex buffer

	//end messy code

	// select the vertex buffer to display
	RE::get()->direct3dDevice->SetStreamSource(0, vbuf, 0, sizeof(RENDERVERTEX));

	// copy the vertex buffer to the back buffer
	RE::get()->direct3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}
/*
 * RenderEngine.h
 * Defines the render engine class.
 * This object is specific to the client.
 */

#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <d3d9.h>
#include <d3dx9.h>

class RenderEngine {
public:
	static void init() { re = new RenderEngine(); }
	static RenderEngine *get() { return re; }
	static void clean() { delete re; }

	void render();

private:
	RenderEngine();
	virtual ~RenderEngine();

	static RenderEngine *re;
};
typedef RenderEngine RE;

#endif

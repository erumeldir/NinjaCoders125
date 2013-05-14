#pragma once
#include "ParticleSystem.h"
class SmokeEffect :
	public ParticleSystem
{
public:
	SmokeEffect(void);
	virtual ~SmokeEffect(void);
	virtual void resetParticle(ParticleAttributes* a);
	virtual void update(float timeDelta);
	virtual void setPosition(Vec3f pos);
	virtual void preRender(LPDIRECT3DDEVICE9 direct3dDevice);
	virtual void postRender(LPDIRECT3DDEVICE9 direct3dDevice);
	D3DXVECTOR3 pos;

};


#pragma once
#include "ParticleSystem.h"
class ChargeEffect :
	public ParticleSystem
{
public:
	ChargeEffect(int numParticles);
	~ChargeEffect(void);
	virtual void resetParticle(ParticleAttributes* a);
	virtual void update(float timeDelta);
	virtual void setPosition(Vec3f pos, int charge);
	void killAllParticles();

	int numParticles;
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
};


#include "ChargeEffect.h"


ChargeEffect::ChargeEffect(int numParticles)
{
	vbSize = 2048;
	vbOffset = 0;
	vbBatchSize = 512;
	pointSize = 1.5f;
	for(int i= 0; i < numParticles; i++) addParticle();
}


ChargeEffect::~ChargeEffect(void)
{
}


void ChargeEffect::resetParticle(ParticleAttributes* a)
{
	a->isAlive = true;
	getRandVec(&(a->pos),&min, &max); 

	a->pos.x = (getRandFloat(min.x,max.x));
	a->pos.y = (getRandFloat(min.y,max.y));
	a->pos.z = (getRandFloat(min.z,max.z));
	a->vel.x = (getRandFloat(-1,1));
	a->vel.y = (getRandFloat(-1,1));
	a->vel.z = (getRandFloat(-1,1));
	a->color = D3DXCOLOR(1.0f,1.0f,0.0f,1.0f);
}

void ChargeEffect::update(float timeDelta)
{
	list<ParticleAttributes>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++)
	{
		i->pos += i->vel * timeDelta;
		//inside?
		if(i->pos.x < min.x || i->pos.x > max.x
			|| i->pos.y < min.y || i->pos.y > max.y
			|| i->pos.z < min.z || i->pos.z > max.z)
		{
			resetParticle(&(*i));
		}
	}
	
}

void ChargeEffect::setPosition(Vec3f pos)
{
	min = D3DXVECTOR3(pos.x-5, 10, pos.z - 5);
	max = D3DXVECTOR3(pos.x + 5, 30, pos.z + 5);
}
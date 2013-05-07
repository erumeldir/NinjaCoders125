#include "ChargeEffect.h"


ChargeEffect::ChargeEffect(int numParticles)
{
	vbSize = 2048;
	vbOffset = 0;
	vbBatchSize = 512;
	pointSize = 5.0f;
	this->numParticles = numParticles;
	//for(int i= 0; i < numParticles; i++) addParticle();
	charge = 0; // todo horrendous
	killCounter = 2;
}


ChargeEffect::~ChargeEffect(void)
{
}


void ChargeEffect::resetParticle(ParticleAttributes* a)
{

	a->isAlive = true;
	a->pos = middle;

	min = D3DXVECTOR3(-1.0f,-1.0f,-1.0f);
	max = D3DXVECTOR3(1.0f,1.0f,1.0f);

	this->getRandVec(&a->vel, &min, &max);

	D3DXVec3Normalize(&a->vel, &a->vel);

	a->vel *= 7;  

	a->color = D3DXCOLOR(this->getRandFloat(0.0f,1.0f), this->getRandFloat(0.0f,1.0f), this->getRandFloat(0.0f,1.0f), 1.0f);

	a->age = 0;
	a->lifetime = 2;

	////a->isAlive = true;
	//a->lifetime = 10;
	//a->age = 0;
	//getRandVec(&(a->pos),&min, &max); 
	//a->pos.x = (getRandFloat(min.x,max.x));
	//a->pos.y = (getRandFloat(min.y,max.y));
	//a->pos.z = (getRandFloat(min.z,max.z));
	///*a->vel.x = (getRandFloat(-1,1));
	//a->vel.y = (getRandFloat(-1,1));
	//a->vel.z = (getRandFloat(-1,1));*/
	//a->color = D3DXCOLOR(1.0f,0.0f,1.0f,1.0f);
}

void ChargeEffect::update(float timeDelta)
{
	this->removeDeadParticles();

	if (charge == 0) killCounter--;
	else killCounter = 20;

	if (killCounter < 0) particles.clear();

	list<ParticleAttributes>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++)
	{
		//if(i->isAlive)
		//{
			i->age+=timeDelta;
			if(i->age > i->lifetime) 
			{
				//if (killCounter > 0)
					resetParticle(&(*i));
				//else
				//	i->isAlive = false;
			}

			i->pos += i->vel * timeDelta;
			/*if(i->pos.x < min.x || i->pos.x > max.x
				|| i->pos.y < min.y || i->pos.y > max.y
				|| i->pos.z < min.z || i->pos.z > max.z)
			{
				resetParticle(&(*i));
			}*/
		//}
	}
}

void ChargeEffect::setPosition(Vec3f pos, int charge)
{
	static int counter = 0;
	this->charge = charge;
	//min = D3DXVECTOR3(pos.x-5, pos.y, pos.z - 5);
	//max = D3DXVECTOR3(pos.x + 5, pos.y + 1, pos.z + 5);
	middle = D3DXVECTOR3(pos.x, pos.y, pos.z);
	//if(counter == 0) 

	list<ParticleAttributes>::iterator i;
	if(charge > 0 && charge < 14)
	{
		list<ParticleAttributes>::iterator i;
		//for(i = particles.begin(); i != particles.end(); i++)
		//{
		//	if(!i->isAlive) i->isAlive = true;
		//}
		if(particles.size() < 100)
		{
			int index = 0;
			while(index < numParticles)
			{
				addParticle();
				index++;
			}
			counter++;
		}
	}
	else
	{
		//int index = 0;
		//for(i = particles.begin(); i != particles.end() /*&& index < 2*numParticles*/; i++)
		//{
		//	if (i->age > i->lifetime)
		//		i->isAlive = false;
		//	//index++;
		//}
	//	
	//	//killAllParticles();
	//	counter = 0;
	//	particles.clear();
	}
}

void ChargeEffect::killAllParticles()
{
	//particles.clear();
}
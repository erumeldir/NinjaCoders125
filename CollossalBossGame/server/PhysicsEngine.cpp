#include "PhysicsEngine.h"
#define TIMESTEP 5
#define SMALLRADIUS 5.0f
#define LARGERADIUS 20.0f

//Static members
PhysicsEngine *PhysicsEngine::pe;

PhysicsEngine::PhysicsEngine(void)
{
}


PhysicsEngine::~PhysicsEngine(void)
{
}


/*
 * applyPhysics(PhysicsModel *mdl)
 * Updates the physical position of the model and returns true if this object
 * should have collision physics applied.
 */
bool PhysicsEngine::applyPhysics(PhysicsModel *mdl) {
	float dt = TIMESTEP;

	//Apply additional forces, such as gravity and friction.
	// We are ignoring both for now and applying a half-assed version of
	// friction when we update the velocity.

	//Update position
	float dx = 0.5 * mdl->accel.x * dt * dt + mdl->vel.x * dt,
		  dy = 0.5 * mdl->accel.y * dt * dt + mdl->vel.y * dt,
		  dz = 0.5 * mdl->accel.z * dt * dt + mdl->vel.z * dt;
	mdl->ref->translate(Point_t(dx, dy, dz));

	//Update velocity
	mdl->vel.x = mdl->accel.x * dt + mdl->vel.x / mdl->frictCoeff;
	mdl->vel.y = mdl->accel.y * dt + mdl->vel.y / mdl->frictCoeff;
	mdl->vel.z = mdl->accel.z * dt + mdl->vel.z / mdl->frictCoeff;

	//Update acceleration
	mdl->accel = Vec3f();

	return true;	//We'll add a detection for has-moved later
}

/*applyPhysics: figures out if the two objects collide and how to react
 *
 *	General Note: Currently dealing only in spheres
 *  Author: Bryan
 *  Alex note: separate detection and collision response
 */

#include <iostream>
void PhysicsEngine::applyPhysics(ServerObject *obj1, ServerObject *obj2) {
	//Collision checks/inter-object physics
	
	//1. get their positions types (eventually size as well?)
	Point_t first = obj1->getPhysicsModel()->ref->getPos();
	Point_t second = obj2->getPhysicsModel()->ref->getPos();

	//2. get radii of each object
	CollisionBox size1 = obj1->getPhysicsModel()->getColBox();	
	CollisionBox size2 = obj2->getPhysicsModel()->getColBox();

	float r1 = (size1 == CB_SMALL) ? SMALLRADIUS : LARGERADIUS;
	float r2 = (size2 == CB_SMALL) ? SMALLRADIUS : LARGERADIUS;

	//3. calculate the distance
	float distX = first.x - second.x;
	float distY = first.y - second.y;
	float distZ = first.z - second.z;

	float squareDist = distX * distX + distY * distY + distZ * distZ;
	
	PhysicsModel * p1 = obj1->getPhysicsModel();
	PhysicsModel * p2 = obj2->getPhysicsModel();

	//4. find if we collided
	bool collidedX = distX * distX < (r1 + r2) * (r1 + r2);
	bool collidedY = distY * distY < (r1 + r2) * (r1 + r2);
	bool collidedZ = distZ * distZ < (r1 + r2) * (r1 + r2);

	
		float vx1 = obj1->getPhysicsModel()->vel.x;
		float vx2 = obj2->getPhysicsModel()->vel.x;
		
		float vy1 = obj1->getPhysicsModel()->vel.y;
		float vy2 = obj2->getPhysicsModel()->vel.y;

		float vz1 = obj1->getPhysicsModel()->vel.z;
		float vz2 = obj2->getPhysicsModel()->vel.z;

		float vx21 = obj2->getPhysicsModel()->vel.x - obj1->getPhysicsModel()->vel.x;
		float vy21 = obj2->getPhysicsModel()->vel.y - obj1->getPhysicsModel()->vel.y;
		float vz21 = obj2->getPhysicsModel()->vel.z - obj1->getPhysicsModel()->vel.z;

		float m1 = obj1->getPhysicsModel()->mass;
		float m2 = obj2->getPhysicsModel()->mass;

		float pi2=2*acos(-1.0E0);
		float m21 = m2 / m1;
		float x21 = obj2->getPhysicsModel()->ref->getPos().x - obj1->getPhysicsModel()->ref->getPos().x;
		float y21 = obj2->getPhysicsModel()->ref->getPos().y - obj1->getPhysicsModel()->ref->getPos().y;
		float z21 = obj2->getPhysicsModel()->ref->getPos().z - obj1->getPhysicsModel()->ref->getPos().z;
		float gammav=atan2(-vy21,-vx21);
		float gammaxy=atan2(y21,x21);
		float dgamma=gammaxy-gammav;
		float d=sqrt(x21*x21 +y21*y21+z21*z21);
        if (dgamma>pi2) 
			{dgamma=dgamma-pi2;}
		else if (dgamma<-pi2) 
			{dgamma=dgamma+pi2;}
		
		float dr=d*sin(dgamma)/r1+r2;

	//Dealing with collisions!
	if (d<r1 + r2)
	{
		
		float vx_cm = (m1*vx1+m2*vx2)/(m1+m2) ;
		float vy_cm = (m1*vy1+m2*vy2)/(m1+m2) ;

//     *** I have inserted the following statements to avoid a zero divide; 
//         (for single precision calculations, 
//          1.0E-12 should be replaced by a larger value). **************  
  
       float fy21=1.0E-12*fabs(y21);
	   float sign;
       if ( fabs(x21)<fy21 ) {  
                   if (x21<0) { sign=-1; } else { sign=1;}  
                   x21=fy21*sign; 
        } 

	   float a, dvx2;
//     ***  update velocities ***
       a=y21/x21;
       dvx2= -2*(vx21 +a*vy21)/((1+a*a)*(1+m21)) ;
       vx2=vx2+dvx2;
       vy2=vy2+a*dvx2;
       vx1=vx1-m21*dvx2;
       vy1=vy1-a*m21*dvx2;

	   float R = r1;
//     ***  velocity correction for inelastic collisions ***
       vx1=(vx1-vx_cm)*R + vx_cm;
       vy1=(vy1-vy_cm)*R + vy_cm;
       vx2=(vx2-vx_cm)*R + vx_cm;
       vy2=(vy2-vy_cm)*R + vy_cm;
	   
	   obj1->getPhysicsModel()->vel = Vec3f(vx1, vy1, obj1->getPhysicsModel()->vel.z);
	   obj2->getPhysicsModel()->vel = Vec3f(vx2, vy2, obj2->getPhysicsModel()->vel.z);
//		cout << "squareDist: " << squareDist << ", things: " << (r1 + r2) * (r1 + r2) + (r1 + r2) * (r1 + r2) + (r1 + r2) * (r1 + r2)<< endl;
		/*General principle: conservation of momentum 
		 *	If we were to have this be absolutely accurate, we would need
		 *   a gauge of elasticity for both objects. However since we don't
		 *   have elasticity, we will instead use a difference of mass as 
		 *   a factor.
		 *
		 */

		//0. set acceleration
		obj1->getPhysicsModel()->accel = Vec3f(0, 0, 0);
		obj2->getPhysicsModel()->accel = Vec3f(0, 0, 0);

		//1. calculate momentum. 
		Vec3f moment = Vec3f(obj1->getPhysicsModel()->mass * obj1->getPhysicsModel()->vel.x
								+ obj2->getPhysicsModel()->mass * obj2->getPhysicsModel()->vel.x,
								obj1->getPhysicsModel()->mass * obj1->getPhysicsModel()->vel.y
								+ obj2->getPhysicsModel()->mass * obj2->getPhysicsModel()->vel.y,
								obj1->getPhysicsModel()->mass * obj1->getPhysicsModel()->vel.z
								+ obj2->getPhysicsModel()->mass * obj2->getPhysicsModel()->vel.z);

		//2. calculate the percent difference of masses to indicate the final direction of obj1
		float direction = (obj1->getPhysicsModel()->mass - obj2->getPhysicsModel()->mass) 
								/(obj1->getPhysicsModel()->mass + obj2->getPhysicsModel()->mass);

		//Now we have two tasks: set the positions of the objects appropriately (A) and 
		// setting the velocities appropriately (B)

		//A. Set the positions, which we'll base on the difference.
		//1. We need the difference that we need to make up. 
		Vec3f positionDif = Vec3f (r1+r2 - distX, r1+r2 - distY, r1+r2 - distZ);

		//2. the amounts that we need to move
		Vec3f divisor = obj1->getPhysicsModel()->vel + obj2->getPhysicsModel()->vel;
		Vec3f move1a = (obj1->getPhysicsModel()->vel / divisor);
		Vec3f move1 = move1a * positionDif;
		Vec3f move2a = obj2->getPhysicsModel()->vel / divisor;
		Vec3f move2 = move2a * positionDif;

		//3. do the movement
		Vec3f set1 = obj1->getPhysicsModel()->vel - move1;
		Vec3f set2 = obj2->getPhysicsModel()->vel - move2;
	/*	obj1->getPhysicsModel()->ref->setPos(set1);
		obj2->getPhysicsModel()->ref->setPos(set2);
	
		*/
		
		Vec3f force = obj1->getPhysicsModel()->vel * direction;
		//B. Set the velocities
		obj1->getPhysicsModel()->applyForce( force );

		Vec3f firstMom = Vec3f( obj1->getPhysicsModel()->vel.x * obj1->getPhysicsModel()->mass,
			obj1->getPhysicsModel()->vel.y * obj1->getPhysicsModel()->mass,
			obj1->getPhysicsModel()->vel.z * obj1->getPhysicsModel()->mass);

		//I got to override an operator! :D
		moment = moment - firstMom;

		//Overriding crazy! Also storing the final velocity in it
		obj2->getPhysicsModel()->applyForce(moment / obj2->getPhysicsModel()->mass * direction);
		
	}

}


#pragma once

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9d.lib")
#include <d3dx9.h>
#include "defs.h"

/**
 * Camera that follows player around
 *
 * Based off of this tutorial:
 * http://sasquatchsoft.blogspot.com/2008/01/building-third-person-camera-with.html
 * Thanks sasquatch! =D
 *
 * Author(s): Suman, Haro
 */
class Camera
{
public:
	Camera(float distance);
	~Camera(void);
#if 0
	void viewTarget(); // View target

	// Relative position/rotation
	void yaw(float angle); // Perform a yaw
	void pitch(float angle); // Perform a pitch
	void forward(float distance); // Move forward
	void backward(float distance); // Move backwards
	void left(float distance); // Move left
	void right(float distance); // Move right

	// Absolute position/rotation
	void setYaw(float angle); // left-right rotation
	void setPitch(float angle);
	void setTargetPosAndRot(const Point_t &pos, const Rot_t &rot);	//Sets position and rotation of the target
	void setTargetUp(const Vec3f &up);
#endif
	//New camera update information
	void update(const Point_t &tarPos, const Quat_t &tarRot, float pitch);

	//void setViewProps(const Point_t &tarPos, const Vec3f &tarDir, const Vec3f &up);

	D3DXMATRIX* getViewMatrix() { return &viewMatrix; }

private:
	D3DXMATRIX viewMatrix;
	float viewDistance;
#if 0
	// Vectors for the target and camera
	D3DXVECTOR3 tarView, tarUp, tarRight, tarPos;
	D3DXVECTOR3 camView, camUp, camRight, camPos;
	// Store the pitch and distance
	float currentPitch;
#endif
};


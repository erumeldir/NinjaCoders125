#include "Camera.h"


Camera::Camera(float distance)
{
	tarPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // Set at origin
	tarView = D3DXVECTOR3(0.0f, 0.0f, 1.0f); // Look along Z-axis
	tarUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // Normal along Y-axis
	tarRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f); // Right along X-axis

	viewDistance = distance;

	// Set camera to be essentially the same as the target
	camPos = tarPos; // Pos will auto updated in the View routine
	camView = tarView;
	camUp = tarUp;
	camRight = tarRight;

	currentPitch = 0;
}


Camera::~Camera(void)
{
}

/**
 * Rotates left and right
 */
void Camera::yaw(float angle)
{
	// Perform a rotation around the up vector of the target
	D3DXMATRIX yaw; // create a matrix to hold the rotation
	D3DXMatrixRotationAxis(&yaw, &tarUp, angle);

	// Update the view vectors of both the target and camera based on the rotation represented in our yaw matrix
	D3DXVec3TransformCoord(&camView, &camView, &yaw);
	D3DXVec3TransformCoord(&tarView, &tarView, &yaw);
	// This next part is so the rotation is absolute and not relative
	//D3DXVECTOR3 newCamView = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//D3DXVECTOR3 newTarView = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//D3DXVec3TransformCoord(&camView, &newCamView, &yaw);
	//D3DXVec3TransformCoord(&tarView, &newTarView, &yaw);

	// Also update the right vectors of the target and camera
	D3DXVec3TransformCoord(&camRight, &camRight, &yaw);
	D3DXVec3TransformCoord(&tarRight, &tarRight, &yaw);
	/*D3DXVECTOR3 newCamRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 newTarRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&camRight, &newCamRight, &yaw);
	D3DXVec3TransformCoord(&tarRight, &newTarRight, &yaw);*/
}

void Camera::setYaw(float angle)
{
	static float lastYaw = 0;
	yaw(angle - lastYaw);
	lastYaw = angle;
}

/**
 * Rotates up and down
  * TODO Radians PI/2
 */
void Camera::pitch(float angle)
{
	// Make sure our angle is within bounds
	if (currentPitch + angle > - 45 && currentPitch + angle < 90)
	{
		// Perform a rotation around the right vector of the target
		D3DXMATRIX pitch; // create a matrix to hold the rotation
		D3DXMatrixRotationAxis(&pitch, &tarRight, D3DXToRadian(angle));

		// Update the view vector of the camera to reflect the rotation
		D3DXVec3TransformCoord(&camView, &camView, &pitch);

		// Also update the up vector of the camera
		D3DXVec3TransformCoord(&camUp, &camUp, &pitch);

		currentPitch += angle;
	}
}

void Camera::forward(float distance)
{
	D3DXVECTOR3 movVec = distance * tarView;
	tarPos += movVec;
}

void Camera::backward(float distance)
{
	D3DXVECTOR3 movVec = -1 * distance * tarView;
	tarPos += movVec;
}

void Camera::right(float distance)
{
	D3DXVECTOR3 movVec = distance * tarRight;
	tarPos += movVec;
}

void Camera::left(float distance)
{
	D3DXVECTOR3 movVec = -1 * distance * tarRight;
	tarPos += movVec;
}

void Camera::setX(float x)
{
	float oldX = D3DXVec3Dot(&tarPos, &tarRight);
	right(x - oldX);
}

void Camera::setZ(float z)
{
	float oldZ = D3DXVec3Dot(&tarPos, &tarView);
	forward(z - oldZ);
}

/**
 * Updates view matrix
 */
void Camera::viewTarget()
{
	// Normalize view, up and right vectors for target and ensure they are mutually orthogonal
	D3DXVec3Normalize(&tarView, &tarView);

	D3DXVec3Cross(&tarUp, &tarView, &tarRight);
	D3DXVec3Normalize(&tarUp, &tarUp);

	D3DXVec3Cross(&tarRight, &tarUp, &tarView);
	D3DXVec3Normalize(&tarRight, &tarRight);

	// Do the same for the camera’s vectors
	D3DXVec3Normalize(&camView, &camView);

	D3DXVec3Cross(&camUp, &camView, &camRight);
	D3DXVec3Normalize(&camUp, &camUp);

	D3DXVec3Cross(&camRight, &camUp, &camView);
	D3DXVec3Normalize(&camRight, &camRight);

	// Calculate camera position
	camPos = tarPos - (viewDistance * camView);

	// Construct the matrix based on the matrix defined in the first section
	(viewMatrix)(0, 0) = camRight.x;
	(viewMatrix)(1, 0) = camRight.y;
	(viewMatrix)(2, 0) = camRight.z;
	(viewMatrix)(3, 0) = -D3DXVec3Dot(&camRight, &camPos);

	(viewMatrix)(0, 1) = camUp.x;
	(viewMatrix)(1, 1) = camUp.y;
	(viewMatrix)(2, 1) = camUp.z;
	(viewMatrix)(3, 1) = -D3DXVec3Dot(&camUp, &camPos);

	(viewMatrix)(0, 2) = camView.x;
	(viewMatrix)(1, 2) = camView.y;
	(viewMatrix)(2, 2) = camView.z;
	(viewMatrix)(3, 2) = -D3DXVec3Dot(&camView, &camPos);

	(viewMatrix)(0, 3) = 0.0f;
	(viewMatrix)(1, 3) = 0.0f;
	(viewMatrix)(2, 3) = 0.0f;
	(viewMatrix)(3, 3) = 1.0f;
}
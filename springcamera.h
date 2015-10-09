#ifndef _SPRINGCAMERA_H_
#define _SPRINGCAMERA_H_

#include "classlist.h"
#include "vector.h"
#include "quaternion.h"

#include "helpers.h"
#include "basecamera.h"

class SpringCamera : public BaseCamera{
public:
	SpringCamera();

	bool Initialize(float positionWeight, float springStep);
	bool Initialize(char * datafile);

	Vector GetLookVector();
	Vector GetPosition();
	void Update(Vector targetPosition);
	void Reset();

	void Render(float t);
	void GetViewMatrix(D3DXMATRIX& viewMatrix);
	void GetGUIWorldMatrix(D3DXMATRIX& worldMatrix);

private:
	//position and velocity of camera
	Vector position;
	Vector velocity;
	
	Vector look;  //direction camera is looking in

	Vector targetPosition;  //position of target camera is following

	Quaternion rotation;  //rotation of camera (not currently used)


	//weights for spring
	float vweight;
	float dweight;
	float step;

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_worldMatrix;
};

#endif
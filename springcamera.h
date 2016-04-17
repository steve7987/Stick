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

	Vector GetLookVector();
	Vector GetPosition();
	void Update(Vector targetPosition, float t);  //updates the camera position based on spring movement
	void Reset(Vector position);
	void SetRotation(Quaternion rotate);
	
	Quaternion GetRotation();
	void Render(float t);
	void GetViewMatrix(D3DXMATRIX& viewMatrix);
	void GetGUIWorldMatrix(D3DXMATRIX& worldMatrix);

	void SetFieldOfView(float fov);
	float GetFieldOfView();

	Vector GetFinalPosition();

private:
	//position and velocity of camera
	Vector position;
	Vector velocity;
	
	Vector look;  //direction camera is looking in

	Vector targetPosition;  //position of target camera is following

	Quaternion rotation;  //rotation of camera 


	//weights for spring
	float vweight;
	float dweight;
	float step;

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_worldMatrix;
	float m_fieldOfView;
};

#endif
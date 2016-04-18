#include "springcamera.h"

SpringCamera::SpringCamera(){

}

bool SpringCamera::Initialize(float positionWeight, float springStep){
	this->vweight = -2*positionWeight;
	this->dweight = -1*positionWeight*positionWeight;
	step = springStep;
	position = Vector(0,0,0);
	targetPosition = Vector(0,0,0);
	velocity = Vector(0,0,0);
	look = Vector(1,0,0);  //camera always looks forward in x direction 
	rotation = Quaternion(Vector(1,0,0), 0);
	m_fieldOfView = PI / 4;  //probably overwritten in actionstate

	return true;
}

Vector SpringCamera::GetLookVector(){
	return look;
}

Vector SpringCamera::GetPosition(){
	Vector target = position + look;
	Vector finalPos = target - rotation* look;
	return finalPos;
}

void SpringCamera::Update(Vector targetPosition, float t){
	this->targetPosition = targetPosition;
	//simulate spring deq
	while (t > step){
		Vector accel = vweight * velocity + dweight * (position - targetPosition);
		velocity = velocity + accel * step / 1000.0f;
		position = position + velocity * step / 1000.0f;
		t = t - step;
	}
	Vector accel = vweight * velocity + dweight * (position - targetPosition);
	velocity = velocity + accel * t / 1000.0f;
	position = position + velocity * t / 1000.0f;
}
	
void SpringCamera::Reset(Vector position){
	this->position = position;
	targetPosition = position;
	velocity = Vector(0,0,0);
	look = Vector(1,0,0);  //camera always looks forward in x direction 
	rotation = Quaternion(Vector(1,0,0), 0);
}

void SpringCamera::SetRotation(Quaternion rotate){
	rotation = rotate;
}

Quaternion SpringCamera::GetRotation(){
	return rotation;
}

void SpringCamera::Render(float t){
	Vector desiredPosition = targetPosition;
	Vector up = Vector(0, 1, 0);
	//rotate up vector by rotation around <1,0,0> of rotation quaternion
	Quaternion upRot = rotation;
	upRot.y = 0;
	upRot.z = 0;
	upRot = upRot / sqrt(upRot.x * upRot.x + upRot.w * upRot.w);
	up = upRot * up;
	

	Vector target = position + look;
	Vector finalPos = target - rotation* look;
	//create matricies
	D3DXMatrixLookAtLH(&m_viewMatrix, &(finalPos).d3dvector(), &(target).d3dvector(), &up.d3dvector());
	D3DXMatrixTranslation(&m_worldMatrix, finalPos.x, finalPos.y, finalPos.z);
}

void SpringCamera::GetViewMatrix(D3DXMATRIX& viewMatrix){
	viewMatrix = m_viewMatrix;
}

void SpringCamera::GetGUIWorldMatrix(D3DXMATRIX& worldMatrix){
	worldMatrix = m_worldMatrix;
}

void SpringCamera::SetFieldOfView(float fov){
	m_fieldOfView = fov;
}

float SpringCamera::GetFieldOfView(){
	return m_fieldOfView;
}

Vector SpringCamera::GetFinalPosition(){
	Vector target = position + look;
	Vector finalPos = target - rotation* look;
	return finalPos;
}
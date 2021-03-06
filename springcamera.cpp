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
	look = Vector(0,0,4);  //camera always looks forward in z direction 
	rotation = Quaternion(Vector(0,1,0), 0);
	return true;
}

Vector SpringCamera::GetLookVector(){
	return look;
}

Vector SpringCamera::GetPosition(){
	return position;
}

void SpringCamera::Update(Vector targetPosition){
	this->targetPosition = targetPosition;
}
	
void SpringCamera::Reset(Vector position){
	this->position = position;
	targetPosition = Vector(0,0,0);
	velocity = Vector(0,0,0);
	rotation = Quaternion(Vector(0,1,0), 0);
}

void SpringCamera::SetRotation(Quaternion rotate){
	rotation = rotate;
}

Quaternion SpringCamera::GetRotation(){
	return rotation;
}

void SpringCamera::Render(float t){
	Vector desiredPosition = targetPosition + Vector(0, 0.5, -4);  //adding offset from target
	Vector up = Vector(0, 1, 0);
	//simulate spring deq
	Vector accel = vweight * velocity + dweight * (position - desiredPosition);
	velocity = velocity + accel * t / 1000.0f;
	position = position + velocity * t / 1000.0f;
	
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
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
	look = Vector(0,0,1);  //camera always looks forward in z direction 

	return true;
}
/*
bool SpringCamera::Initialize(char * datafile){
	position = Vector(0,0,0);
	shipPosition = Vector(0,0,0);
	velocity = Vector(0,0,0);
	distance = 0;
	//load data from file
	ifstream fin;
	char input;
	fin.open(datafile);
	if (fin.fail()){
		textDump("failed to open camera data file");
		return false;
	}
	for (fin.get(input); input != ':'; fin.get(input));
	float w;
	fin >> w;
	vweight = -2*w;
	dweight = -1*w*w;
	for (fin.get(input); input != ':'; fin.get(input));
	fin >> step;
	fin.close();
	return true;
}
*/
Vector SpringCamera::GetLookVector(){
	return look;
}

Vector SpringCamera::GetPosition(){
	return position;
}

void SpringCamera::Update(Vector targetPosition){
	this->targetPosition = targetPosition;
}
	
void SpringCamera::Reset(){
	position = Vector(-4,0,0);
	targetPosition = Vector(0,0,0);
	velocity = Vector(0,0,0);
}

void SpringCamera::Render(float t){
	Vector desiredPosition = targetPosition + Vector(0, 0, -4);  //adding offset from target
	Vector up = Vector(0, 1, 0);
	//simulate spring deq
	Vector accel = vweight * velocity + dweight * (position - desiredPosition);
	velocity = velocity + accel * t / 1000.0f;
	position = position + velocity * t / 1000.0f;
	/*
	while (t > 0) {
		Vector accel = vweight * velocity + dweight * (position - desiredPosition);
		if (t > step){
			t -= step;
			velocity = velocity + accel * step / 1000.0f;
			position = position + velocity * step / 1000.0f;
		}
		else {
			velocity = velocity + accel * t / 1000.0f;
			position = position + velocity * t / 1000.0f;
			t = 0;
		}
	}
	*/
	Vector target = position + look;
	//create matricies
	D3DXMatrixLookAtLH(&m_viewMatrix, &(position).d3dvector(), &(target).d3dvector(), &up.d3dvector());
	D3DXMatrixTranslation(&m_worldMatrix, position.x, position.y, position.z);
}

void SpringCamera::GetViewMatrix(D3DXMATRIX& viewMatrix){
	viewMatrix = m_viewMatrix;
}

void SpringCamera::GetGUIWorldMatrix(D3DXMATRIX& worldMatrix){
	worldMatrix = m_worldMatrix;
}
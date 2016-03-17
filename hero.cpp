#include "hero.h"

#define SPEED_MAX 60
#define ACCELERATION 600
#define MAX_ROLL 0.523
#define MAX_PITCH 0.523

Hero::Hero(void)
{
	heroModel = 0;
	
}


Hero::~Hero(void)
{
}


bool Hero::Initialize(Vector position, Vector softBoundary){
	heroModel = new Model();
	if (!heroModel){
		textDump("Could not create model in the hero class");
		return false;
	}
	if (!heroModel->Initialize(g_graphics->GetDevice(), "./Assets/shipmodel1.txt", L"./Assets/shiptex.dds", false)){
		textDump("Could not create model in hero class");
		return false;
	}
	this->position = position;
	this->velocity = Vector(0, 0, 0);
	heroModel->SetPosition(position);

	this->softBoundary = softBoundary;
	
	return true;
}
	
void Hero::Shutdown(){
	
	if (heroModel){
		heroModel->Shutdown();
		delete heroModel;
	}
}


bool Hero::Render(float t){
	g_graphics->RenderObject(heroModel, SHADER_LIGHT);
	return true;
}

void Hero::Update(float t, Input * input){
	//get key movement
	Vector keyYZacc = Vector(0,0,0);
	if (input->IsKeyDown(0x57) || input->IsKeyDown(0x41) || input->IsKeyDown(0x53) || input->IsKeyDown(0x44)){  
		//wasd for yz movement
		if (input->IsKeyDown(0x57)) keyYZacc = keyYZacc + Vector(0,1,0);
		if (input->IsKeyDown(0x41)) keyYZacc = keyYZacc + Vector(0,0,1);
		if (input->IsKeyDown(0x53)) keyYZacc = keyYZacc + Vector(0,-1,0);
		if (input->IsKeyDown(0x44)) keyYZacc = keyYZacc + Vector(0,0,-1);
	}
	//calculate acceleration
	Vector acceleration = keyYZacc;
	//if outside of softboundry set acceleration to zero if its moving away from boundry
	//decel will activate to change velocity if needed
	if (position.y > softBoundary.y && acceleration.y > 0){
		acceleration.y = 0;
	}
	if (position.y < -1 * softBoundary.y && acceleration.y < 0){
		acceleration.y = 0;
	}
	if (position.z > softBoundary.z && acceleration.z > 0){
		acceleration.z = 0;
	}
	if (position.z < -1 * softBoundary.z && acceleration.z < 0){
		acceleration.z = 0;
	}
	//set accel to go in opposite direction of movement if no key pressed
	if (acceleration.y == 0 && velocity.y > 0){
		acceleration.y = -1;
	}
	if (acceleration.y == 0 && velocity.y < 0){
		acceleration.y = 1;
	}
	if (acceleration.z == 0 && velocity.z > 0){
		acceleration.z = -1;
	}
	if (acceleration.z == 0 && velocity.z < 0){
		acceleration.z = 1;
	}
	


	//calculate velocity
	if (acceleration * acceleration != 0){ 
		acceleration = acceleration / acceleration.length();
		acceleration = ACCELERATION * acceleration * t / 1000.0;  //final accel vector
		
		if (acceleration.y * velocity.y < 0 && acceleration.y *acceleration.y > velocity.y * velocity.y){
			//if accel in opposite direction of velocity and larger create a stop
			velocity.y = 0;
			acceleration.y = 0;
		}
		if (acceleration.z * velocity.z < 0 && acceleration.z *acceleration.z > velocity.z * velocity.z){
			//if accel in opposite direction of velocity and larger create a stop
			velocity.z = 0;
			acceleration.z = 0;
		}

		velocity = velocity + acceleration;
		if (velocity * velocity > SPEED_MAX * SPEED_MAX) {
			velocity = velocity / velocity.length() * SPEED_MAX;
		}
	}
	
	
	//based on velocity calculate desired rotation for the ship
	Quaternion rotation = Quaternion(Vector(1,0,0), velocity.z / SPEED_MAX * MAX_ROLL) * Quaternion(Vector(0,0,1), velocity.y / SPEED_MAX * MAX_PITCH);
	//update position
	position = position + velocity * t / 1000.0;
	//update model
	heroModel->SetPosition(position);
	heroModel->SetRotation(rotation);
}

Vector Hero::GetPosition(){
	return position;
}

Vector Hero::GetVelocity(){
	return velocity;
}
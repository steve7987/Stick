#include "hero.h"

#define SPEED_MAX 60
#define ACCELERATION 200
#define MAX_ROLL 0.523
#define MAX_PITCH 0.523

Hero::Hero(void)
{
	heroModel = 0;
	
}


Hero::~Hero(void)
{
}


bool Hero::Initialize(Vector position){
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
	Vector keyYZacc = Vector(0,0,0);
	if (input->IsKeyDown(0x57) || input->IsKeyDown(0x41) || input->IsKeyDown(0x53) || input->IsKeyDown(0x44)){  
		//wasd for yz movement
		if (input->IsKeyDown(0x57)) keyYZacc = keyYZacc + Vector(0,1,0);
		if (input->IsKeyDown(0x41)) keyYZacc = keyYZacc + Vector(0,0,1);
		if (input->IsKeyDown(0x53)) keyYZacc = keyYZacc + Vector(0,-1,0);
		if (input->IsKeyDown(0x44)) keyYZacc = keyYZacc + Vector(0,0,-1);
	}
	//calculate velocity
	if (keyYZacc * keyYZacc != 0){  //accelerate based on keys
		keyYZacc = keyYZacc / keyYZacc.length();
		velocity = velocity + ACCELERATION * keyYZacc * t / 1000.0;
		if (velocity * velocity > SPEED_MAX * SPEED_MAX) {
			velocity = velocity / velocity.length() * SPEED_MAX;
		}
	}
	else if (velocity * velocity != 0){  //deccelerate
		Vector decel = ACCELERATION * velocity / velocity.length() * t / 1000.0;
		if (decel * decel > velocity * velocity){
			velocity = Vector(0,0,0);
		}
		else {
			velocity = velocity - decel;
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
#include "hero.h"


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
	Vector keyYZvel = Vector(0,0,0);
	if (input->IsKeyDown(0x57) || input->IsKeyDown(0x41) || input->IsKeyDown(0x53) || input->IsKeyDown(0x44)){  
		//wasd for yz movement
		if (input->IsKeyDown(0x57)) keyYZvel = keyYZvel + Vector(0,1,0);
		if (input->IsKeyDown(0x41)) keyYZvel = keyYZvel + Vector(0,0,1);
		if (input->IsKeyDown(0x53)) keyYZvel = keyYZvel + Vector(0,-1,0);
		if (input->IsKeyDown(0x44)) keyYZvel = keyYZvel + Vector(0,0,-1);
	}
	if (keyYZvel * keyYZvel != 0){
		keyYZvel = keyYZvel / keyYZvel.length();
	}
	//calculate velocity
	velocity = 60 * keyYZvel;
	
	//update position
	position = position + velocity * t / 1000.0;
	heroModel->SetPosition(position);
}

Vector Hero::GetPosition(){
	return position;
}
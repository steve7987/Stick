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
	if (!heroModel->Initialize(g_graphics->GetDevice(), "./Assets/sphere.txt", L"./Assets/border.dds", false)){
		textDump("Could not create model in hero class");
		return false;
	}
	this->position = position;
	heroModel->SetPosition(position);
	heroModel->SetScale(0.25, 0.25, 0.25);
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

void Hero::Update(float t, Vector velocityDirection){
	position = position + t*velocityDirection / 1000.0 * 6;  //6 is just temporary velocity
	heroModel->SetPosition(position);
}

Vector Hero::GetPosition(){
	return position;
}
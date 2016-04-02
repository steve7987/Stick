#include "projectile.h"

#define PROJECTILE_MODEL "./Assets/laser.txt"
#define PROJECTILE_TEXTURE L"./Assets/Outline.dds"
#define PROJECTILE_TIMER 4.0f
#define PROJECTILE_SPEED 180.0f

Projectile::Projectile(void)
{
	projectileModel = 0;
}


Projectile::~Projectile(void)
{
}


bool Projectile::Initialize(Vector position, Vector direction){
	projectileModel = new Model();
	if (!projectileModel){
		textDump("could not create model in projectile class");
		return false;
	}
	if (!projectileModel->Initialize(g_graphics->GetDevice(), PROJECTILE_MODEL, PROJECTILE_TEXTURE, false)){
		textDump("could not initialize model in projectile class");
		return false;
	}
	this->position = position;
	this->direction = direction;
	this->previousPosition = position;
	projectileModel->SetPosition(position);
	projectileModel->SetRotation(Quaternion(Vector(1,0,0), direction));
	
	timer = PROJECTILE_TIMER;

	return true;
}
	
void Projectile::Shutdown(){
	if (projectileModel){
		projectileModel->Shutdown();
		delete projectileModel;
		projectileModel = 0;
	}

}

bool Projectile::Render(float t){
	g_graphics->RenderObject(projectileModel, SHADER_LIGHT);
	return true;
}

bool Projectile::Update(float t){
	this->previousPosition = position;
	position = position + direction * PROJECTILE_SPEED * t / 1000.0f;
	projectileModel->SetPosition(position);
	timer -= t / 1000.0f;
	return timer > 0;
}

Vector Projectile::getPosition(){
	return position;
}

Vector Projectile::getPreviousPosition(){
	return previousPosition;
}

void Projectile::HitObject(){
	timer = 0;
}
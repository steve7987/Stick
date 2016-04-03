#include "enemy.h"

#define SHIP_MODEL "./Assets/enemyship.txt"
#define SHIP_TEXTURE L"./Assets/blocktex.dds"
#define SHIP_DIMENSIONS Vector(0.937, 1.087, 1.091);
#define SHIP_SPEED 50.0f


Enemy::Enemy(void){
	m_Model = 0;
}
	
Enemy::~Enemy(void){

}

	
bool Enemy::Initialize(Vector origin, Vector target, Vector exit, float time){
	//setup model
	m_Model = new Model();
	if (!m_Model){
		textDump("error creating model in enemy class");
		return false;
	}
	if (!m_Model->Initialize(g_graphics->GetDevice(), SHIP_MODEL, SHIP_TEXTURE, false)){
		textDump("error initializing model in enemy class");
		return false;
	}
	m_Model->SetPosition(position);
	m_Model->SetScale(3, 3, 3);

	//setup motion for enemy
	this->position = origin;
	this->target = target;
	this->exit = exit;
	this->timer = time;
	this->mode = 1;

	//setup hitbox	
	this->dimensions = 3*SHIP_DIMENSIONS;
	this->remove = false;	

	return true;
}

void Enemy::Shutdown(){
	if (m_Model){
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
}

	
bool Enemy::Render(float t){
	g_graphics->RenderObject(m_Model, SHADER_LIGHT);
	return true;
}
	
bool Enemy::Update(float t){
	if (mode == 1){  //move to target
		if(MoveToTarget(target, SHIP_SPEED, t)){
			mode++;
		}
	}
	else if (mode == 2){  //wait on the screen
		timer -= t / 1000.0f;
		if (timer < 0){
			mode++;
		}
	}
	else if (mode == 3){  //move to exit point
		if(MoveToTarget(exit, SHIP_SPEED, t)){
			mode++;
		}
	}
	//update model position
	m_Model->SetPosition(position);

	return !remove && mode != 4;  //remove if been hit or reached exit point
}
	
Vector Enemy::GetPosition(){
	return position;
}

Vector Enemy::GetDimensions(){
	return dimensions;
}

void Enemy::BeenHit(){
	remove = true;
}

bool Enemy::MoveToTarget(Vector destination, float speed, float t){
	Vector velocity = (destination - position).normalize() * speed * t / 1000.0f;
	if (velocity * velocity > (destination - position)*(destination - position)){
		position = destination;
		return true;
	}
	else {
		position = position + velocity;
		return false;
	}
}
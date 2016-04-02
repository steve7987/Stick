#include "enemy.h"

#define SHIP_MODEL "./Assets/enemyship.txt"
#define SHIP_TEXTURE L"./Assets/blocktex.dds"
#define SHIP_DIMENSIONS Vector(0.937, 1.087, 1.091);

Enemy::Enemy(void){
	m_Model = 0;
}
	
Enemy::~Enemy(void){

}

	
bool Enemy::Initialize(Vector position){
	m_Model = new Model();
	if (!m_Model){
		textDump("error creating model in enemy class");
		return false;
	}
	if (!m_Model->Initialize(g_graphics->GetDevice(), SHIP_MODEL, SHIP_TEXTURE, false)){
		textDump("error initializing model in enemy class");
		return false;
	}
	this->position = position;
	this->dimensions = SHIP_DIMENSIONS;
	m_Model->SetPosition(position);
	
	remove = false;
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
	return !remove;
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
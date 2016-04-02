#include "enemy.h"

#define SHIP_MODEL "./Assets/sphere.txt"
#define SHIP_TEXTURE L"./Assets/blocktex.dds"

Enemy::Enemy(void){
	m_Model = 0;
}
	
Enemy::~Enemy(void){

}

	
bool Enemy::Initialize(Vector position, Vector dimensions){
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
	this->dimensions = dimensions;
	m_Model->SetPosition(position);
	m_Model->SetScale(dimensions);
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
	
void Enemy::Update(float t){

}
	
Vector Enemy::GetPosition(){
	return position;
}
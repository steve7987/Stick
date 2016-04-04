#include "explosion.h"

#define EXPLOSION_MODEL "./Assets/sphere.txt"
#define EXPLOSION_TEXTURE L"./Assets/Outline.dds"

Explosion::Explosion(void){
	m_Model = 0;
}
	
Explosion::~Explosion(void){

}


bool Explosion::Initialize(Vector position){
	m_Model = new Model();
	if (!m_Model){
		textDump("could not create model in explosion class");
		return false;
	}
	if (!m_Model->Initialize(g_graphics->GetDevice(), EXPLOSION_MODEL, EXPLOSION_TEXTURE, false)){
		textDump("could not initialize model in explosion class");
		return false;
	}
	this->position = position;
	m_Model->SetPosition(position);

	return true;
}

void Explosion::Shutdown(){
	if (m_Model){
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
}


bool Explosion::Render(float t){
	g_graphics->RenderObject(m_Model, SHADER_LIGHT);
	return true;
}

bool Explosion::Update(float t){
	return true;
}
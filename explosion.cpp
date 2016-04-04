#include "explosion.h"

#define EXPLOSION_MODEL "./Assets/icosphere4.txt"
#define EXPLOSION_TEXTURE L"./Assets/tmpexplosion.dds"

#define EXPLOSION_LOOPTIME 0.75f

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

	timer = 0;

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
	float * x = new float[4];
	x[0] = sin(timer / EXPLOSION_LOOPTIME * 2*PI) * 0.5f + 0.25f;
	x[1] = sin(timer / EXPLOSION_LOOPTIME * 2*PI + 2*PI / 3) * 0.5f + 0.25f;
	x[2] = sin(timer / EXPLOSION_LOOPTIME * 2*PI + 4*PI / 3) * 0.5f + 0.25f;
	x[3] = 0.0f;

	g_graphics->RenderObject(m_Model, SHADER_EXPLOSION, x);
	return true;
}

bool Explosion::Update(float t){
	timer += t / 1000.0f;
	return true;
}
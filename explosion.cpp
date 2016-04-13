#include "explosion.h"

#define EXPLOSION_MODEL "./Assets/icosphere5.txt"
#define EXPLOSION_TEXTURE L"./Assets/explosionDistortion5.dds"
#define EXPLOSION_RAMPTEXTURE L"./Assets/tempExpRamp.dds"

#define EXPLOSION_LOOPTIME 0.85f


Explosion::Explosion(void){
	m_Model = 0;
}
	
Explosion::~Explosion(void){

}


bool Explosion::Initialize(Vector position, Vector scale, float duration){
	m_Model = new Model();
	if (!m_Model){
		textDump("could not create model in explosion class");
		return false;
	}
	if (!m_Model->Initialize(g_graphics->GetDevice(), EXPLOSION_MODEL, EXPLOSION_TEXTURE, false)){
		textDump("could not initialize model in explosion class");
		return false;
	}
	m_Model->SetSecondaryTexture(g_graphics->GetDevice(), EXPLOSION_RAMPTEXTURE);
	this->position = position;
	m_Model->SetPosition(position);

	timer = 0;

	//give the explosion a random orientation
	Vector axis = Vector(randb(-1.0f, 1.0f), randb(-1.0f, 1.0f), randb(-1.0f, 1.0f));
	m_Model->SetRotation(Quaternion(Vector(1,0,0), axis));

	this->duration = duration;
	this->scale = scale;

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
	x[3] = timer;

	float len = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
	x[0] /= len;
	x[1] /= len;
	x[2] /= len;

	g_graphics->RenderObject(m_Model, SHADER_EXPLOSION, x);
	return true;
}

bool Explosion::Update(float t){
	timer += t / (1000.0f * duration);
	float size = timer + 0.1f;
	m_Model->SetScale(size * scale);

	
	return timer < 1.0f;
}
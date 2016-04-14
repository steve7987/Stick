#include "environment.h"


Environment::Environment(void)
{
	m_Terrain = 0;
}


Environment::~Environment(void)
{
}


bool Environment::Initialize(){
	m_Terrain = new Terrain();
	if (!m_Terrain){
		textDump("error creating terrain");
		return false;
	}
	if (!m_Terrain->Initialize(g_graphics->GetDevice())){
		textDump("error initializing terrain");
		return false;
	}
	

	return true;
}
	
void Environment::Shutdown(){
	if (m_Terrain){
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

}

void Environment::render(float t){
	g_graphics->RenderObject(m_Terrain, SHADER_LIGHT);
}

void Environment::update(float t){

}
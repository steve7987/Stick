#include "environment.h"

#define TERRAIN_TEXTURE L"./assets/sand.dds"
#define SKYDOME_MODEL "./assets/sphere.txt"
#define SKYDOME_TEXTURE L"./assets/explosionDistortion5.dds"

Environment::Environment(void)
{
	m_Terrain = 0;
	m_SkyDome = 0;
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
	m_SkyDome = new Model();
	if (!m_SkyDome){
		textDump("Error creating skydome model");
		return false;
	}
	if (!m_SkyDome->Initialize(g_graphics->GetDevice(), SKYDOME_MODEL, SKYDOME_TEXTURE, false)){
		textDump("error initializing skydome model");
		return false;
	}


	m_block = new Block();
	if (!m_block){
		textDump("error creating environment block");
		return false;
	}
	if (!m_block->Initialize(Vector(-20, -12.1f, -1000), Vector(5000, 0.0f, 2000), TERRAIN_TEXTURE)){
		textDump("error initializing environment block");
		return false;
	}

	return true;
}
	
void Environment::Shutdown(){
	if (m_SkyDome){
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}
	if (m_block){
		m_block->Shutdown();
		delete m_block;
		m_block = 0;
	}
	
	if (m_Terrain){
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

}

void Environment::render(float t){
	g_graphics->RenderSkyDome(m_SkyDome, Vector(0.1f, 0.1f, 0.5f), Vector(0.1f, 0.6f, 1.0f));
	m_Terrain->Render(t);
	//m_block->Render(t);
}

void Environment::update(float t, Vector CameraPosition){
	m_SkyDome->SetPosition(CameraPosition);
	m_Terrain->Scroll(t / 10.0f);
}

float Environment::GetTerrainHeight(float x, float z){
	return m_Terrain->GetHeight(x, z);
}

Vector Environment::GetTerrainNormal(float x, float z){
	return m_Terrain->GetNormal(x, z);
}
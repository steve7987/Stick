#include "environment.h"


Environment::Environment(void)
{
	m_block = 0;
}


Environment::~Environment(void)
{
}


bool Environment::Initialize(){
	Vector position = Vector(-60,-8,-50);
	Vector dimensions = Vector(1000,1,100);
	m_block = new Block();
	m_block->Initialize(position, dimensions);
	
	return true;
}
	
void Environment::Shutdown(){
	if (m_block){
		m_block->Shutdown();
		delete m_block;
		m_block = 0;
	}

}

void Environment::render(float t){
	m_block->Render(t);
}

void Environment::update(float t){

}
#include "actionstate.h"

ActionState::ActionState(){
	m_Block = 0;
	m_Hero = 0;
	m_Camera = 0;
}

ActionState::~ActionState(){

}


bool ActionState::Initialize(){
	asSentence = g_graphics->RegisterSentence(64);
	g_graphics->ChangeSentence(asSentence, "Action State", 12, 38, 1.0f, 1.0f, 1.0f);
	g_graphics->SetVisibleSentence(asSentence, false);
	g_gui->setVisible(GUIWINDOW_ACTION, false);
	//setup camera
	m_Camera = new SpringCamera();
	if (!m_Camera){
		textDump("unable to create camera in action state");
		return false;
	}
	if (!m_Camera->Initialize(20, 0.5)){
		textDump("unable to initialize camera in action state");
		return false;
	}

	return true;
}

void ActionState::Shutdown(){
	if (m_Camera){
		delete m_Camera;
	}
}

	
bool ActionState::update(float t, Input * input){
	if (input->KeyBeenPushed(VK_ESCAPE)){
		g_gameStateManager->change("main menu");
	}
	Vector vel = Vector(0,0,0);
	if (input->IsKeyDown(0x57) || input->IsKeyDown(0x41) || input->IsKeyDown(0x53) || input->IsKeyDown(0x44)){  //wasd for horizontal movement
		
		if (input->IsKeyDown(0x57)) vel = vel + Vector(0,0,1);
		if (input->IsKeyDown(0x41)) vel = vel + Vector(-1,0,0);
		if (input->IsKeyDown(0x53)) vel = vel + Vector(0,0,-1);
		if (input->IsKeyDown(0x44)) vel = vel + Vector(1,0,0);
	}
	m_Hero->Update(t, vel);
	m_Camera->Update(m_Hero->GetPosition());

	return true;
}

void ActionState::render(float t){
	g_graphics->StartFrame(t, m_Camera);

	m_Block->Render(t);
	m_Hero->Render(t);

	g_graphics->EndFrame();
}

void ActionState::onExit(){
	g_gui->setVisible(GUIWINDOW_ACTION, false);
	g_graphics->SetVisibleSentence(asSentence, false);
	//shutdown blocks
	m_Block->Shutdown();
	delete m_Block;
	//shutdown hero
	if (m_Hero){
		m_Hero->Shutdown();
		delete m_Hero;
	}
}

void ActionState::onEnter(){
	g_gui->setVisible(GUIWINDOW_ACTION, true);
	g_graphics->SetVisibleSentence(asSentence, true);
	//create blocks
	m_Block = new Block();
	m_Block->Initialize(Vector(0,0,0), Vector(1,1,1));
	//create hero
	m_Hero = new Hero();
	if (!m_Hero){
		textDump("error creating hero in action state");
		return;
	}
	if (!m_Hero->Initialize(Vector(0,2,0))){
		textDump("error initializing hero in action state");
		return;
	}
}

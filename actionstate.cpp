#include "actionstate.h"

ActionState::ActionState(){
	m_Block = 0;
}

ActionState::~ActionState(){

}


bool ActionState::Initialize(){
	asSentence = g_graphics->RegisterSentence(64);
	g_graphics->ChangeSentence(asSentence, "Action State", 12, 38, 1.0f, 1.0f, 1.0f);
	g_graphics->SetVisibleSentence(asSentence, false);
	g_gui->setVisible(GUIWINDOW_ACTION, false);
	return true;
}

void ActionState::Shutdown(){

}

	
bool ActionState::update(float t, Input * input){
	if (input->KeyBeenPushed(VK_ESCAPE)){
		g_gameStateManager->change("main menu");
	}
	
	

	return true;
}

void ActionState::render(float t){
	g_graphics->StartFrame(t);

	m_Block->Render(t);

	g_graphics->EndFrame();
}

void ActionState::onExit(){
	g_gui->setVisible(GUIWINDOW_ACTION, false);
	g_graphics->SetVisibleSentence(asSentence, false);
	//shutdown blocks
	m_Block->Shutdown();
	delete m_Block;
}

void ActionState::onEnter(){
	g_gui->setVisible(GUIWINDOW_ACTION, true);
	g_graphics->SetVisibleSentence(asSentence, true);
	//create blocks
	m_Block = new Block();
	m_Block->Initialize(Vector(0,0,0), Vector(1,1,1));
}

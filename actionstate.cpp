#include "actionstate.h"

ActionState::ActionState(){
	m_Camera = 0;
}

ActionState::~ActionState(){

}


bool ActionState::Initialize(){
	asSentence = g_graphics->RegisterSentence(64);
	g_graphics->ChangeSentence(asSentence, "Action State", 12, 38, 1.0f, 1.0f, 1.0f);
	g_graphics->SetVisibleSentence(asSentence, false);
	
	debugSentence = g_graphics->RegisterSentence(256);
	g_graphics->ChangeSentence(debugSentence, "Block Vector: ", 12, 52, 1.0f, 1.0f, 1.0f);
	g_graphics->SetVisibleSentence(debugSentence, false);
	
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
		m_Camera = 0;
	}
}

	
bool ActionState::update(float t, Input * input){
	
	if (input->KeyBeenPushed(VK_ESCAPE)){
		g_gameStateManager->change("main menu");
	}
	if (input->IsKeyDown(0x45)){  //e pressed
		m_Camera->SetRotation(m_Camera->GetRotation() * Quaternion(Vector(0,1,0), t / 2000.0));
	}
	if (input->IsKeyDown(0x51)){  //q pressed
		m_Camera->SetRotation(m_Camera->GetRotation() * Quaternion(Vector(0,1,0), -1 * t / 2000.0));
	}
	

	//check for end of level
	if (LevelEndReached()){
		g_gameStateManager->change("level complete");
	}
	return true;
}

void ActionState::render(float t){
	g_graphics->StartFrame(t, m_Camera);

	g_graphics->EndFrame();
}

void ActionState::onExit(){
	g_gui->setVisible(GUIWINDOW_ACTION, false);
	g_graphics->SetVisibleSentence(asSentence, false);
	g_graphics->SetVisibleSentence(debugSentence, false);
}

void ActionState::onEnter(){
	g_gui->setVisible(GUIWINDOW_ACTION, true);
	g_graphics->SetVisibleSentence(asSentence, true);
	g_graphics->SetVisibleSentence(debugSentence, true);
	
	//reset camera
	m_Camera->Reset(Vector(0, 3, 0));
}

bool ActionState::LevelEndReached(){
	
	return false;
}
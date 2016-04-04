#include "teststate.h"

#define CAM_DISTANCE 18.0f

TestState::TestState(){
	m_Camera = 0;
	m_Block = 0;
	m_Explosion = 0;
}
	
TestState::~TestState(){

}

bool TestState::Initialize(){
	asSentence = g_graphics->RegisterSentence(64);
	g_graphics->ChangeSentence(asSentence, "Animation Test State", 12, 38, 1.0f, 1.0f, 1.0f);
	g_graphics->SetVisibleSentence(asSentence, false);
	
	debugSentence = g_graphics->RegisterSentence(256);
	g_graphics->ChangeSentence(debugSentence, "", 12, 52, 1.0f, 1.0f, 1.0f);
	g_graphics->SetVisibleSentence(debugSentence, false);
	
	g_gui->setVisible(GUIWINDOW_ANIMATIONTEST, false);
	//setup camera
	m_Camera = new SpringCamera();
	if (!m_Camera){
		textDump("unable to create camera in test state");
		return false;
	}
	if (!m_Camera->Initialize(8, 0.5)){
		textDump("unable to initialize camera in test state");
		return false;
	}
	//setup a block to look at
	Vector position = Vector(-60,-8,-50);
	Vector dimensions = Vector(1000,1,100);
	m_Block = new Block();
	m_Block->Initialize(position, dimensions);
	//create an explosion
	m_Explosion = new Explosion();
	m_Explosion->Initialize(Vector (10, 2, 0));


	return true;
}
	
void TestState::Shutdown(){
	if (m_Explosion){
		m_Explosion->Shutdown();
		delete m_Explosion;
		m_Explosion = 0;
	}
	if (m_Block){
		m_Block->Shutdown();
		delete m_Block;
		m_Block = 0;
	}
	if (m_Camera){
		delete m_Camera;
		m_Camera = 0;
	}
}



bool TestState::update(float t, Input * input){
	if (input->KeyBeenPushed(VK_ESCAPE)){
		g_gameStateManager->change("main menu");
	}
	int mx, my;
	input->GetMouseLocation(mx, my);
	g_gui->updateMouse(mx, my);
	if (input->MouseBeenPushed(MK_LBUTTON)){
		
		int window = g_gui->Frame(mx, my);		
		if (window == GUIWINDOW_ANIMATIONMAIN){
			g_gameStateManager->change("main menu");
		}
	}

	m_Explosion->Update(t);

	return true;
}

void TestState::render(float t){
	g_graphics->StartFrame(t, m_Camera);
	m_Block->Render(t);
	m_Explosion->Render(t);
	g_graphics->EndFrame();
}

void TestState::onExit(){
	g_gui->setVisible(GUIWINDOW_ANIMATIONTEST, false);
	g_graphics->SetVisibleSentence(asSentence, false);
	g_graphics->SetVisibleSentence(debugSentence, false);
}

void TestState::onEnter(){
	g_gui->setMouseVisible(true);
	g_gui->setVisible(GUIWINDOW_ANIMATIONTEST, true);
	g_graphics->SetVisibleSentence(asSentence, true);
	g_graphics->SetVisibleSentence(debugSentence, true);

	//reset camera
	m_Camera->Reset(Vector(-2 * CAM_DISTANCE, 0, 0));
	m_Camera->SetFieldOfView(PI / 4);
	m_Camera->Update(Vector(-CAM_DISTANCE, 0, 0));
}
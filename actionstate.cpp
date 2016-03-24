#include "actionstate.h"

#define CAM_DISTANCE 20.0f

ActionState::ActionState(){
	m_Camera = 0;
	m_Hero = 0;
	m_Environment = 0;
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
	if (!m_Camera->Initialize(8, 0.5)){
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
	if (m_Hero){
		m_Hero->Shutdown();
		delete m_Hero;
		m_Hero = 0;
	}
}

	
bool ActionState::update(float t, Input * input){
	std::ostringstream debugoss;
	debugoss << "Velocity: " << m_Hero->GetVelocity().y << ", " << m_Hero->GetVelocity().z;
	g_graphics->ChangeSentence(debugSentence, debugoss.str(), 12, 52, 1.0f, 1.0f, 1.0f);

	if (input->KeyBeenPushed(VK_ESCAPE)){
		g_gameStateManager->change("main menu");
	}
	/*
	if (input->IsKeyDown(0x45)){  //e pressed
		m_Camera->SetRotation(m_Camera->GetRotation() * Quaternion(Vector(0,1,0), t / 2000.0));
	}
	if (input->IsKeyDown(0x51)){  //q pressed
		m_Camera->SetRotation(m_Camera->GetRotation() * Quaternion(Vector(0,1,0), -1 * t / 2000.0));
	}
	*/

	m_Hero->Update(t, input);
	m_Environment->update(t);
	AdjustCamera();
	

	//check for end of level
	if (LevelEndReached()){
		g_gameStateManager->change("level complete");
	}
	return true;
}

void ActionState::render(float t){
	g_graphics->StartFrame(t, m_Camera);
	m_Hero->Render(t);
	m_Environment->render(t);
	g_graphics->EndFrame();
}

void ActionState::onExit(){
	g_gui->setVisible(GUIWINDOW_ACTION, false);
	g_graphics->SetVisibleSentence(asSentence, false);
	g_graphics->SetVisibleSentence(debugSentence, false);
	if (m_Environment){
		m_Environment->Shutdown();
		delete m_Environment;
		m_Environment = 0;
	}
	if (m_Hero){
		m_Hero->Shutdown();
		delete m_Hero;
		m_Hero = 0;
	}
}

void ActionState::onEnter(){
	g_gui->setVisible(GUIWINDOW_ACTION, true);
	g_graphics->SetVisibleSentence(asSentence, true);
	g_graphics->SetVisibleSentence(debugSentence, true);
	
	//reset camera
	m_Camera->Reset(Vector(-2 * CAM_DISTANCE, 0, 0));
	m_Camera->SetFieldOfView(PI / 4);
	m_Camera->Update(Vector(-CAM_DISTANCE, 0, 0));
	//compute soft boundary for hero based on camera
	float frustrumHeight = CAM_DISTANCE * tan(m_Camera->GetFieldOfView() * 0.5);
	float frustrumWidth = frustrumHeight * g_graphics->GetAspectRatio();
	softBoundary = Vector(0, frustrumHeight, frustrumWidth);

	//create hero
	m_Hero = new Hero();
	if (!m_Hero){
		textDump("unable to create hero in action state");
	}
	if (!m_Hero->Initialize(Vector(0,0,0), softBoundary)){
		textDump("unable to initialize hero in action state");
	}
	//create environment
	m_Environment = new Environment();
	if (!m_Environment){
		textDump("unable to create environment in action state");
	}
	if (!m_Environment->Initialize()){
		textDump("unable to initialize environment in action state");
	}
}

bool ActionState::LevelEndReached(){
	
	return false;
}

void ActionState::AdjustCamera(){
	Vector pos = m_Hero->GetPosition();
	pos = pos * 0.9;
	if (pos.y > softBoundary.y){
		pos.y = softBoundary.y;
	}
	if (pos.y < -softBoundary.y){
		pos.y = -softBoundary.y;
	}
	if (pos.z > softBoundary.z){
		pos.z = softBoundary.z;
	}
	if (pos.z < -softBoundary.z){
		pos.z = -softBoundary.z;
	}

	m_Camera->Update(pos + Vector(-CAM_DISTANCE, 0, 0));
	Quaternion xRot = m_Hero->GetRotation();  //compute only rotation around x-axis
	xRot.y = 0;
	xRot.z = 0;
	xRot.w += 3;  //decreases the angle of rotation
	xRot = xRot / sqrt(xRot.x * xRot.x + xRot.w * xRot.w);
	m_Camera->SetRotation(xRot);
}
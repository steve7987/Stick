#include "actionstate.h"

ActionState::ActionState(){
	m_Hero = 0;
	m_Camera = 0;
	blockDeque = 0;
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

	blockDeque = new deque<Block *>();

	return true;
}

void ActionState::Shutdown(){
	if (m_Camera){
		delete m_Camera;
		m_Camera = 0;
	}
	if (blockDeque){
		delete blockDeque;
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

	
	//render blocks
	for (std::deque<Block*>::iterator it = blockDeque->begin(); it != blockDeque->end(); ++it){
		(*it)->Render(t);
	}
	//render hero
	m_Hero->Render(t);

	g_graphics->EndFrame();
}

void ActionState::onExit(){
	g_gui->setVisible(GUIWINDOW_ACTION, false);
	g_graphics->SetVisibleSentence(asSentence, false);
	//shutdown blocks
	while (!blockDeque->empty()){
		Block * del = blockDeque->front();
		blockDeque->pop_front();
		del->Shutdown();
		delete del;
		del = 0;
	}
	//shutdown hero
	if (m_Hero){
		m_Hero->Shutdown();
		delete m_Hero;
		m_Hero = 0;
	}
}

void ActionState::onEnter(){
	g_gui->setVisible(GUIWINDOW_ACTION, true);
	g_graphics->SetVisibleSentence(asSentence, true);
	//create hero
	m_Hero = new Hero();
	if (!m_Hero){
		textDump("error creating hero in action state");
		return;
	}
	if (!m_Hero->Initialize(Vector(0,1.25,0))){
		textDump("error initializing hero in action state");
		return;
	}
	//create blocks
	unsigned int seed = time(NULL);
	std::stringstream oss;
	oss << "Seed for creating blocks is: " << seed;
	textDump(oss.str());
	CreateBlocks(seed);
}


void ActionState::CreateBlocks(unsigned int seed){
	srand(seed);
	for (int i = 0; i < 50; i++){
		Vector position = Vector(0,0,2*i);
		Vector size = Vector(1,1,1);
		Block * b = new Block();
		if (!b){
			textDump("Error creating block in action state");
			return;
		}
		if (!b->Initialize(position, size)){
			textDump("Error initializing block in action state");
			return;
		}


		blockDeque->push_back(b);
	}
}
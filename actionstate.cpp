#include "actionstate.h"

#define CAM_DISTANCE 20.0f

ActionState::ActionState(){
	m_Camera = 0;
	m_Hero = 0;
	m_Environment = 0;
	m_EnemyDeque = 0;
	
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
	if (m_Environment){
		m_Environment->Shutdown();
		delete m_Environment;
		m_Environment = 0;
	}
	if (m_EnemyDeque){
		while (!m_EnemyDeque->empty()){
			Enemy * del = m_EnemyDeque->front();
			m_EnemyDeque->pop_front();
			del->Shutdown();
			delete del;
			del = 0;
		}
		delete m_EnemyDeque;
		m_EnemyDeque = 0;
	}
	
}

	
bool ActionState::update(float t, Input * input){
	std::ostringstream debugoss;
	debugoss << "Velocity: " << m_Hero->GetVelocity().y << ", " << m_Hero->GetVelocity().z;
	g_graphics->ChangeSentence(debugSentence, debugoss.str(), 12, 52, 1.0f, 1.0f, 1.0f);

	if (input->KeyBeenPushed(VK_ESCAPE)){
		g_gameStateManager->change("main menu");
	}
	

	m_Hero->Update(t, input, m_Camera);
	m_Environment->update(t);
	AdjustCamera();
	//update enemies, check if they should be removed
	for (std::deque<Enemy*>::iterator it = m_EnemyDeque->begin(); it != m_EnemyDeque->end(); ){
		if (!(*it)->Update(t)){
			//need to remove
			(*it)->Shutdown();
			delete (*it);
			(*it) = 0;
			it = m_EnemyDeque->erase(it);
		}
		else {
			++it;  //dont increment in for loop since removing automatically iterates to the next element
		}
	}

	CheckCollisions();

	//check for end of level
	if (LevelEndReached()){
		g_gameStateManager->change("level complete");
	}
	return true;
}

void ActionState::render(float t){
	g_graphics->StartFrame(t, m_Camera);
	m_Hero->Render(t);
	for (std::deque<Enemy*>::iterator it = m_EnemyDeque->begin(); it != m_EnemyDeque->end(); ++it){
		(*it)->Render(t);
	}
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
	if (m_EnemyDeque){
		while (!m_EnemyDeque->empty()){
			Enemy * del = m_EnemyDeque->front();
			m_EnemyDeque->pop_front();
			del->Shutdown();
			delete del;
			del = 0;
		}
		delete m_EnemyDeque;
		m_EnemyDeque = 0;
	}
}

void ActionState::onEnter(){
	g_gui->setMouseVisible(false);
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
	softBoundary = 2*Vector(0, frustrumHeight, frustrumWidth);

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
	//create enemy deque
	m_EnemyDeque = new deque<Enemy *>();
	//add some random enemies
	for (int i = 0; i < 10; i++){
		Vector pos = Vector(50 + rand()%50, 5 + rand()%20, -30 + rand()%60);
		Enemy * add = new Enemy();
		add->Initialize(pos);
		m_EnemyDeque->push_front(add);
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
	//xRot.y = 0;
	xRot.z = 0;
	xRot.w += 3;  //decreases the angle of rotation
	xRot = xRot / sqrt(xRot.x * xRot.x + xRot.w * xRot.w + xRot.y * xRot.y + xRot.z * xRot.z);
	m_Camera->SetRotation(xRot);
}

void ActionState::CheckCollisions(){
	//check for collision between enemies and hero projectiles
	std::deque<Projectile*> * projDeque = m_Hero->GetProjectileDeque();

	for (std::deque<Enemy*>::iterator it = m_EnemyDeque->begin(); it != m_EnemyDeque->end(); ++it){
		for (std::deque<Projectile*>::iterator pit = projDeque->begin(); pit != projDeque->end(); ++pit){
			//see if the two intersected
			if (ellipsoidLineSegmentCollide((*it)->GetPosition(), (*it)->GetDimensions(), (*pit)->getPosition(), (*pit)->getPreviousPosition())){
				(*pit)->HitObject();
				(*it)->BeenHit();
			}
		}
	}

}
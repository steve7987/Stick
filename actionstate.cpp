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
	//shutdown hero
	if (m_Hero){
		m_Hero->Shutdown();
		delete m_Hero;
		m_Hero = 0;
	}
	//shutdown blocks
	while (!blockDeque->empty()){
		Block * del = blockDeque->front();
		blockDeque->pop_front();
		del->Shutdown();
		delete del;
		del = 0;
	}
}

void ActionState::onEnter(){
	g_gui->setVisible(GUIWINDOW_ACTION, true);
	g_graphics->SetVisibleSentence(asSentence, true);
	//create blocks
	unsigned int seed = time(NULL);
	std::stringstream oss;
	oss << "Seed for creating blocks is: " << seed;
	textDump(oss.str());
	CreateBlocks(seed, 4, 40, 0.57);
	//create hero
	m_Hero = new Hero();
	if (!m_Hero){
		textDump("error creating hero in action state");
		return;
	}
	//calc hero start based on where first blocks are
	Vector startpos = blockDeque->front()->getPosition() + blockDeque->front()->getDimensions() / 2;  //middle of block
	startpos = startpos + Vector(0, blockDeque->front()->getDimensions().y / 2, 0);
	if (!m_Hero->Initialize(startpos)){
		textDump("error initializing hero in action state");
		return;
	}
	//reset camera
	m_Camera->Reset(startpos + Vector(0, 3, 0));
}


void ActionState::CreateBlocks(unsigned int seed, int width, int length, float blockPercent){
	srand(seed);
	//first create an int array for pathing 0 is valid/open -1 is no block/closed
	int * bpath = new int[width*length];
	for (int i = 0; i < length; i++){
		for (int j = 0; j < width; j++){
			if ((float)(rand() % 1024) / 1024 < blockPercent){
				bpath[i*width + j] = 0;
			}
			else{
				bpath[i*width + j] = -1;
			}
		}
	}
	//add block at the beginning of the level

	//add other blocks
	for (int i = 0; i < length; i++){
		for (int j = 0; j < width; j++){
			if (bpath[i*width + j] == 0){
				int x = 1;
				int z = 1;
				bool canExpand = true;
				while (canExpand){
					if (rand() % 2 == 0 && j + x < width){ //check if block can expand in x dir
						if (ValidRect(bpath, i, j, x+1, z, width)){
							x++;
						}
						else {
							canExpand = false;
						}
					}
					else if (i + z < length)  { //check if it can expand in z dir
						if (ValidRect(bpath, i, j, x, z+1, width)){
							z++;
						}
						else {
							canExpand = false;
						}
					}
					else {
						canExpand = false;
					}
				}
				//close blocks that were included
				for (int a = i; a < i + z; a++){
					for (int b = j; b < j + x; b++){
						bpath[a*width + b] = -1;
					}
				}
				//add block with dimensions x, 1, z (adjusted slightly for edge gap)
				Vector position = Vector(j,0,i);
				Vector size = Vector(x - 0.1, 1, z - 0.1);  //0.1 is edge gap
					
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
	}


	//add block at end of the level



	delete [] bpath;
}

bool ActionState::ValidRect(int * bpath, int i, int j, int x, int z, int width){
	for (int a = i; a < i + z; a++){
		for (int b = j; b < j + x; b++){
			if (bpath[a*width + b] != 0){
				return false;
			}
		}
	}
	return true;
}
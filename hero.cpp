#include "hero.h"


Hero::Hero(void)
{
	heroModel = 0;
	anchorBlock = 0;
	heroLegModel = 0;
}


Hero::~Hero(void)
{
}


bool Hero::Initialize(Vector position){
	heroModel = new Model();
	if (!heroModel){
		textDump("Could not create model in the hero class");
		return false;
	}
	if (!heroModel->Initialize(g_graphics->GetDevice(), "./Assets/sphere.txt", L"./Assets/border.dds", false)){
		textDump("Could not create model in hero class");
		return false;
	}
	this->position = position;
	this->velocity = Vector(0, 0, 0);
	this->anchorVector = Vector(0 ,0, 0);
	heroModel->SetPosition(position);
	heroModel->SetScale(0.25, 0.25, 0.25);
	this->radius = 0.25;

	heroLegModel = new Model();
	if (!heroLegModel){
		textDump("Could not create model in the hero class");
		return false;
	}
	if (!heroLegModel->Initialize(g_graphics->GetDevice(), "./Assets/cube.txt", L"./Assets/border.dds", false)){
		textDump("Could not create model in hero class");
		return false;
	}
	legBlockDimensions = Vector(0.25, 0.5, 0.25);
	legBlockOffset = -1* legBlockDimensions + Vector(0.125, 0, 0.125);
	legBlockPosition = position + legBlockOffset;
	heroLegModel->SetPosition(legBlockPosition);
	heroLegModel->SetScale(legBlockDimensions);

	return true;
}
	
void Hero::Shutdown(){
	if (heroModel){
		heroModel->Shutdown();
		delete heroModel;
	}
	if (heroLegModel){
		heroLegModel->Shutdown();
		delete heroLegModel;
	}
}


bool Hero::Render(float t){
	g_graphics->RenderObject(heroModel, SHADER_LIGHT);
	g_graphics->RenderObject(heroLegModel, SHADER_LIGHT);
	return true;
}

void Hero::Update(float t, Input * input, deque<Block *> * blockDeque){
	Vector keyXZvel = Vector(0,0,0);
	if (input->IsKeyDown(0x57) || input->IsKeyDown(0x41) || input->IsKeyDown(0x53) || input->IsKeyDown(0x44)){  
		//wasd for xz movement
		if (input->IsKeyDown(0x57)) keyXZvel = keyXZvel + Vector(0,0,1);
		if (input->IsKeyDown(0x41)) keyXZvel = keyXZvel + Vector(-1,0,0);
		if (input->IsKeyDown(0x53)) keyXZvel = keyXZvel + Vector(0,0,-1);
		if (input->IsKeyDown(0x44)) keyXZvel = keyXZvel + Vector(1,0,0);
	}
	//calculate velocity
	velocity.x = 0;
	velocity.z = 0;
	velocity = velocity + keyXZvel * 6; //6 is just temporary velocity
	//check if gravity should be applied
	if (anchorBlock == 0){
		velocity.y = velocity.y + t / 1000.0 * -6; //-6 is just temp gravity constant
	}
	//check for jump
	if (anchorBlock != 0 && input->IsKeyDown(VK_SPACE)){  //could change to check for key press??
		velocity.y = 2;  //const is just temp jump power
	}
	//update position
	position = position + t * velocity / 1000.0;  
	heroModel->SetPosition(position);
	legBlockPosition = position + legBlockOffset;
	heroLegModel->SetPosition(legBlockPosition);
	//check and resolve collisions with blocks
	checkCollisions(blockDeque);
	//update anchorVector
	if (anchorBlock != 0){
		anchorVector = sphereBoxSidesCollide(position, radius, anchorBlock->getPosition(), anchorBlock->getDimensions());
	}
}

Vector Hero::GetPosition(){
	return position;
}

Vector Hero::GetAnchorVector(){
	return anchorVector;
}

Block * Hero::GetAnchorBlock(){
	return anchorBlock;
}

//check if we collided with any block, if not in the air so set anchor to zero
void Hero::checkCollisions(deque<Block *> * blockDeque){
	//check for detatchment from anchor block
	if (anchorBlock != 0 && !sphereBoxCollide(position, radius, anchorBlock->getPosition(), anchorBlock->getDimensions())
		&& !aabbCollide(legBlockPosition, legBlockDimensions, anchorBlock->getPosition(), anchorBlock->getDimensions())){
		anchorBlock = 0;
		anchorVector = Vector(0, 0, 0);
	}
	bool collided = false;
	for (std::deque<Block*>::iterator it = blockDeque->begin(); it != blockDeque->end(); ++it){
		if (aabbCollide(legBlockPosition, legBlockDimensions, (*it)->getPosition(), (*it)->getDimensions())){
			resolveBlockCollision((*it));
			collided = true;
		}
		
		if (sphereBoxCollide(position, radius, (*it)->getPosition(), (*it)->getDimensions())){
			resolveSphereCollision((*it));
			collided = true;
		}
	}
	if (!collided){
		anchorBlock = 0;
		anchorVector = Vector(0, 0, 0);
	}
}

//if theres no anchor, b becomes anchor and set velocity to zero
void Hero::resolveSphereCollision(Block * b){
	if (anchorBlock == 0){
		anchorBlock = b;
		anchorVector = sphereBoxSidesCollide(position, radius, b->getPosition(), b->getDimensions());
		velocity = Vector(0, 0, 0);
		return;
	}
	//check collision further with regards to other types of motion
}

void Hero::resolveBlockCollision(Block * b){
	if (anchorBlock == 0){
		anchorBlock = b;
		anchorVector = Vector(0, 0, 0);
		velocity = Vector(0, 0, 0);
		return;
	}
}
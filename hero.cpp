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
	Vector legBlockDisplayDimensions = Vector(0.25, 0.5, 0.25);
	legBlockOffset = -1* legBlockDisplayDimensions + Vector(0.125, 0, 0.125);
	legBlockPosition = position + legBlockOffset;
	heroLegModel->SetPosition(legBlockPosition);
	heroLegModel->SetScale(legBlockDisplayDimensions);
	legBlockDimensions = Vector(0.25, 0.25, 0.25);
	legAnchorBuffer = Vector(0.01, 0.01, 0.01);

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
	
	//check and resolve collisions with blocks
	proposedPosition = position + t * velocity / 1000.0;
	proposedVelocity = velocity;
	legBlockPosition = proposedPosition + legBlockOffset;
	checkCollisions(blockDeque, t);
	//update anchorVector
	if (anchorBlock != 0){
		anchorVector = sphereBoxSidesCollide(position, radius, anchorBlock->getPosition(), anchorBlock->getDimensions());
	}

	//update position
	velocity = proposedVelocity;
	position = proposedPosition;  
	heroModel->SetPosition(position);
	legBlockPosition = position + legBlockOffset;
	heroLegModel->SetPosition(legBlockPosition);
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
void Hero::checkCollisions(deque<Block *> * blockDeque, float t){
	bool collided = false;
	for (std::deque<Block*>::iterator it = blockDeque->begin(); it != blockDeque->end(); ++it){
		if (aabbCollide(legBlockPosition, legBlockDimensions, (*it)->getPosition(), (*it)->getDimensions())){
			resolveBlockCollision((*it), t);
			collided = true;
		}
		
		if (sphereBoxCollide(proposedPosition, radius, (*it)->getPosition(), (*it)->getDimensions())){
			resolveSphereCollision((*it), t);
			collided = true;
		}
	}
	//check for leg collision with anchor block (using leganchorbuffer)
	if (anchorBlock != 0 && !aabbCollide(legBlockPosition - legAnchorBuffer, 
		legBlockDimensions + legAnchorBuffer, anchorBlock->getPosition(), anchorBlock->getDimensions()))
	{
		anchorBlock = 0;
		anchorVector = Vector(0, 0, 0);
		collided = true;
	}
}

//if theres no anchor, b becomes anchor and set velocity to zero
void Hero::resolveSphereCollision(Block * b, float t){
	/*
	if (anchorBlock == 0){
		anchorBlock = b;
		anchorVector = sphereBoxSidesCollide(position, radius, b->getPosition(), b->getDimensions());
		velocity = Vector(0, 0, 0);
		return;
	}*/
	//check collision further with regards to other types of motion
}


void Hero::resolveBlockCollision(Block * b, float t){
	//compute which side hit the block, and revert that side to old position
	Vector offset = proposedPosition - position;
	if (velocity.x > 0 && legBlockPosition.x + legBlockDimensions.x > b->getPosition().x 
		&& legBlockPosition.x + legBlockDimensions.x - offset.x <= b->getPosition().x)
	{
		proposedPosition.x = position.x;
		proposedVelocity.x = 0;

	}
	if (velocity.x < 0 && legBlockPosition.x < b->getPosition().x + b->getDimensions().x 
		&& legBlockPosition.x - offset.x >= b->getPosition().x + b->getDimensions().x)
	{
		proposedPosition.x = position.x;
		proposedVelocity.x = 0;
	}

	if (velocity.y > 0 && legBlockPosition.y + legBlockDimensions.y > b->getPosition().y 
		&& legBlockPosition.y + legBlockDimensions.y - offset.y <= b->getPosition().y)
	{
		proposedPosition.y = position.y;
		proposedVelocity.y = 0;

	}
	if (velocity.y < 0 && legBlockPosition.y < b->getPosition().y + b->getDimensions().y 
		&& legBlockPosition.y - offset.y >= b->getPosition().y + b->getDimensions().y)
	{
		proposedPosition.y = position.y;
		anchorBlock = b;  //landed on top of a block
		proposedVelocity.y = 0;
	}

	if (velocity.z > 0 && legBlockPosition.z + legBlockDimensions.z > b->getPosition().z 
		&& legBlockPosition.z + legBlockDimensions.z - offset.z <= b->getPosition().z)
	{
		proposedPosition.z = position.z;
		proposedVelocity.z = 0;

	}
	if (velocity.z < 0 && legBlockPosition.z < b->getPosition().z + b->getDimensions().z 
		&& legBlockPosition.z - offset.z >= b->getPosition().z + b->getDimensions().z)
	{
		proposedPosition.z = position.z;
		proposedVelocity.z = 0;
	}
}
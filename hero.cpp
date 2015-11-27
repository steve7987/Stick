#include "hero.h"


Hero::Hero(void)
{
	heroModel = 0;
	anchorBlock = 0;
	handAnchorBlock = 0;
	heroLegModel = 0;
	for (int i = 0; i < HERO_NUMFEET; i++){
		footModel[i] = 0;
	}
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
	if (!heroModel->Initialize(g_graphics->GetDevice(), "./Assets/sphere.txt", L"./Assets/Outline.dds", true)){
		textDump("Could not create model in hero class");
		return false;
	}
	this->position = position;
	this->velocity = Vector(0, 0, 0);
	this->handAnchorVector = Vector(0 ,0, 0);
	heroModel->SetPosition(position);
	heroModel->SetScale(0.25, 0.25, 0.25);
	this->radius = 0.25;

	heroLegModel = new Model();
	if (!heroLegModel){
		textDump("Could not create model in the hero class");
		return false;
	}
	if (!heroLegModel->Initialize(g_graphics->GetDevice(), "./Assets/cube.txt", L"./Assets/Outline.dds", true)){
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

	//setup stick figure animation
	for (int i = 0; i < HERO_NUMFEET; i++){
		footModel[i] = new Model();
		if (!footModel[i]){
			textDump("Could not create model in the hero class");
			return false;
		}
		if (!footModel[i]->Initialize(g_graphics->GetDevice(), "./Assets/sphere.txt", L"./Assets/Black.dds", false)){
			textDump("Could not initialize model in the hero class");
			return false;
		}
		footRadius = 0.03;
		footDefault[i] = -1 * Vector(0, legBlockDisplayDimensions.y, 0) + Vector(0, footRadius, 0) 
						  + (2*i - 1) * Vector(legBlockDisplayDimensions.x / 4 - footRadius, 0, 0);
		footPosition[i] = footDefault[i];
		footModel[i]->SetPosition(position + footPosition[i]);
		footModel[i]->SetScale(Vector(footRadius, footRadius, footRadius));
		
	}
	footVelocity = Vector(0,0,0);
	mainFootForward = true;

	return true;
}
	
void Hero::Shutdown(){
	for (int i = 0; i < HERO_NUMFEET; i++){
		if (footModel[i]){
			footModel[i]->Shutdown();
			delete footModel[i];
			footModel[i] = 0;
		}
	}
	
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
	for (int i = 0; i < HERO_NUMFEET; i++){
		g_graphics->RenderObject(footModel[i], SHADER_LIGHT);
	}
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
	velocity = velocity + keyXZvel * 1.5; //num is just temporary velocity
	//check if gravity should be applied
	if (anchorBlock == 0 && handAnchorVector.y != 1){
		velocity.y = velocity.y + t / 1000.0 * -6; //-6 is just temp gravity constant
	}
	//check for jump
	if ((handAnchorVector.y == 1 || anchorBlock != 0) && input->IsKeyDown(VK_SPACE)){  //could change to check for key press??
		velocity.y = 2;  //const is just temp jump power
	}
	
	//check and resolve collisions with blocks
	proposedPosition = position + t * velocity / 1000.0;
	proposedVelocity = velocity;
	legBlockPosition = proposedPosition + legBlockOffset;
	checkCollisions(blockDeque, t);
	
	//update position
	velocity = proposedVelocity;
	position = proposedPosition;  
	heroModel->SetPosition(position);
	legBlockPosition = position + legBlockOffset;
	heroLegModel->SetPosition(legBlockPosition);
	
	//update stick animations
	updateAnimations(t);
}

Vector Hero::GetPosition(){
	return position;
}

Vector Hero::GetHandAnchorVector(){
	return handAnchorVector;
}

Block * Hero::GetAnchorBlock(){
	return anchorBlock;
}

Block * Hero::GetHandAnchorBlock(){
	return handAnchorBlock;
}

//check if we collided with any block, if not in the air so set anchor to zero
void Hero::checkCollisions(deque<Block *> * blockDeque, float t){
	//check for leg collision with anchor block (using leganchorbuffer)
	if (anchorBlock != 0 && !aabbCollide(legBlockPosition - legAnchorBuffer, 
		legBlockDimensions + legAnchorBuffer, anchorBlock->getPosition(), anchorBlock->getDimensions()))
	{
		anchorBlock = 0;
	}
	//check for arm collision with anchor block
	if (handAnchorBlock != 0 && !sphereBoxCollide(proposedPosition, radius, handAnchorBlock->getPosition(), 
												  handAnchorBlock->getDimensions()))
	{
		handAnchorBlock = 0;
		handAnchorVector = Vector(0, 0, 0);
	}
	bool legCollided = false;
	bool armCollided = false;
	for (std::deque<Block*>::iterator it = blockDeque->begin(); it != blockDeque->end(); ++it){
		if (aabbCollide(legBlockPosition, legBlockDimensions, (*it)->getPosition(), (*it)->getDimensions())){
			resolveBlockCollision((*it), t);
			legCollided = true;
		}
		
		if (sphereBoxCollide(proposedPosition, radius, (*it)->getPosition(), (*it)->getDimensions())){
			resolveSphereCollision((*it), t);
			armCollided = true;
		}
	}
	
	if (armCollided == false){
		handAnchorBlock = 0;
		handAnchorVector = Vector(0, 0, 0);
	}
}


void Hero::resolveSphereCollision(Block * b, float t){
	//if no hand block make it this one
	if (handAnchorBlock == 0){
		handAnchorBlock = b;
		handAnchorVector = sphereBoxSidesCollide(position, radius, b->getPosition(), b->getDimensions());
	}
	//if top of this box can be reached, make it the anchor
	if (sphereBoxSidesCollide(proposedPosition, radius, b->getPosition(), b->getDimensions()).y == 1){
		handAnchorBlock = b;
		handAnchorVector = sphereBoxSidesCollide(proposedPosition, radius, b->getPosition(), b->getDimensions());
	}
	//if top of this box was held last frame but it isnt any more, move back to old spot and stop y velocity if its negative
	else if (sphereBoxSidesCollide(position, radius, b->getPosition(), b->getDimensions()).y == 1){
		if (velocity.y < 0){
			proposedVelocity.y = 0;
			proposedPosition.y = position.y;
		}
	}
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


void Hero::updateAnimations(float t){
	if (anchorBlock != 0 && velocity * velocity != 0){
		//if moving on the ground
		Vector targetPos = getDesiredStepPoint();
		Vector footVel = targetPos - footPosition[0];
		if (footVel * velocity > 0){
			mainFootForward = true;
		}
		else {
			mainFootForward = false;
		}
		footVel = footVel / footVel.length() * velocity.length();
		footPosition[0] = footPosition[0] + footVel * t / 1000.0;
	}

	//compute position of mirrored foot (foot 1)
	footPosition[1] = footDefault[1] - (footPosition[0] - footDefault[0]);
	//update feet model positions
	for (int i = 0; i < HERO_NUMFEET; i++){
		footModel[i]->SetPosition(position + footPosition[i]);
	}
}

Vector Hero::getDesiredStepPoint(){
	Vector dir;
	if (mainFootForward){
		dir = velocity;
	}
	else {
		dir = -1 * velocity;
	}
	float tmin = FLT_MAX;
	//compute time to hit four edges of the foot box (leg half of leg box)
	float txmin = (legBlockOffset.x - footDefault[0].x + footRadius) / dir.x;
	float txmax = (0 - footDefault[0].x - footRadius) / dir.x;
	float tzmin = (legBlockOffset.z + footRadius) / dir.z;
	float tzmax = (legBlockOffset.z + legBlockDimensions.z - footRadius) / dir.z;
	//find smallest positive time
	if (dir.x != 0 && txmin > 0 && txmin < tmin){
		tmin = txmin;
	}
	if (dir.x != 0 && txmax > 0 && txmax < tmin){
		tmin = txmax;
	}
	if (dir.z != 0 && tzmin > 0 && tzmin < tmin){
		tmin = tzmin;
	}
	if (dir.z != 0 && tzmax > 0 && tzmax < tmin){
		tmin = tzmax;
	}
	//if there is only yvel, move back to default point;
	if (dir.x == 0 && dir.z == 0){
		tmin = 0;
	}
	return footDefault[0] + tmin * dir;
}
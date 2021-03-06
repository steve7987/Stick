#include "hero.h"

#define GRAVITY_CONST -6
#define SPEED_CONST 0.25


Hero::Hero(void)
{
	heroModel = 0;
	anchorBlocks = 0;
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
	anchorBlocks = new deque<Block *>();
	
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
		stepHeight[i] = 0;
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
	if (anchorBlocks){
		delete anchorBlocks;
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
	velocity = velocity + keyXZvel * SPEED_CONST;
	//check if gravity should be applied
	if (anchorBlocks->size() == 0 && handAnchorVector.y != 1){
		velocity.y = velocity.y + t / 1000.0 * GRAVITY_CONST;
	}
	//check for jump
	if ((handAnchorVector.y == 1 || anchorBlocks->size() != 0) && input->IsKeyDown(VK_SPACE)){  //could change to check for key press??
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

deque<Block *> * Hero::GetAnchorBlocks(){
	return anchorBlocks;
}

Block * Hero::GetHandAnchorBlock(){
	return handAnchorBlock;
}

//check if we collided with any block, recreate the deque of anchor blocks
void Hero::checkCollisions(deque<Block *> * blockDeque, float t){
	//clear out anchorblocks from last frame
	anchorBlocks->clear();
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
		//check if hero is standing on this block, and add it to anchorblock deque if it is
		if (aabbCollide(legBlockPosition - legAnchorBuffer, legBlockDimensions + legAnchorBuffer, 
			(*it)->getPosition(), (*it)->getDimensions()) && legBlockPosition.y > (*it)->getPosition().y + (*it)->getDimensions().y)
		{
			//textDump("added anchor block in checkCollisions");
			anchorBlocks->push_front((*it));
		}
		//check for contact collisions
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
		if (anchorBlocks->size() == 0 || b != anchorBlocks->front()){
			anchorBlocks->push_front(b);  //landed on top of a block and not already on deque
			//textDump("added anchor block in resolveBlockCollision");
		}
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

	//if moving on the ground
	if (anchorBlocks->size() != 0){
		//get target points for feet
		Vector targetPos [2];
		targetPos[0] = footPosition[0];  
		targetPos[1] = footPosition[1];
		if (velocity * velocity != 0){  //if moving get desired point to step to
			targetPos[0] = getDesiredStepPoint();  
			targetPos[1] = footDefault[1] + footDefault[0] - targetPos[0];
		}
		else {  //if not moving reset forward foot (then it will switch and other foot will be reset too)
			if (mainFootForward){
				targetPos[0] = footDefault[0];
			}
			else {
				targetPos[1] = footDefault[1];
			}
		}
		//adjust target positions for the feet, and move them accordingly
		for (int i = 0; i < HERO_NUMFEET; i++){
			//find closest spot to targetpos that is on an anchor block
			Vector newTarget = closestPointOnBlock(targetPos[i] + position, footRadius, anchorBlocks->front()->getPosition(), 
												   anchorBlocks->front()->getDimensions(), Vector(0, 1, 0)) 
												   - position + Vector(0, footRadius, 0);
			float distSQ = (targetPos[i] - newTarget) * (targetPos[i] - newTarget);
			for (std::deque<Block*>::iterator it = anchorBlocks->begin(); it != anchorBlocks->end(); ++it){
				Vector tmp = closestPointOnBlock(targetPos[i] + position, footRadius, (*it)->getPosition(), 
												 (*it)->getDimensions(), Vector(0, 1, 0)) 
												 - position + Vector(0, footRadius, 0);
				if (distSQ > (targetPos[i] - tmp)*(targetPos[i] - tmp)){
					distSQ = (targetPos[i] - tmp)*(targetPos[i] - tmp);
					newTarget = tmp;
				}
			}
			targetPos[i] = newTarget;
			//move foot
			Vector footVel = targetPos[i] - footPosition[i];
			if (footVel * footVel != 0){  //if need to move
				footVel = footVel / footVel.length() * SPEED_CONST;
				if ((targetPos[i] - footPosition[i]) * (targetPos[i] - footPosition[i]) <= (footVel * t / 1000.0) * (footVel * t / 1000.0)){
					footPosition[i] = targetPos[i];
					//if target has been reached, swap foot moving forward (only if its the main foot moving)
					if (i == 0){
						mainFootForward = !mainFootForward;
					}
				}
				else {
					footPosition[i] = footPosition[i] + footVel * t / 1000.0;
				}
			}
			//calculate step height
			float desiredHeight = 0;
			if (mainFootForward ^ (i % 2 == 1)){  //if this is main foot and moving forward, or off foot and not forward
				desiredHeight = (targetPos[i] - footPosition[i]).length() / 2;
			}
			//adjust stepHeight
			if (desiredHeight > stepHeight[i]){
				if (desiredHeight - stepHeight[i] > SPEED_CONST * t / 1000.0){
					stepHeight[i] += SPEED_CONST * t / 1000.0;
				}
				else {
					stepHeight[i] = desiredHeight;
				}
			}
			else if (desiredHeight < stepHeight[i]){
				if (stepHeight[i]  - desiredHeight > SPEED_CONST * t / 1000.0){
					stepHeight[i] -= SPEED_CONST * t / 1000.0;
				}
				else {
					stepHeight[i] = desiredHeight;
				}
			}
		}
	}
	

	//update feet model positions
	for (int i = 0; i < HERO_NUMFEET; i++){
		footModel[i]->SetPosition(position + footPosition[i] + Vector(0, stepHeight[i], 0));
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
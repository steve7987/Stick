#include "hero.h"


Hero::Hero(void)
{
	heroModel = 0;
	anchorBlock = 0;
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
	heroModel->SetPosition(position);
	heroModel->SetScale(0.25, 0.25, 0.25);
	this->radius = 0.25;
	return true;
}
	
void Hero::Shutdown(){
	if (heroModel){
		heroModel->Shutdown();
		delete heroModel;
	}
}


bool Hero::Render(float t){
	g_graphics->RenderObject(heroModel, SHADER_LIGHT);
	return true;
}

void Hero::Update(float t, Vector velocityDirection, deque<Block *> * blockDeque){
	//calculate velocity
	velocity.x = 0;
	velocity.z = 0;
	velocity = velocity + velocityDirection * 6; //6 is just temporary velocity
	//check if gravity should be applied
	if (anchorBlock == 0){
		velocity.y = velocity.y + t / 1000.0 * -6;
	}
	//update position
	position = position + t * velocity / 1000.0;  
	heroModel->SetPosition(position);
	//check and resolve collisions with blocks
	checkCollisions(blockDeque);
}

Vector Hero::GetPosition(){
	return position;
}

//check if we collided with any block, if not in the air so set anchor to zero
void Hero::checkCollisions(deque<Block *> * blockDeque){
	bool collided = false;
	for (std::deque<Block*>::iterator it = blockDeque->begin(); it != blockDeque->end(); ++it){
		if (sphereBoxCollide(position, radius, (*it)->getPosition(), (*it)->getDimensions())){
			resolveCollision((*it));
			collided = true;
		}
	}
	if (!collided){
		anchorBlock = 0;
	}
}

//if theres no anchor, b becomes anchor and set velocity to zero
void Hero::resolveCollision(Block * b){
	if (anchorBlock == 0){
		anchorBlock = b;
		velocity = Vector(0, 0, 0);
		return;
	}
	//check collision further with regards to other types of motion

}
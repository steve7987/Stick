#ifndef _HERO_H_
#define _HERO_H_

#include <deque>

#include "graphics.h"
#include "input.h"
#include "model.h"
#include "vector.h"
#include "helpers.h"
#include "block.h"

extern Graphics * g_graphics;

#define HERO_NUMFEET 2

class Hero
{
public:
	Hero(void);
	~Hero(void);

	bool Initialize(Vector position);
	void Shutdown();

	bool Render(float t);
	//velocity should come from the keyboard
	//blockDeque is a deque of blocks that can be collided with
	void Update(float t, Input * input, deque<Block *> * blockDeque );   

	Vector GetPosition();
	Vector GetHandAnchorVector();
	Block * GetAnchorBlock();
	Block * GetHandAnchorBlock();

private:
	//checks for collisions with any block in the list, if a collision occurs, position and velocity are adjusted
	void checkCollisions(deque<Block *> * blockDeque, float t);
	//knowing that there has been a collision with b, resolve the collision by changing position and velocity
	void resolveSphereCollision(Block * b, float t);
	//resolve a collision involving the leg block
	void resolveBlockCollision(Block * b, float t);
	//updates the animations of the stick figure
	void updateAnimations(float t);
	//returns where main foot should be stepping based on hero velocity
	Vector getDesiredStepPoint();

	//model for the sphere
	Model * heroModel;
	float radius;
	//info for the block representing the legs
	Model * heroLegModel;
	Vector legBlockDimensions;  //dimensions of the block
	Vector legBlockPosition;
	Vector legBlockOffset;  //offset vector from positon to legBlockPosition
	Vector legAnchorBuffer;  //buffer for the leg block to keep the anchor

	//vectors for what the position and velocity should be before collision detection
	Vector proposedPosition;
	Vector proposedVelocity;

	Vector position;
	Vector velocity;  //velocity is in units per second
	

	//the block that the hero is standing on to, 0 if in the air
	Block * anchorBlock;
	//contains the faces of the block that the hero is anchored to (e.g. <0,1,-1> means far y and near z planes)
	Block * handAnchorBlock;
	Vector handAnchorVector; 

	//STUFF FOR STICK FIGURE ANIMATION
	float footRadius;
	Model * footModel[HERO_NUMFEET];
	

	Vector footDefault[HERO_NUMFEET];  //relative to vector position
	Vector footPosition[HERO_NUMFEET];  //relative to Vector position
	Vector footVelocity; //vel of the main foot, other is just mirrored
	bool mainFootForward;  //if the main foot is moving with or against the characters velocity
};

#endif
#ifndef _HERO_H_
#define _HERO_H_

#include <deque>

#include "graphics.h"
#include "input.h"
#include "model.h"
#include "vector.h"
#include "helpers.h"
#include "billboard.h"
#include "projectile.h"

extern Graphics * g_graphics;

class Hero
{
public:
	Hero(void);
	~Hero(void);

	bool Initialize(Vector position, Vector softBoundary);
	void Shutdown();

	bool Render(float t);
	//velocity should come from the keyboard
	//blockDeque is a deque of blocks that can be collided with
	void Update(float t, Input * input, BaseCamera * activeCam);   

	Vector GetPosition();
	Vector GetVelocity();
	Quaternion GetRotation();

private:
	void HandleMovement(float t, Input * input);
	void AdjustTargeting(Input * input, BaseCamera * activeCam);
	void HandleShooting(float t, Input * input);

	//model for the hero
	Model * heroModel;
	Model *sbModel;  //transparent cube to represent the soft boundary for the ship
	
	Vector position;
	Vector velocity;  //velocity is in units per second
	Quaternion rotation;

	Vector softBoundary;  //outside this boundry (box from -sb to sb) ship wont accelerate

	Billboard * m_NearTarget;  //the near targeter goes from mouse to plane parallel to camera
	Billboard * m_FarTarget;  //far targeter goes on ray from ship to near targeter

	Vector nearTargetPos;

	std::deque<Projectile*> * projDeque;  //deque of projectiles that have been fired by the ship
};

#endif
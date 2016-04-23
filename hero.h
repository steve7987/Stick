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
#include "enemy.h"
#include "environment.h"

extern Graphics * g_graphics;

class Hero
{
public:
	Hero(void);
	~Hero(void);

	bool Initialize(Vector position, Vector softBoundary);
	void Shutdown();

	bool Render(float t);

	void Update(float t, Input * input, BaseCamera * activeCam, std::deque<Enemy *> * enemyDeque, Environment * environment);   

	Vector GetPosition();
	Vector GetVelocity();
	Quaternion GetRotation();

	std::deque<Projectile*> * GetProjectileDeque();

private:
	void HandleMovement(float t, Input * input, Environment * environment);
	void AdjustTargeting(Input * input, BaseCamera * activeCam, std::deque<Enemy *> * enemyDeque);
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

	Vector nearTargetPos;  //position of the near targeter
	float nearTargetDistance;  //x-coord of the near targeter

	std::deque<Projectile*> * projDeque;  //deque of projectiles that have been fired by the ship

	float fireTimer;  //time remaining until can shoot again
};

#endif
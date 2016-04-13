#ifndef _ENEMY_H
#define _ENEMY_H

#include <deque>

#include "graphics.h"
#include "model.h"
#include "vector.h"
#include "helpers.h"
#include "explosion.h"

extern Graphics * g_graphics;

class Enemy
{
public:
	Enemy(void);
	~Enemy(void);

	bool Initialize(Vector origin, Vector target, Vector exit, float time);
	void Shutdown();

	bool Render(float t);
	bool Update(float t);   //returns false if the enemy should be removed from the deque

	Vector GetPosition();
	Vector GetDimensions();

	void BeenHit();  //called when the ship has been hit

private:
	bool MoveToTarget(Vector destination, float speed, float t);  //moves the enemy to the destination based on speed and time, returns true if target reached

	Vector position;
	Vector dimensions;

	//basic AI vars
	Vector target;  //where the enemy will move to on the screen
	Vector exit;  //where the enemy will exit to off the screen
	float timer;  //timer until exit begins
	int mode;  //current action.  1=move to target, 2=at target, 3=move to exit, 4=remove

	Model * m_Model;  //model for the main part of the enemy ship

	bool remove;

};

#endif
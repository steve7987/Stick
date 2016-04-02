#ifndef _ENEMY_H
#define _ENEMY_H

#include <deque>

#include "graphics.h"
#include "model.h"
#include "vector.h"
#include "helpers.h"

extern Graphics * g_graphics;

class Enemy
{
public:
	Enemy(void);
	~Enemy(void);

	bool Initialize(Vector position);
	void Shutdown();

	bool Render(float t);
	bool Update(float t);   //returns false if the enemy should be removed from the deque

	Vector GetPosition();
	Vector GetDimensions();

	void BeenHit();  //called when the ship has been hit

private:
	Vector position;
	Vector dimensions;

	Model * m_Model;  //model for the main part of the enemy ship

	bool remove;

};

#endif
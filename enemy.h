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

	bool Initialize(Vector position, Vector dimensions);
	void Shutdown();

	bool Render(float t);
	void Update(float t);   

	Vector GetPosition();

private:
	Vector position;
	Vector dimensions;

	Model * m_Model;  //model for the main part of the enemy ship

};

#endif
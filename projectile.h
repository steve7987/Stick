#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "graphics.h"
#include "model.h"
#include "vector.h"

extern Graphics * g_graphics;


class Projectile{
public:
	Projectile(void);
	~Projectile(void);

	bool Initialize(Vector position, Vector direction);
	void Shutdown();

	bool Render(float t);
	bool Update(float t);  //returns false if the projectile should be removed

	Vector getPosition();

private:
	Model * projectileModel;

	Vector position;  
	Vector direction;

};

#endif
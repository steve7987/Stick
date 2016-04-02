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
	Vector getPreviousPosition();

	void HitObject();  //called when the projectile hits an object

private:
	Model * projectileModel;

	Vector position;  //current position of proj tip
	Vector direction;

	Vector previousPosition;  //position of the tip during the previous frame (same as pos during first frame)

	float timer;
};

#endif
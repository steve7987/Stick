#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "classlist.h"
#include "graphics.h"
#include "model.h"
#include "vector.h"


//game files
#include "block.h"

extern Graphics * g_graphics;

class Environment{
public:
	Environment();
	~Environment();

	bool Initialize();
	void Shutdown(); 
	
	void update(float t);
	void render(float t);

private:
	Block * m_block;

};

#endif
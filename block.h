#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "graphics.h"
#include "model.h"
#include "vector.h"

extern Graphics * g_graphics;

#define BLOCK_MODEL "./Assets/cube.txt"
#define BLOCK_TEXTURE L"./Assets/blocktex.dds"

class Block{
public:
	Block(void);
	~Block(void);

	bool Initialize(Vector position, Vector dimensions);
	void Shutdown();

	bool Render(float t);

private:
	Model * blockModel;

	//block goes from pos to pos + dim
	Vector position;  
	Vector dimensions;

};

#endif
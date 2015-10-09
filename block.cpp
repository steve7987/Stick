#include "block.h"


Block::Block(void)
{
	blockModel = 0;
}


Block::~Block(void)
{
}


bool Block::Initialize(Vector position, Vector dimensions){
	blockModel = new Model();
	if (!blockModel){
		textDump("could not create model in block class");
		return false;
	}
	if (!blockModel->Initialize(g_graphics->GetDevice(), BLOCK_MODEL, BLOCK_TEXTURE, false)){
		textDump("could not initialize model in block class");
		return false;
	}
	this->position = position;
	this->dimensions = dimensions;
	blockModel->SetPosition(position);
	blockModel->SetScale(dimensions);
	
	return true;
}
	
void Block::Shutdown(){
	if (blockModel){
		blockModel->Shutdown();
		delete blockModel;
	}

}

bool Block::Render(float t){
	g_graphics->RenderObject(blockModel, SHADER_LIGHT);
	return true;
}
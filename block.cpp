#include "block.h"


Block::Block(void)
{
	blockModel = 0;
}


Block::~Block(void)
{
}


bool Block::Initialize(Vector position, Vector dimensions, WCHAR* textureFilename){
	blockModel = new Model();
	if (!blockModel){
		textDump("could not create model in block class");
		return false;
	}
	bool result = true;
	if (textureFilename != 0){
		result = blockModel->Initialize(g_graphics->GetDevice(), BLOCK_MODEL, textureFilename, false);
	}
	else {
		result = blockModel->Initialize(g_graphics->GetDevice(), BLOCK_MODEL, BLOCK_TEXTURE, false);
	}

	if (!result){
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
		blockModel = 0;
	}

}

bool Block::Render(float t){
	g_graphics->RenderObject(blockModel, SHADER_TEXTURE);
	return true;
}

Vector Block::getPosition(){
	return position;
}
	
Vector Block::getDimensions(){
	return dimensions;
}
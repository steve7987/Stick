#ifndef _ACTIONSTATE_H_
#define _ACTIONSTATE_H_

#include <deque>
#include <time.h>

#include "classlist.h"
#include "helpers.h"
#include "gamestate.h"
#include "gamestatemanager.h"
#include "input.h"
#include "gui.h"
//game files
#include "block.h"
#include "hero.h"
#include "springcamera.h"

extern Graphics * g_graphics;
extern Gui * g_gui;
extern GameStateManager * g_gameStateManager;

class ActionState : public GameState{
public:
	ActionState();
	~ActionState();

	bool Initialize();
	void Shutdown(); 
	
	bool update(float t, Input * input);
	void render(float t);
	void onExit();
	void onEnter();

private:
	//create the deque of blocks that will be the platforms for the level
	//width and length is size of the field
	//blockPercent is percentage of possible area which should be covered
	//there will always be a 2x2 or 3x2 starting block and a 2xwidth ending block
	void CreateBlocks(unsigned int seed, int width, int length, float blockPercent, float edgeGap);  
	//determines if the array contains a rect of zeros from (i,j) to (i+y, j+x)
	bool ValidRect(int * bpath, int i, int j, int x, int z, int width); 
	
	int asSentence;

	std::deque<Block*> * blockDeque;
	
	Hero * m_Hero;
	
	SpringCamera * m_Camera;
};

#endif
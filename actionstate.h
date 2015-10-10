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
	void CreateBlocks(unsigned int seed);  //create the deque of blocks that will be the platforms for the level

	int asSentence;

	std::deque<Block*> * blockDeque;
	
	Hero * m_Hero;
	
	SpringCamera * m_Camera;
};

#endif
#ifndef _TESTSTATE_H_
#define _TESTSTATE_H_

#include <deque>
#include <time.h>

#include "classlist.h"
#include "gamestate.h"
#include "gamestatemanager.h"
#include "input.h"
#include "gui.h"
#include "helpers.h"
//game files
#include "springcamera.h"
#include "block.h"
#include "billboard.h"
#include "explosion.h"

extern Graphics * g_graphics;
extern Gui * g_gui;
extern GameStateManager * g_gameStateManager;

class TestState : public GameState{
public:
	TestState();
	~TestState();

	bool Initialize();
	void Shutdown(); 
	
	bool update(float t, Input * input);
	void render(float t);
	void onExit();
	void onEnter();

private:
	
	int asSentence;
	int debugSentence;  //used for ouputting different messages
	
	SpringCamera * m_Camera;

	Block * m_Block;  //just a block for something to look at

	Explosion * m_Explosion;
};

#endif
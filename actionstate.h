#ifndef _ACTIONSTATE_H_
#define _ACTIONSTATE_H_

#include <deque>
#include <time.h>

#include "classlist.h"
#include "gamestate.h"
#include "gamestatemanager.h"
#include "input.h"
#include "gui.h"
#include "helpers.h"
//game files
#include "hero.h"
#include "springcamera.h"
#include "environment.h"

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
	bool LevelEndReached();
	
	int asSentence;
	int debugSentence;  //used for ouputting different messages
	
	Hero * m_Hero;

	Environment * m_Environment;

	SpringCamera * m_Camera;
};

#endif
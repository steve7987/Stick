#ifndef _LEVELCOMPLETESTATE_H_
#define _LEVELCOMPLETESTATE_H_

#include "classlist.h"
#include "helpers.h"
#include "gamestate.h"
#include "gamestatemanager.h"
#include "input.h"
#include "gui.h"

extern Graphics * g_graphics;
extern Gui * g_gui;
extern GameStateManager * g_gameStateManager;

class LevelCompleteState : public GameState{
public:
	LevelCompleteState();
	~LevelCompleteState();

	bool Initialize();
	void Shutdown(); 
	
	bool update(float t, Input * input);
	void render(float t);
	void onExit();
	void onEnter();

private:
	int lcSentence;

};

#endif
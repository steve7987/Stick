#ifndef _GAMESTATEMANAGER_H_
#define _GAMESTATEMANAGER_H_

#include <string.h>
#include <map>

#include "classlist.h"
#include "input.h"
#include "fps.h"
#include "gamestate.h"
#include "graphics.h"
#include "helpers.h"

extern Graphics * g_graphics;

class GameStateManager{
public:
	GameStateManager();
	~GameStateManager();

	bool Initialize(Input * input);
	void Shutdown();

	bool update(float t, Fps * fps);
	void render(float t);

	void add(std::string name, GameState * toAdd);
	void change(std::string name);

private:
	int mouseSentence;
	int fpsSentence;

	int pmouseX, pmouseY;  //last recorded mouse x and y pos
	int pfps;  //last recorded fps value
	
	Input * input;
	
	GameState * m_currentState;
	GameState * m_nextState;
	
	std::map<std::string, GameState *> mStates;
};

#endif
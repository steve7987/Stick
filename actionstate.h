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
#include "billboard.h"
#include "enemy.h"
#include "projectile.h"

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
	void CheckCollisions();
	bool LevelEndReached();
	void AdjustCamera(float t);
	
	void CreateWave();  //creates a new wave of enemies to fight
	void BottomWave();  //creates a wave of enemies that spawn from the bottom
	void TopWave();  //creates a wave of enemies that spawn from the top
	void SideWave();  //creates a wave of enemies that spawn from the sides

	int asSentence;
	int debugSentence;  //used for ouputting different messages
	
	Hero * m_Hero;
	Environment * m_Environment;
	SpringCamera * m_Camera;

	Vector softBoundary;  // yz boundary for the level.  Ship can move a bit outside of it

	std::deque<Enemy*> * m_EnemyDeque;  //contains all of the current enemies
};

#endif
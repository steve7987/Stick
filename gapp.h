#ifndef _GAPP_H_
#define _GAPP_H_


#define WIN32_LEAN_AND_MEAN


#include <Windows.h>
#include <WindowsX.h>
#include <time.h>
//engine classes
#include "classlist.h"
#include "graphics.h"
#include "input.h"
#include "fps.h"
#include "timer.h"
#include "gui.h"
#include "gamestate.h"
#include "gamestatemanager.h"
#include "modelmanager.h"
#include "helpers.h"
//game state classes
#include "mainmenustate.h"
#include "actionstate.h"
#include "levelcompletestate.h"
#include "teststate.h"

extern ModelManager * g_modelManager;
extern TextureManager * g_textureManager;
extern Graphics * g_graphics;
extern Gui * g_gui;
extern int g_screenWidth, g_screenHeight;
extern GameStateManager * g_gameStateManager;

class Gapp{
public:
	Gapp();
	~Gapp();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	void InitializeWindows(int& x, int& y);  //screen size
	void ShutdownWindows();
	
	Graphics * m_graphics;
	TextureManager * m_textureManager;
	GameStateManager * m_gameStateManager;
	ModelManager * m_modelManager;
	Input * m_input;
	Fps * m_fps;
	Timer * m_timer;
	Gui * m_gui;
	


	int screenWidth;
	int screenHeight;
	
	bool done;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

static Gapp* GappHandle = 0;

#endif
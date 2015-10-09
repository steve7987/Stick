#include "mainmenustate.h"

MainMenuState::MainMenuState(){

}

MainMenuState::~MainMenuState(){

}


bool MainMenuState::Initialize(){
	mmSentence = g_graphics->RegisterSentence(64);
	g_graphics->ChangeSentence(mmSentence, "Main Menu State", 12, 38, 1.0f, 1.0f, 1.0f);
	g_graphics->SetVisibleSentence(mmSentence, false);
	g_gui->setVisible(GUIWINDOW_MAINMENU, false);
	return true;
}

void MainMenuState::Shutdown(){

}

	
bool MainMenuState::update(float t, Input * input){
	if (input->KeyBeenPushed(VK_ESCAPE)){
		return false;
	}
	
	if (input->MouseBeenPushed(MK_LBUTTON)){
		int mx, my;
		input->GetMouseLocation(mx, my);
		int window = g_gui->Frame(mx, my);

		if (window == GUIWINDOW_MAINMENUQUIT){
			return false;
		}
		if (window == GUIWINDOW_MAINMENUACTION){
			g_gameStateManager->change("action");
		}
		if (window == GUIWINDOW_MAINMENUANIM){
			//g_gameStateManager->change("animation");
		}
	}

	return true;
}

void MainMenuState::render(float t){
	g_graphics->StartFrame(t);
	g_graphics->EndFrame();
}

void MainMenuState::onExit(){
	g_gui->setVisible(GUIWINDOW_MAINMENU, false);
	g_graphics->SetVisibleSentence(mmSentence, false);
	
}

void MainMenuState::onEnter(){
	g_gui->setVisible(GUIWINDOW_MAINMENU, true);
	g_graphics->SetVisibleSentence(mmSentence, true);
	
}

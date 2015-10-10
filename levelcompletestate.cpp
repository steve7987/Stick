#include "levelcompletestate.h"

LevelCompleteState::LevelCompleteState(){

}

LevelCompleteState::~LevelCompleteState(){

}


bool LevelCompleteState::Initialize(){
	lcSentence = g_graphics->RegisterSentence(64);
	g_graphics->ChangeSentence(lcSentence, "Level Complete State", 12, 38, 1.0f, 1.0f, 1.0f);
	g_graphics->SetVisibleSentence(lcSentence, false);
	g_gui->setVisible(GUIWINDOW_LEVELCOMPLETE, false);
	return true;
}

void LevelCompleteState::Shutdown(){

}

	
bool LevelCompleteState::update(float t, Input * input){
	if (input->KeyBeenPushed(VK_ESCAPE)){
		g_gameStateManager->change("main menu");
	}
	
	if (input->MouseBeenPushed(MK_LBUTTON)){
		int mx, my;
		input->GetMouseLocation(mx, my);
		int window = g_gui->Frame(mx, my);

		if (window == GUIWINDOW_LEVELCOMPLETEMAIN){
			g_gameStateManager->change("main menu");
		}
		if (window == GUIWINDOW_LEVELCOMPLETENEXT){
			g_gameStateManager->change("action");
		}
	}

	return true;
}

void LevelCompleteState::render(float t){
	g_graphics->StartFrame(t);
	g_graphics->EndFrame();
}

void LevelCompleteState::onExit(){
	g_gui->setVisible(GUIWINDOW_LEVELCOMPLETE, false);
	g_graphics->SetVisibleSentence(lcSentence, false);
	
}

void LevelCompleteState::onEnter(){
	g_gui->setVisible(GUIWINDOW_LEVELCOMPLETE, true);
	g_graphics->SetVisibleSentence(lcSentence, true);
	
}

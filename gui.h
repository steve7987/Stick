#ifndef _GUI_H_
#define _GUI_H_

#include "classlist.h"
#include "graphics.h"
#include "window.h"
#include "bitmap.h"

class Graphics;
class Window;

#define GUIWINDOW_MAINMENU 1
#define GUIWINDOW_MAINMENUQUIT 2
#define GUIWINDOW_MAINMENUACTION 3
#define GUIWINDOW_MAINMENUANIM 4
#define GUIWINDOW_ACTION 5
#define GUIWINDOW_LEVELCOMPLETE 6
#define GUIWINDOW_LEVELCOMPLETEMAIN 7
#define GUIWINDOW_LEVELCOMPLETENEXT 8
#define GUIWINDOW_ANIMATIONTEST 9
#define GUIWINDOW_ANIMATIONMAIN 10

class Gui{
private:
	struct window{
		int sentenceIndex;
		int imageIndex;
		int top, bottom, left, right;
	};

public:
	Gui();
	~Gui();
	//reads gui file and creates windows based on that file
	bool Initialize(Graphics * graphics, char * guiFilename, int sWidth, int sHeight, ID3D11Device* device);  
	

	int Frame(int mouseX, int mouseY);  //takes mouse x,y and returns window id that mouse is over or -1
	void updateMouse(int mouseX, int mouseY);  //moves the mouse cursor to the input values
	void setMouseVisible(bool visible);  //toggle whether to show the mouse cursor or not
	void setVisible(int windowID, bool visible);
	void setProgressBar(int windowID, float amount);  //amount should be between 0 and 1
	bool Render(ID3D11DeviceContext* deviceContext, TextureShader * textureShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
				D3DXMATRIX orthoMatrix);  //renders all the GUI windows
	bool RenderCursor(ID3D11DeviceContext* deviceContext, TextureShader * textureShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
				D3DXMATRIX orthoMatrix);  //renders the mouse cursor
	int AddWindow(WCHAR* textureFilename, int parent, int bitmapWidth, int bitmapHeight, float tx, float ty, float bx, float by, bool vis, 
					int xpos, int ypos, bool hasBorder);  //returns id of new window
	void RemoveWindow(int id, int parentid);  //removes window but not children (although children will never be rendered if parent removed)

	void Shutdown();

private:
	bool loadFile(char * filename, ID3D11Device* device);
	
	Graphics * m_graphics;
	int numWindows;
	int screenWidth;
	int screenHeight;
	Window ** windowArray;
	int winArrayLength;
	
	Bitmap * m_Mouse;
	bool mouseVisible;
};


#endif
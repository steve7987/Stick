#ifndef _CLASSLIST_H_
#define _CLASSLIST_H_

class ActionState;
class BaseCamera;  //base type for any sort of camera
class Billboard;
class Bitmap;  //2d images
class Camera;  //basic camera for displaying a scene
class ColorShader;  //displays all vertices as a single color
class D3Dgraphics;  //low level graphics
class Font;
class FontShader;
class Fps;
class GameState;  //interface for different states of the game
class GameStateManager;  //keeps track and runs current state
class Gapp;  //main app
class Graphics;  //uses d3d to do graphics
class Gui;
class Input;  //handles input from mouse/keyboard
class LevelCompleteState;  //game state that occurs after a level has been finished
class Light;
class LightShader;
class MainMenuState;  //opening menu of game
class Model;  //3d object loaded from a text file
class ModelManager;
class ProgressBar;  //for gui
class Quaternion;
class Renderable;   //any object thats rendered in 3d world
class SelectionState;  //state for preparing ship and level
class Shield;  //handles graphics for shields
class ShieldShader;  //shader used for creating shield effects
class SpringCamera;
class Text;  
class Texture;  //holds single texture
class TextureManager;  //keeps track of all textures
class TextureShader;  //renders from a texture
class Timer;
class Vector;
class Window;  //for gui

#endif;
#include <3ds.h>
#include <citro2d.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <random>

#define MAX_SPRITES 11
#define TOP_SCREEN_WIDTH 400
#define TOP_SCREEN_HEIGHT 240
#define BOT_SCREEN_WIDTH 320
#define BOT_SCREEN_HEIGHT 240
#define PI 3.1416

//#define BOTTOM_DEBUG_MODE
//#define TOP_DEBUG_MODE

// Simple sprite struct
typedef struct
{
	C2D_Sprite spr;
} Sprite;
struct timeval begin, end;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[MAX_SPRITES];
Sprite* sprite = &sprites[0];

touchPosition touch;
int lastPos[2];//the last position where the touch input was

int GRID_SIZE = 10;
float gameSpeed = 0.1;
bool isGameOver = false;
bool isTheWallModeOn = false;
bool isSimpleGraphicsSet = false;
int enemyPos[2] = {rand()%TOP_SCREEN_WIDTH/GRID_SIZE,rand()%TOP_SCREEN_HEIGHT/GRID_SIZE};
int snakeHeading = 0;
int snakeHeadingBuffer = 0;
std::vector<int> score= {0};

//all the buttons:
// x and y are the middle of a button.
struct{int x=BOT_SCREEN_WIDTH/2;int y=BOT_SCREEN_HEIGHT/2     ;int sizeX=200;int sizeY=20;u32 color;}menuBar1;
struct{int x=BOT_SCREEN_WIDTH/2;int y=BOT_SCREEN_HEIGHT/2+20*2;int sizeX=200;int sizeY=20;u32 color;}menuBar2;
struct{int x=BOT_SCREEN_WIDTH/2;int y=BOT_SCREEN_HEIGHT/2+20*4;int sizeX=200;int sizeY=20;u32 color;}menuBar3;
struct{int x=35/2+5;int y=35/2+5;int sizeX=35;int sizeY=35;u32 color;}goBackBox;

struct toggle{int x; int y; int sizeX = 200; int sizeY = 20; u32 color; bool toggled = false;};
struct slider{int x; int y; int sizeX = 200; int sizeY = 20; int barSizeX = 50; int barSizeY = 20; u32 color; u32 barColor; float value = 10; bool isItTouched = false; float minValue = 1; float maxValue = 60;};

std::vector<std::vector<int>> snakeBodyPos = 
{
	{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
	{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
	{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2}
};



// Create colors
u32 clrClear   			   =C2D_Color32(0xFF, 0xF0, 0xFF, 0xFF);
u32 menuBarColor		   =C2D_Color32(0xB3, 0xB3, 0x83, 0xFF);
u32 highlightedMenuBarColor=C2D_Color32(0x93, 0x93, 0x53, 0xFF);
u32 ScrollBackgroundColor  =C2D_Color32(0x29, 0x29, 0x13, 0xFF);
u32 toggledColor           =C2D_Color32(0x40, 0x40, 0xB0, 0xFF);
u32 wallColor 			   =C2D_Color32(0xFF, 0x10, 0x10, 0xFF);
u32 enemyColor			   =C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
u32 snakeColor			   =C2D_Color32(0x00, 0x00, 0xFF, 0xFF);

//create text (why does citro make me do this?)
C2D_TextBuf g_staticBuf, g_dynamicBuf;
C2D_Text myText[6];

void exitTheGame(){
	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
}

int isScreenButtonPressed(int x,int y,int sizeX,int sizeY){
	//checks if player clicked on the bar
	if(touch.px>x-sizeX/2 && touch.px<x+sizeX/2 && touch.py>y-sizeY/2 && touch.py<y+sizeY/2){
		if(lastPos[0]==0 && lastPos[1]==0){
			return 2;
		}
		return 1;
	}
	return 0;
	//return 2 if a button was pressed
	//return 1 if the touch input is over the button
	//return 0 if nothing happen
}

static void initText(){
	g_staticBuf  = C2D_TextBufNew(4096);
	g_dynamicBuf = C2D_TextBufNew(4096);

	//menus
	C2D_TextParse(&myText[0], g_staticBuf, "Restart");
	C2D_TextParse(&myText[1], g_staticBuf, "Scores");
	C2D_TextParse(&myText[2], g_staticBuf, "Options");
	//pause screen
	C2D_TextParse(&myText[3], g_staticBuf, "Pause");
	//DIFERENT OPTIONS
	//toggle wall mode
	C2D_TextParse(&myText[4], g_staticBuf, "wall mode toggle");
	//toggle simple graphics
	C2D_TextParse(&myText[5], g_staticBuf, "simple graphics");

	// Optimize the static text strings
	C2D_TextOptimize(&myText[0]);
	C2D_TextOptimize(&myText[1]);
	C2D_TextOptimize(&myText[2]);
	C2D_TextOptimize(&myText[3]);
	C2D_TextOptimize(&myText[4]);
	C2D_TextOptimize(&myText[5]);
}

void pauseTheGame(C3D_RenderTarget* topScreenTarget, C3D_RenderTarget* botScreenTarget){
	#ifndef TOP_DEBUG_MODE
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_SceneBegin(topScreenTarget);
	//show the pause text
	C2D_DrawText(&myText[3], C2D_AlignCenter, TOP_SCREEN_WIDTH/2,TOP_SCREEN_HEIGHT/2, 0.5f, 0.5f, 0.5f);
	C3D_FrameEnd(0);
	#endif
	while (aptMainLoop()){
		//wait for an unpause
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START){
			if(kDown & KEY_SELECT){
				exitTheGame();
			}
			break;
		}
	}
}

void showTheScoreBoard(C3D_RenderTarget* topScreenTarget, C3D_RenderTarget* botScreenTarget){
	struct{int x=BOT_SCREEN_WIDTH-20-20/2;int y=20+100/2;int sizeX=20;int sizeY=100;u32 color = menuBarColor;}scrollBar;
	while (aptMainLoop()){
		//handle input
		hidScanInput();
		hidTouchRead(&touch);

		if(isScreenButtonPressed(goBackBox.x,goBackBox.y,goBackBox.sizeX,goBackBox.sizeY) == 2){
			break;
		}
		else if(isScreenButtonPressed(goBackBox.x,goBackBox.y,goBackBox.sizeX,goBackBox.sizeY) == 1){
			goBackBox.color = highlightedMenuBarColor;
		}
		else if(touch.px != 0 && touch.py != 0){
			goBackBox.color = menuBarColor;
			scrollBar.color = highlightedMenuBarColor;
		}
		else{
			goBackBox.color = menuBarColor;
			scrollBar.color = menuBarColor;
		}

		//move the crap if the player is touching the display
		if(touch.px != 0 && touch.py != 0){
			if(lastPos[0] != 0 && lastPos[1] != 0){
				scrollBar.y += lastPos[1] - touch.py;
			}
		}

		//limit the scroll bar movement
		if(scrollBar.y < 20+scrollBar.sizeY/2){
			scrollBar.y = 20+scrollBar.sizeY/2;
		}
		else if(scrollBar.y > BOT_SCREEN_HEIGHT-20-scrollBar.sizeY/2){
			scrollBar.y = BOT_SCREEN_HEIGHT-20-scrollBar.sizeY/2;
		}
		
		//have to do this to make buttons glow nicely in this function
		lastPos[0] = touch.px;
		lastPos[1] = touch.py;

		//draw the scores
		#ifndef TOP_DEBUG_MODE
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(topScreenTarget, clrClear);
		C2D_TargetClear(botScreenTarget, clrClear);
		C2D_SceneBegin(topScreenTarget);
		//draw a cool background
		Sprite* sprite = &sprites[6];
		C2D_SpriteSetPos(&sprite->spr,TOP_SCREEN_WIDTH/2,TOP_SCREEN_HEIGHT/2);
		C2D_DrawSprite(&sprites[6].spr);
		#endif
		
		#ifndef BOTTOM_DEBUG_MODE
		C2D_SceneBegin(botScreenTarget);
		//draw a cool background but on the bottom screen
		sprite = &sprites[1];
		C2D_SpriteSetPos(&sprite->spr,BOT_SCREEN_WIDTH/2,BOT_SCREEN_HEIGHT/2);
		C2D_DrawSprite(&sprites[1].spr);

		//draw a menu box
		C2D_DrawRectSolid(goBackBox.x-goBackBox.sizeX/2,goBackBox.y-goBackBox.sizeY/2,0,goBackBox.sizeX,goBackBox.sizeY,goBackBox.color);

		//draw a arrow that gets you back
		sprite = &sprites[7];
		C2D_SpriteSetPos(&sprite->spr,35/2+5,35/2+5);
		C2D_DrawSprite(&sprites[7].spr);

		//draw a scroll bar
		C2D_DrawRectSolid(BOT_SCREEN_WIDTH-20-20,20,0,20,BOT_SCREEN_HEIGHT-20-20,ScrollBackgroundColor);
		C2D_DrawRectSolid(scrollBar.x-scrollBar.sizeX/2,scrollBar.y-scrollBar.sizeY/2,0,scrollBar.sizeX,scrollBar.sizeY,scrollBar.color);

		//draw the scores!!!
		C2D_TextBufClear(g_dynamicBuf);
		C2D_Text dynText;
		//let's do some hackery!
		char allScores[64];
		for (int i = 0; i<=int(score.size()-1) ; i++){
			//						  scroll bar position        -           / divide so that you only move one tile when scrolling  * multiply by how many tiles do you want to be able to move
			float calibrationValue = (scrollBar.y-scrollBar.sizeY -20+100/2) / 4 * int(score.size())-10;
			//draw background for the score text
			C2D_DrawRectSolid(BOT_SCREEN_WIDTH/2-200/2, i*25+10 - calibrationValue,0,200,20,menuBarColor);

			//text stuff here
			itoa(score[i],allScores,10);
			C2D_TextParse(&dynText, g_dynamicBuf, allScores);
			C2D_TextOptimize(&dynText);
			C2D_DrawText(&dynText, C2D_AlignCenter, BOT_SCREEN_WIDTH/2, i*25+15 - calibrationValue,0,0.5f,0.5f);
		}
		C3D_FrameEnd(0);
		#endif
	}
}

void showOptions(C3D_RenderTarget* topScreenTarget, C3D_RenderTarget* botScreenTarget){
	toggle wallModeOption;
	slider gridSizeManipulator;
	slider speedManipulator;
	toggle simpleGraphics;

	wallModeOption.x = BOT_SCREEN_WIDTH/2;
	wallModeOption.y = BOT_SCREEN_HEIGHT/2-30;
	gridSizeManipulator.x = BOT_SCREEN_WIDTH/2;
	gridSizeManipulator.y = BOT_SCREEN_HEIGHT/2;
	gridSizeManipulator.color = ScrollBackgroundColor;
	gridSizeManipulator.barColor = menuBarColor;
	speedManipulator.x = BOT_SCREEN_WIDTH/2;
	speedManipulator.y = BOT_SCREEN_HEIGHT/2+30;
	speedManipulator.color = ScrollBackgroundColor;
	speedManipulator.barColor = menuBarColor;
	speedManipulator.minValue = 0.001;
	speedManipulator.maxValue = 1;
	simpleGraphics.x = BOT_SCREEN_WIDTH/2;
	simpleGraphics.y = BOT_SCREEN_HEIGHT/2+60;

	//sets the correct value after returing to settings
	wallModeOption.toggled = isTheWallModeOn;
	gridSizeManipulator.value = float(GRID_SIZE);
	speedManipulator.value = gameSpeed;
	simpleGraphics.toggled = isSimpleGraphicsSet;

	while (aptMainLoop()){
		//handle input
		hidScanInput();
		hidTouchRead(&touch);

		//go back button stuff
		if(isScreenButtonPressed(goBackBox.x,goBackBox.y,goBackBox.sizeX,goBackBox.sizeY) == 2){
			isTheWallModeOn = wallModeOption.toggled;
			GRID_SIZE = gridSizeManipulator.value;
			gameSpeed = speedManipulator.value;
			isSimpleGraphicsSet = simpleGraphics.toggled;
			break;
		}
		else if(isScreenButtonPressed(goBackBox.x,goBackBox.y,goBackBox.sizeX,goBackBox.sizeY) == 1){
			goBackBox.color = highlightedMenuBarColor;
		}
		else{
			goBackBox.color = menuBarColor;
		}

		//size manipulator stuff
		if(isScreenButtonPressed(gridSizeManipulator.x,gridSizeManipulator.y,gridSizeManipulator.sizeX,gridSizeManipulator.sizeY) == 2){
			gridSizeManipulator.isItTouched = true;
		}
		if(isScreenButtonPressed(gridSizeManipulator.x,gridSizeManipulator.y,gridSizeManipulator.sizeX,gridSizeManipulator.sizeY) == 1 || gridSizeManipulator.isItTouched){
			gridSizeManipulator.barColor = highlightedMenuBarColor;
		}
		else{
			gridSizeManipulator.barColor = menuBarColor;
		}

		//move the bar when user is touching it
		if(gridSizeManipulator.isItTouched){
			if(touch.px == 0 && touch.py == 0){
				gridSizeManipulator.isItTouched = false;
			}
			else{
				if(lastPos[0] != 0 && lastPos[1] != 0){
					//move the bar when it's touched 
					gridSizeManipulator.value -= (lastPos[0] - touch.px)/3;
				}
			}
		}

		//speed manipulator stuff
		if(isScreenButtonPressed(speedManipulator.x,speedManipulator.y,speedManipulator.sizeX,speedManipulator.sizeY) == 2){
			speedManipulator.isItTouched = true;
		}
		if(isScreenButtonPressed(speedManipulator.x,speedManipulator.y,speedManipulator.sizeX,speedManipulator.sizeY) == 1 || speedManipulator.isItTouched){
			speedManipulator.barColor = highlightedMenuBarColor;
		}
		else{
			speedManipulator.barColor = menuBarColor;
		}
		//move the bar when user is touching it
		if(speedManipulator.isItTouched){
			if(touch.px == 0 && touch.py == 0){
				speedManipulator.isItTouched = false;
			}
			else{
				if(lastPos[0] != 0 && lastPos[1] != 0){
					//move the bar when it's touched 
					speedManipulator.value -= float(lastPos[0] - touch.px)/speedManipulator.sizeX;
				}
			}
		}

		//limit the scroll bar movement for the speed manipulator
		if(speedManipulator.value < speedManipulator.minValue){
			speedManipulator.value = speedManipulator.minValue;
		}
		else if(speedManipulator.value > speedManipulator.maxValue){
			speedManipulator.value = speedManipulator.maxValue;
		}
		//limit the scroll bar movement for the grid size manipulator
		if(gridSizeManipulator.value < gridSizeManipulator.minValue){
			gridSizeManipulator.value = gridSizeManipulator.minValue;
		}
		else if(gridSizeManipulator.value > gridSizeManipulator.maxValue){
			gridSizeManipulator.value = gridSizeManipulator.maxValue;
		}

		//toggle wall mode button
		if(isScreenButtonPressed(wallModeOption.x,wallModeOption.y,wallModeOption.sizeX,wallModeOption.sizeY) == 2){
			wallModeOption.toggled = !wallModeOption.toggled;
		}
		if(wallModeOption.toggled){
			wallModeOption.color = toggledColor;
		}
		else if(!wallModeOption.toggled){
			wallModeOption.color = highlightedMenuBarColor;
		}
		//toggle simple graphics button
		if(isScreenButtonPressed(simpleGraphics.x,simpleGraphics.y,simpleGraphics.sizeX,simpleGraphics.sizeY) == 2){
			simpleGraphics.toggled = !simpleGraphics.toggled;
		}
		if(simpleGraphics.toggled){
			simpleGraphics.color = toggledColor;
		}
		else if(!simpleGraphics.toggled){
			simpleGraphics.color = highlightedMenuBarColor;
		}
		
		//have to do this to make buttons glow nicely in this function
		lastPos[0] = touch.px;
		lastPos[1] = touch.py;

		#ifndef TOP_DEBUG_MODE
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(topScreenTarget, clrClear);
		C2D_TargetClear(botScreenTarget, clrClear);
		C2D_SceneBegin(topScreenTarget);

		//draw the cool background
		sprite = &sprites[6];
		C2D_SpriteSetPos(&sprite->spr,TOP_SCREEN_WIDTH/2,TOP_SCREEN_HEIGHT/2);
		C2D_DrawSprite(&sprites[6].spr);
		#endif
		
		#ifndef BOTTOM_DEBUG_MODE
		C2D_SceneBegin(botScreenTarget);

		//draw the lower background
		sprite = &sprites[1];
		C2D_SpriteSetPos(&sprite->spr,BOT_SCREEN_WIDTH/2,BOT_SCREEN_HEIGHT/2);
		C2D_DrawSprite(&sprites[1].spr);

		//draw go back option
		C2D_DrawRectSolid(goBackBox.x-goBackBox.sizeX/2,goBackBox.y-goBackBox.sizeY/2,0, goBackBox.sizeX,goBackBox.sizeY, goBackBox.color);
		sprite = &sprites[7];
		C2D_SpriteSetPos(&sprite->spr,35/2+5,35/2+5);
		C2D_DrawSprite(&sprites[7].spr);

		//draw the wall mode option
		C2D_DrawRectSolid(wallModeOption.x-wallModeOption.sizeX/2,wallModeOption.y-wallModeOption.sizeY/2,0, wallModeOption.sizeX,wallModeOption.sizeY, wallModeOption.color);
		C2D_DrawText(&myText[4], C2D_AlignCenter, wallModeOption.x, wallModeOption.y-5, 0.5f, 0.5f, 0.5f);

		//draw the grid size slider
		C2D_DrawRectSolid(gridSizeManipulator.x-gridSizeManipulator.sizeX/2,gridSizeManipulator.y-gridSizeManipulator.sizeY/2,0, gridSizeManipulator.sizeX,gridSizeManipulator.sizeY, gridSizeManipulator.color);
		C2D_DrawRectSolid(gridSizeManipulator.x-gridSizeManipulator.sizeX/2+ (((gridSizeManipulator.value-gridSizeManipulator.minValue) / gridSizeManipulator.maxValue ) * (gridSizeManipulator.sizeX-gridSizeManipulator.barSizeX+3)),  gridSizeManipulator.y-gridSizeManipulator.sizeY/2,0, gridSizeManipulator.barSizeX,gridSizeManipulator.barSizeY, gridSizeManipulator.barColor);
		
		C2D_TextBufClear(g_dynamicBuf);
		C2D_Text dynText;
		//let's do some hackery!
		char gridSizeInText[3];
		itoa(gridSizeManipulator.value,gridSizeInText,10);//<<this is some bullcrap!! But at least it works.
		C2D_TextParse(&dynText, g_dynamicBuf, gridSizeInText);
		C2D_TextOptimize(&dynText);
		C2D_DrawText(&dynText, C2D_AlignLeft, gridSizeManipulator.x-gridSizeManipulator.sizeX/2+15 + (((gridSizeManipulator.value-gridSizeManipulator.minValue) / gridSizeManipulator.maxValue ) * (gridSizeManipulator.sizeX-gridSizeManipulator.barSizeX+3)),gridSizeManipulator.y-5,0,0.5f,0.5f);

		//draw the speed manipulator
		C2D_DrawRectSolid(speedManipulator.x-speedManipulator.sizeX/2,speedManipulator.y-speedManipulator.sizeY/2,0, speedManipulator.sizeX,speedManipulator.sizeY, speedManipulator.color);
		C2D_DrawRectSolid(speedManipulator.x-speedManipulator.sizeX/2+ (((speedManipulator.value-speedManipulator.minValue) / speedManipulator.maxValue ) * (speedManipulator.sizeX-speedManipulator.barSizeX)),  speedManipulator.y-speedManipulator.sizeY/2,0, speedManipulator.barSizeX,speedManipulator.barSizeY, speedManipulator.barColor);
		
		C2D_TextBufClear(g_dynamicBuf);
		//let's do some hackery!
		char speedInText[3];
		itoa(speedManipulator.value,speedInText,10);
		C2D_TextParse(&dynText, g_dynamicBuf, speedInText);
		C2D_TextOptimize(&dynText);
		C2D_DrawText(&dynText, C2D_AlignLeft, speedManipulator.x-speedManipulator.sizeX/2+15 + (((speedManipulator.value-speedManipulator.minValue) / speedManipulator.maxValue ) * (speedManipulator.sizeX-speedManipulator.barSizeX+3)),speedManipulator.y-5,0,0.5f,0.5f);

		//draw the toggle simple graphics button
		C2D_DrawRectSolid(simpleGraphics.x-simpleGraphics.sizeX/2,simpleGraphics.y-simpleGraphics.sizeY/2,0, simpleGraphics.sizeX,simpleGraphics.sizeY, simpleGraphics.color);
		C2D_DrawText(&myText[5], C2D_AlignCenter, simpleGraphics.x, simpleGraphics.y-5, 0.5f, 0.5f, 0.5f);

		C3D_FrameEnd(0);
		#endif
	}
}

void restartTheGame(){
	score.push_back(0);
	snakeBodyPos = {
		{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
		{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
		{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2}
	};
	//generate random enemy pos
	enemyPos[0] = rand()%TOP_SCREEN_WIDTH /GRID_SIZE;
	enemyPos[1] = rand()%TOP_SCREEN_HEIGHT/GRID_SIZE;

	snakeHeading = 0;
	isGameOver = false;
}

int waitBuffer[2] = {0};
void handleGamepadInput(C3D_RenderTarget* topScreenTarget, C3D_RenderTarget* botScreenTarget, double elapsed){
	hidScanInput();

	// Respond to user input
	u32 kDown = hidKeysDown();
	if (kDown & KEY_START){
		pauseTheGame(topScreenTarget, botScreenTarget);
	}
	if (kDown & KEY_SELECT)
		restartTheGame();
    if (kDown & KEY_DUP){
        //the code below is used to prevent the player from moving back into himself
        if(snakeHeading==2){

        }
        else{
			if(waitBuffer[0] != snakeBodyPos[0][0] || waitBuffer[1] != snakeBodyPos[0][1]){
				waitBuffer[0] = snakeBodyPos[0][0];
				waitBuffer[1] = snakeBodyPos[0][1];
				snakeHeading = 1;
			}
			else{
				snakeHeadingBuffer = 1;
			}
        }
    }
    if (kDown & KEY_DDOWN){
		//the code below is used to prevent the player from moving back into himself
        if(snakeHeading==1){

        }
        else{
			if(waitBuffer[0] != snakeBodyPos[0][0] || waitBuffer[1] != snakeBodyPos[0][1]){
				waitBuffer[0] = snakeBodyPos[0][0];
				waitBuffer[1] = snakeBodyPos[0][1];
				snakeHeading = 2;
			}
			else{
				snakeHeadingBuffer = 2;
			}
        }
    }
    if (kDown & KEY_DLEFT){
		//the code below is used to prevent the player from moving back into himself
		if(snakeHeading==4){

        }
        else{
			if(waitBuffer[0] != snakeBodyPos[0][0] || waitBuffer[1] != snakeBodyPos[0][1]){
				waitBuffer[0] = snakeBodyPos[0][0];
				waitBuffer[1] = snakeBodyPos[0][1];
				snakeHeading = 3;
			}
			else{
				snakeHeadingBuffer = 3;
			}
        }
    }
    if (kDown & KEY_DRIGHT){
		//the code below is used to prevent the player from moving back into himself
		if(snakeHeading==3){

        }
        else{
			if(waitBuffer[0] != snakeBodyPos[0][0] || waitBuffer[1] != snakeBodyPos[0][1]){
				waitBuffer[0] = snakeBodyPos[0][0];
				waitBuffer[1] = snakeBodyPos[0][1];
				snakeHeading = 4;
			}
			else{
				snakeHeadingBuffer = 4;
			}
        }
    }
}

void handleScreenInput(C3D_RenderTarget* topScreenTarget, C3D_RenderTarget* botScreenTarget){
	hidTouchRead(&touch);

	//restart button
	//on press
	if(isScreenButtonPressed(menuBar1.x,menuBar1.y,menuBar1.sizeX,menuBar1.sizeY) == 2){
		restartTheGame();
	}
	//on hover
	else if(isScreenButtonPressed(menuBar1.x,menuBar1.y,menuBar1.sizeX,menuBar1.sizeY) == 1){
		menuBar1.color=highlightedMenuBarColor;
	}
	else{
		menuBar1.color=menuBarColor;
	}

	//scoreboard button
	//on press
	if(isScreenButtonPressed(menuBar2.x,menuBar2.y,menuBar2.sizeX,menuBar2.sizeY) == 2){
		//flip this option
		showTheScoreBoard(topScreenTarget, botScreenTarget);
	}
	//on hover
	else if(isScreenButtonPressed(menuBar2.x,menuBar2.y,menuBar2.sizeX,menuBar2.sizeY) == 1){
		menuBar2.color=highlightedMenuBarColor;
	}
	else{
		menuBar2.color=menuBarColor;
	}

	//options button
	//on press
	if(isScreenButtonPressed(menuBar3.x,menuBar3.y,menuBar3.sizeX,menuBar3.sizeY) == 2){
		showOptions(topScreenTarget,botScreenTarget);
	}
	//on hover
	else if(isScreenButtonPressed(menuBar3.x,menuBar3.y,menuBar3.sizeX,menuBar3.sizeY) == 1){
		menuBar3.color=highlightedMenuBarColor;
	}
	else{
		menuBar3.color=menuBarColor;
	}

	lastPos[0] = touch.px;
	lastPos[1] = touch.py;
}

//---------------------------------------------------------------------------------
static void initSprites() {
//---------------------------------------------------------------------------------
	srand(time(NULL));

	for (size_t i = 0; i < MAX_SPRITES; i++)
	{
		Sprite* sprite = &sprites[i];

		C2D_SpriteFromSheet(&sprite->spr, spriteSheet, i);
		C2D_SpriteSetCenter(&sprite->spr, 0.5f, 0.5f);
	}
}
//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	#ifdef BOTTOM_DEBUG_MODE
	consoleInit(GFX_BOTTOM, NULL);
	#endif
	#ifdef TOP_DEBUG_MODE
	consoleInit(GFX_TOP, NULL);
	#endif

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	// Initialize sprites and text
	initSprites();
	initText();
	
	// Start measuring time
	gettimeofday(&begin, 0);

	// Main loop
	while (aptMainLoop())
	{	
		// Stop measuring time and calculate the elapsed time
		gettimeofday(&end, 0);
		long seconds = end.tv_sec - begin.tv_sec;
		long microseconds = end.tv_usec - begin.tv_usec;
		double elapsed = seconds + microseconds*1e-6;

		//handle input
		handleGamepadInput(top, bottom, elapsed);	
		handleScreenInput(top, bottom);

		//wait before the next move
		if(elapsed > gameSpeed && isGameOver==false){
			// Start measuring time
			gettimeofday(&begin, 0);

			//move all the body parts
			for (int i=snakeBodyPos.size()-1;i>=1;i--){
				snakeBodyPos[i][0] = snakeBodyPos[i-1][0];
				snakeBodyPos[i][1] = snakeBodyPos[i-1][1];
			}

			//move snakes head
			switch(snakeHeading){
				case 1:
					snakeBodyPos[0][1] --;
					break;
				case 2:
					snakeBodyPos[0][1] ++;
					break;
				case 3:
					snakeBodyPos[0][0] --;
					break;
				case 4:
					snakeBodyPos[0][0] ++;
					break;
			}
			if (snakeHeadingBuffer){
				snakeHeading = snakeHeadingBuffer;
				snakeHeadingBuffer = 0;
			}

			if(!isTheWallModeOn){
				//teleport the snake to the other side when is goes through the screen border
				if(snakeBodyPos[0][0]>=TOP_SCREEN_WIDTH/GRID_SIZE){
					snakeBodyPos[0][0] = 0;
				}
				else if(snakeBodyPos[0][1]>=TOP_SCREEN_HEIGHT/GRID_SIZE){
					snakeBodyPos[0][1] = 0;
				}
				else if(snakeBodyPos[0][0]<0){
					snakeBodyPos[0][0] = TOP_SCREEN_WIDTH/GRID_SIZE-1;
				}
				else if(snakeBodyPos[0][1]<0){
					snakeBodyPos[0][1] = TOP_SCREEN_HEIGHT/GRID_SIZE-1;
				}
			}
			
			//add a body part if it colides with an enemy
			if(snakeBodyPos[0][0]==enemyPos[0] && snakeBodyPos[0][1]==enemyPos[1]){
				//add a point to the score counter
				score[score.size()-1] ++;

				//generate random enemy pos
				generateRandomEnemyPos:
				enemyPos[0] = rand()%TOP_SCREEN_WIDTH /GRID_SIZE;
				enemyPos[1] = rand()%TOP_SCREEN_HEIGHT/GRID_SIZE;
				
				//check if the enemy spawned inside the snake. Get new random position if that's the case
				for(int snakeSeg = 0; snakeSeg<int(snakeBodyPos.size());snakeSeg++){
					if (enemyPos[0] == snakeBodyPos[snakeSeg][0] && enemyPos[1] == snakeBodyPos[snakeSeg][1]){
						goto generateRandomEnemyPos;
					}
				}

				//add a new body part
				snakeBodyPos.push_back(std::vector<int>{
               		snakeBodyPos[snakeBodyPos.size()-1][0] = snakeBodyPos[snakeBodyPos.size()-2][0],
                	snakeBodyPos[snakeBodyPos.size()-1][1] = snakeBodyPos[snakeBodyPos.size()-2][1]
           		});
			}

			//check if the snake is coliding with itself
			if(snakeHeading !=0){
				for (unsigned int i=1;i<snakeBodyPos.size();i++){
					if(snakeBodyPos[0][0] == snakeBodyPos[i][0] && snakeBodyPos[0][1] == snakeBodyPos[i][1]){
						isGameOver = true;
					}
				}
				//check if it's colliding with a wall
				if(isTheWallModeOn){
					if(snakeBodyPos[0][0] >= TOP_SCREEN_WIDTH/GRID_SIZE || snakeBodyPos[0][0] < 0 || snakeBodyPos[0][1] >= TOP_SCREEN_HEIGHT/GRID_SIZE || snakeBodyPos[0][1] < 0){
						isGameOver = true;
					}
				}
			}
		}
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		#ifndef TOP_DEBUG_MODE
		C2D_TargetClear(top, clrClear);
		#endif
		#ifndef BOTTOM_DEBUG_MODE
		C2D_TargetClear(bottom, clrClear);
		#endif
		#ifndef TOP_DEBUG_MODE
		C2D_SceneBegin(top);

		if(!isSimpleGraphicsSet){
			//draw background 
			Sprite* sprite = &sprites[10];
			C2D_SpriteSetPos(&sprite->spr,TOP_SCREEN_WIDTH/2,TOP_SCREEN_HEIGHT/2);
			C2D_DrawSprite(&sprites[10].spr);
		}

		//draw the walls
		if(isTheWallModeOn){
			C2D_DrawRectSolid(0,0,0,2,TOP_SCREEN_HEIGHT,wallColor);
			C2D_DrawRectSolid(0,0,0,TOP_SCREEN_WIDTH,2,wallColor);
			C2D_DrawRectSolid(TOP_SCREEN_WIDTH-2,0,0,2,TOP_SCREEN_HEIGHT,wallColor);
			C2D_DrawRectSolid(0,TOP_SCREEN_HEIGHT-2,0,TOP_SCREEN_WIDTH,2,wallColor);
		}

		//draw snake body
		for(int i = snakeBodyPos.size()-1;i>=0;i = i-1){
			if(!isSimpleGraphicsSet){
				/*
					head sprite = 2
					body sprite = 3
					tail sprite = 4
					turningLeft = 5
					turningRight= 8
				*/
				int whichSprite;
				if(i==0){
					whichSprite = 2;
				}
				else if(i== int(snakeBodyPos.size()-1)){
					whichSprite = 4;
				}
				else{
					whichSprite = 3;
				}
				Sprite* sprite = &sprites[whichSprite];
				
				//change the sprite if it's the corner---------
				if(i != 0 && i != int(snakeBodyPos.size()-1)){
					//check if the next body part is on the left
					if (snakeBodyPos[i-1][0] == snakeBodyPos[i][0]-1){
						//check if the previous body part is above
						if(snakeBodyPos[i+1][1] == snakeBodyPos[i][1]-1){
							whichSprite = 8;
							sprite = &sprites[whichSprite];
							C2D_SpriteSetRotation(&sprite->spr,PI/2);
						}
						//check if the previous body part is bellow
						else if(snakeBodyPos[i+1][1] == snakeBodyPos[i][1]+1){
							whichSprite = 5;
							sprite = &sprites[whichSprite];
							C2D_SpriteSetRotation(&sprite->spr,0);
						}
					}
					//check if the next body part is on the right
					if (snakeBodyPos[i-1][0] == snakeBodyPos[i][0]+1){
						//check if the previous body part is above
						if(snakeBodyPos[i+1][1] == snakeBodyPos[i][1]-1){
							whichSprite = 5;
							sprite = &sprites[whichSprite];
							C2D_SpriteSetRotation(&sprite->spr,PI);
						}
						//check if the previous body part is bellow
						else if(snakeBodyPos[i+1][1] == snakeBodyPos[i][1]+1){
							whichSprite = 8;
							sprite = &sprites[whichSprite];
							C2D_SpriteSetRotation(&sprite->spr,PI/2*3);
						}
					}
					//check if the next body part is above
					if (snakeBodyPos[i-1][1] == snakeBodyPos[i][1]-1){
						//check if the previous body part is on the left
						if(snakeBodyPos[i+1][0] == snakeBodyPos[i][0]-1){
							whichSprite = 5;
							sprite = &sprites[whichSprite];
							C2D_SpriteSetRotation(&sprite->spr,PI/2);
						}
						//check if the previous body part is on the right
						else if(snakeBodyPos[i+1][0] == snakeBodyPos[i][0]+1){
							whichSprite = 8;
							sprite = &sprites[whichSprite];
							C2D_SpriteSetRotation(&sprite->spr,PI);
						}
					}
					//check if the next body part is bellow
					if (snakeBodyPos[i-1][1] == snakeBodyPos[i][1]+1){
						//check if the previous body part is on the left
						if(snakeBodyPos[i+1][0] == snakeBodyPos[i][0]-1){
							whichSprite = 8;
							sprite = &sprites[whichSprite];
							C2D_SpriteSetRotation(&sprite->spr,0);
						}
						//check if the previous body part is on the right
						else if(snakeBodyPos[i+1][0] == snakeBodyPos[i][0]+1){
							whichSprite = 5;
							sprite = &sprites[whichSprite];
							C2D_SpriteSetRotation(&sprite->spr,PI/2*3);
						}
					}
					//skip a chunck of code if this body part is a turing one
					if (whichSprite == 5 || whichSprite == 8){
						goto skipChangingTheBodyRotationStep;
					}
				}

				//change the body parts rotation
				if(i != 0){
					if( i == int(snakeBodyPos.size()-1)){
						//check if next body part is on the left OR this body part is on the left border AND next body part is on the other side of the border
						if	   (snakeBodyPos[i-1][0] == snakeBodyPos[i][0]-1){
							C2D_SpriteSetRotation(&sprite->spr,PI/2*3);
						}
						//check if next body part is on the right
						else if(snakeBodyPos[i-1][0] == snakeBodyPos[i][0]+1){
							C2D_SpriteSetRotation(&sprite->spr,PI/2);
						}
						//check if next body part is on the top
						else if(snakeBodyPos[i-1][1] == snakeBodyPos[i][1]-1){
							C2D_SpriteSetRotation(&sprite->spr,0);
						}
						//check if next body part is on the bottom
						else if(snakeBodyPos[i-1][1] == snakeBodyPos[i][1]+1){
							C2D_SpriteSetRotation(&sprite->spr,PI);
						}
						else{
							C2D_SpriteSetRotation(&sprite->spr,PI/4);
							//this rotation is set if the snake doesn't know where the next body part is...
						}
					}
					//this is a copy of the previous bit but it does more checks, since it's not the tail
					else{
						//check if next body part is on the left OR this body part is on the left border AND next body part is on the other side of the border
						if	   (snakeBodyPos[i-1][0] == snakeBodyPos[i][0]-1 || (snakeBodyPos[i][0] - 1 <= 0 && snakeBodyPos[i+1][1] == snakeBodyPos[i][1])){
							C2D_SpriteSetRotation(&sprite->spr,PI/2*3);
						}
						//check if next body part is on the right
						else if(snakeBodyPos[i-1][0] == snakeBodyPos[i][0]+1 || (snakeBodyPos[i][0] + 1 >= TOP_SCREEN_WIDTH/GRID_SIZE && snakeBodyPos[i+1][1] == snakeBodyPos[i][1])){
							C2D_SpriteSetRotation(&sprite->spr,PI/2);
						}
						//check if next body part is on the top
						else if(snakeBodyPos[i-1][1] == snakeBodyPos[i][1]-1 || (snakeBodyPos[i][1] - 1 <= 0 && snakeBodyPos[i+1][0] == snakeBodyPos[i][0])){
							C2D_SpriteSetRotation(&sprite->spr,0);
						}
						//check if next body part is on the bottom
						else if(snakeBodyPos[i-1][1] == snakeBodyPos[i][1]+1 || (snakeBodyPos[i][1] + 1 >= TOP_SCREEN_HEIGHT/GRID_SIZE  && snakeBodyPos[i+1][0] == snakeBodyPos[i][0])){
							C2D_SpriteSetRotation(&sprite->spr,PI);
						}
						else{
							C2D_SpriteSetRotation(&sprite->spr,PI/4);
							//this rotation is set if the snake doesn't know where the next body part is...
						}
					}
				}
				//change the head direction
				else{
					if(elapsed > gameSpeed && isGameOver==false){
						//check if next body part is on the left
						if(snakeHeading==3){
							C2D_SpriteSetRotation(&sprite->spr,PI/2*3);
						}
						//check if next body part is on the right
						else if(snakeHeading==4){
							C2D_SpriteSetRotation(&sprite->spr,PI/2*1);
						}
						//check if next body part is on the top
						else if(snakeHeading==1){
							C2D_SpriteSetRotation(&sprite->spr,0);
						}
						//check if next body part is on the bottom
						else if(snakeHeading==2){
							C2D_SpriteSetRotation(&sprite->spr,PI/2*2);
						}
						else{
							C2D_SpriteSetRotation(&sprite->spr,0);
						}
					}
				}
				skipChangingTheBodyRotationStep:
				C2D_SpriteSetScale(&sprite->spr,GRID_SIZE/10,GRID_SIZE/10);
				C2D_SpriteSetPos(&sprite->spr,snakeBodyPos[i][0]*GRID_SIZE+GRID_SIZE/2,snakeBodyPos[i][1]*GRID_SIZE+GRID_SIZE/2);
				C2D_DrawSprite(&sprites[whichSprite].spr);
			}
			else{
				C2D_DrawRectSolid(snakeBodyPos[i][0]*GRID_SIZE+GRID_SIZE/2,snakeBodyPos[i][1]*GRID_SIZE+GRID_SIZE/2,0,GRID_SIZE,GRID_SIZE,snakeColor);
			}
		}
		if(!isSimpleGraphicsSet){
			//draw the enemy
			sprite = &sprites[9];
			C2D_SpriteSetScale(&sprite->spr,GRID_SIZE/10,GRID_SIZE/10);
			C2D_SpriteSetPos(&sprite->spr,enemyPos[0]*GRID_SIZE+GRID_SIZE/2,enemyPos[1]*GRID_SIZE+GRID_SIZE/2);
			C2D_DrawSprite(&sprites[9].spr);
		}
		else{
			C2D_DrawRectSolid(enemyPos[0]*GRID_SIZE+GRID_SIZE/2,enemyPos[1]*GRID_SIZE+GRID_SIZE/2,0,GRID_SIZE,GRID_SIZE,enemyColor);
		}
		//draw the scores
		C2D_TextBufClear(g_dynamicBuf);
		C2D_Text dynText;
		//let's do some hackery!
		char scoreInTextForm[3];
		itoa(score[score.size()-1],scoreInTextForm,10);//<<this is some bullcrap!! But at least it works.
		C2D_TextParse(&dynText, g_dynamicBuf, scoreInTextForm);
		C2D_TextOptimize(&dynText);
		C2D_DrawText(&dynText, C2D_AlignLeft, 5,5,0,0.5f,0.5f);

		//draw game over screen
		if(isGameOver){
			sprite = &sprites[0];
			C2D_SpriteSetPos(&sprite->spr,TOP_SCREEN_WIDTH/2,TOP_SCREEN_HEIGHT/2);
			C2D_DrawSprite(&sprites[0].spr);
		}
		#endif
		
		//draw the bottom screen:
		#ifndef BOTTOM_DEBUG_MODE
		C2D_SceneBegin(bottom);

		//background sprite
		sprite = &sprites[1];
		C2D_SpriteSetPos(&sprite->spr,BOT_SCREEN_WIDTH/2,BOT_SCREEN_HEIGHT/2);
		C2D_DrawSprite(&sprites[1].spr);
		//menus
		C2D_DrawRectSolid(menuBar1.x-menuBar1.sizeX/2,menuBar1.y-menuBar1.sizeY/2,0,menuBar1.sizeX,menuBar1.sizeY,menuBar1.color);
		C2D_DrawRectSolid(menuBar2.x-menuBar2.sizeX/2,menuBar2.y-menuBar2.sizeY/2,0,menuBar2.sizeX,menuBar2.sizeY,menuBar2.color);
		C2D_DrawRectSolid(menuBar3.x-menuBar3.sizeX/2,menuBar3.y-menuBar3.sizeY/2,0,menuBar3.sizeX,menuBar3.sizeY,menuBar3.color);
		//menu text
		C2D_DrawText(&myText[0], C2D_AlignCenter, menuBar1.x,menuBar1.y-20/3, 0.5f, 0.5f, 0.5f);
		C2D_DrawText(&myText[1], C2D_AlignCenter, menuBar2.x,menuBar2.y-20/3, 0.5f, 0.5f, 0.5f);
		C2D_DrawText(&myText[2], C2D_AlignCenter, menuBar3.x,menuBar3.y-20/3, 0.5f, 0.5f, 0.5f);
		#endif

		C3D_FrameEnd(0);
		
		//get performance data
		//printf("\x1b[22;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		//printf("\x1b[23;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		//printf("\x1b[24;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
	}
	exitTheGame();
}

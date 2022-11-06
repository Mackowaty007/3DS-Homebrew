// Simple citro2d untextured shape example
#include <3ds.h>
#include <citro2d.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <random>

#define MAX_SPRITES 7
#define TOP_SCREEN_WIDTH 400
#define TOP_SCREEN_HEIGHT 240
#define BOT_SCREEN_WIDTH 320
#define BOT_SCREEN_HEIGHT 240
#define GRID_SIZE 10

// Simple sprite struct
typedef struct
{
	C2D_Sprite spr;
} Sprite;
struct timeval begin, end;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[MAX_SPRITES];

touchPosition touch;
int lastPos[2];

bool isGameOver = false;
int enemyPos[2] = {rand()%TOP_SCREEN_WIDTH/GRID_SIZE,rand()%TOP_SCREEN_HEIGHT/GRID_SIZE};
int snakeHeading = 0;
int score = 0;
struct{int x=BOT_SCREEN_WIDTH/2;int y=BOT_SCREEN_HEIGHT/2     ;int sizeX=200;int sizeY=20;u32 color;}menuBar1;
struct{int x=BOT_SCREEN_WIDTH/2;int y=BOT_SCREEN_HEIGHT/2+20*2;int sizeX=200;int sizeY=20;u32 color;}menuBar2;
struct{int x=BOT_SCREEN_WIDTH/2;int y=BOT_SCREEN_HEIGHT/2+20*4;int sizeX=200;int sizeY=20;u32 color;}menuBar3;
std::vector<std::vector<int>> snakeBodyPos = 
{
	{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
	{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
	{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
	{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2}
};

// Create colors
u32 snakeColor = 			C2D_Color32(0x1A, 0xAC, 0x19, 0xFF);
u32 EnemyColor = 			C2D_Color32(0xF0, 0x1F, 0x0F, 0xFF);
u32 clrClear   = 			C2D_Color32(0xFF, 0xF0, 0xFF, 0xFF);
u32 menuBarColor=			C2D_Color32(0xB3, 0xB3, 0x83, 0xFF);
u32 highlightedMenuBarColor=C2D_Color32(0x93, 0x93, 0x53, 0xFF);

//create text (why does citro make me do this?)
C2D_TextBuf g_staticBuf;
C2D_Text myText[3];

static void initText(){
	g_staticBuf  = C2D_TextBufNew(4096);

	C2D_TextParse(&myText[0], g_staticBuf, "Restart");
	C2D_TextParse(&myText[1], g_staticBuf, "Scores");
	C2D_TextParse(&myText[2], g_staticBuf, "Options");
	// Optimize the static text strings
	C2D_TextOptimize(&myText[0]);
	C2D_TextOptimize(&myText[1]);
	C2D_TextOptimize(&myText[2]);
}

void exitTheGame(){
	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
}

void handleGamepadInput(){
	hidScanInput();

	// Respond to user input
	u32 kDown = hidKeysDown();
	if (kDown & KEY_START)
		exitTheGame(); // break in order to return to hbmenu
    if (kDown & KEY_DUP){
        //the code below is used to prevent the player from moving back into himself
        if(snakeHeading==2){

        }
        else{
            snakeHeading = 1;
        }
    }
    if (kDown & KEY_DDOWN){
		//the code below is used to prevent the player from moving back into himself
        if(snakeHeading==1){

        }
        else{
            snakeHeading = 2;
        }
    }
    if (kDown & KEY_DLEFT){
		//the code below is used to prevent the player from moving back into himself
		if(snakeHeading==4){

        }
        else{
            snakeHeading = 3;
        }
    }
    if (kDown & KEY_DRIGHT){
		//the code below is used to prevent the player from moving back into himself
		if(snakeHeading==3){

        }
        else{
            snakeHeading = 4;
        }
    }
}

bool isScreenButtonPressed(int x,int y,int sizeX,int sizeY){
	//checks if player clicked on the bar
	if(lastPos[0]==0 && lastPos[1]==0){
		if(touch.px>x-sizeX/2 && touch.px<x+sizeX/2 && touch.py>y-sizeY/2 && touch.py<y+sizeY/2){
			return true;
		}
	}

	lastPos[0] = touch.px;
	lastPos[1] = touch.py;
	return false;
}
void handleScreenInput(){
	hidTouchRead(&touch);

	//restart
	//highlights the buttons when hovering over them
	if(touch.px>menuBar1.x-menuBar1.sizeX/2 && touch.px<menuBar1.x+menuBar1.sizeX/2 && touch.py>menuBar1.y-menuBar1.sizeY/2 && touch.py<menuBar1.y+menuBar1.sizeY/2){
		menuBar1.color=highlightedMenuBarColor;
	}
	else{
		menuBar1.color=menuBarColor;
	}
	if(isScreenButtonPressed(menuBar1.x,menuBar1.y,menuBar1.sizeX,menuBar1.sizeY) == true){
		snakeBodyPos = {
			{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
			{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
			{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
			{(TOP_SCREEN_WIDTH/GRID_SIZE)/2  ,(TOP_SCREEN_HEIGHT/GRID_SIZE)/2},
		};
		snakeHeading = 0;
		isGameOver = false;
	}

	
	//scoreboard
	//highlights the buttons when hovering over them
	if(touch.px>menuBar2.x-menuBar2.sizeX/2 && touch.px<menuBar2.x+menuBar2.sizeX/2 && touch.py>menuBar2.y-menuBar2.sizeY/2 && touch.py<menuBar2.y+menuBar2.sizeY/2){
		menuBar2.color=highlightedMenuBarColor;
	}
	else{
		menuBar2.color=menuBarColor;
	}
	isScreenButtonPressed(menuBar2.x,menuBar2.y,menuBar2.sizeX,menuBar2.sizeY);

	//options
	//highlights the buttons when hovering over them
	if(touch.px>menuBar3.x-menuBar3.sizeX/2 && touch.px<menuBar3.x+menuBar3.sizeX/2 && touch.py>menuBar3.y-menuBar3.sizeY/2 && touch.py<menuBar3.y+menuBar3.sizeY/2){
		menuBar3.color=highlightedMenuBarColor;
	}
	else{
		menuBar3.color=menuBarColor;
	}
	isScreenButtonPressed(menuBar3.x,menuBar3.y,menuBar3.sizeX,menuBar3.sizeY);
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
	//consoleInit(GFX_BOTTOM, NULL);

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
		//handle input
		handleGamepadInput();	
		handleScreenInput();

		// Stop measuring time and calculate the elapsed time
		gettimeofday(&end, 0);
		long seconds = end.tv_sec - begin.tv_sec;
		long microseconds = end.tv_usec - begin.tv_usec;
		double elapsed = seconds + microseconds*1e-6;

		//wait before the next move
		if(elapsed > 0.1 && isGameOver==false){
			// Start measuring time
			gettimeofday(&begin, 0);

			//move all the body parts
			//(this part is before the "move snakes head" stuff cuz it fixes a bug where 2 elements are in the same space)
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
			
			//add a body part if it colides with an enemy
			if(snakeBodyPos[0][0]==enemyPos[0] && snakeBodyPos[0][1]==enemyPos[1]){
				//generate random enemy pos
				enemyPos[0] = rand()%TOP_SCREEN_WIDTH /GRID_SIZE;
				enemyPos[1] = rand()%TOP_SCREEN_HEIGHT/GRID_SIZE;
				
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
			}
		}
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_TargetClear(bottom, clrClear);
		C2D_SceneBegin(top);
		//draw the top screen:
		//draw snake body
		for(unsigned int i = 0;i<snakeBodyPos.size();i++){
			C2D_DrawRectSolid(snakeBodyPos[i][0]*GRID_SIZE,snakeBodyPos[i][1]*GRID_SIZE,0,GRID_SIZE,GRID_SIZE, snakeColor);
		}
		//draw the enemy
		C2D_DrawRectSolid(enemyPos[0]*GRID_SIZE,enemyPos[1]*GRID_SIZE,0,GRID_SIZE,GRID_SIZE,EnemyColor);

		//draw game over screen
		if(isGameOver){
			Sprite* sprite = &sprites[0];
			C2D_SpriteSetPos(&sprite->spr,TOP_SCREEN_WIDTH/2,TOP_SCREEN_HEIGHT/2);
			C2D_DrawSprite(&sprites[0].spr);
		}
		
		//draw the bottom screen:
		C2D_SceneBegin(bottom);

		//background sprite
		Sprite* sprite = &sprites[1];
		C2D_SpriteSetPos(&sprite->spr,BOT_SCREEN_WIDTH/2,BOT_SCREEN_HEIGHT/2);
		C2D_DrawSprite(&sprites[1].spr);
		//menus
		C2D_DrawRectSolid(menuBar1.x-200/2,menuBar1.y-20/2,0,200,20,menuBar1.color);
		C2D_DrawRectSolid(menuBar2.x-200/2,menuBar2.y-20/2,0,200,20,menuBar2.color);
		C2D_DrawRectSolid(menuBar3.x-200/2,menuBar3.y-20/2,0,200,20,menuBar3.color);
		//menu text
		C2D_DrawText(&myText[0], C2D_AlignCenter, menuBar1.x,menuBar1.y-20/3, 0.5f, 0.5f, 0.5f);
		C2D_DrawText(&myText[1], C2D_AlignCenter, menuBar2.x,menuBar2.y-20/3, 0.5f, 0.5f, 0.5f);
		C2D_DrawText(&myText[2], C2D_AlignCenter, menuBar3.x,menuBar3.y-20/3, 0.5f, 0.5f, 0.5f);

		C3D_FrameEnd(0);
		
		//get performance data
		//printf("\x1b[22;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		//printf("\x1b[23;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		//printf("\x1b[24;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
	}
	exitTheGame();
}


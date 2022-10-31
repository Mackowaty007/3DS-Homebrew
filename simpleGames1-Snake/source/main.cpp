// Simple citro2d untextured shape example
#include <citro2d.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <random>

#define MAX_SPRITES 8
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define GRID_SIZE 10

// Simple sprite struct
typedef struct
{
	C2D_Sprite spr;
} Sprite;
struct timeval begin, end;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[MAX_SPRITES];

bool isGameOver = false;
int enemyPos[2] = {rand()%SCREEN_WIDTH/GRID_SIZE,rand()%SCREEN_HEIGHT/GRID_SIZE};
int snakeHeading = 0;
std::vector<std::vector<int>> snakeBodyPos = 
{
	{(SCREEN_WIDTH/GRID_SIZE)/2  ,(SCREEN_HEIGHT/GRID_SIZE)/2},
	{(SCREEN_WIDTH/GRID_SIZE)/2  ,(SCREEN_HEIGHT/GRID_SIZE)/2},
	{(SCREEN_WIDTH/GRID_SIZE)/2  ,(SCREEN_HEIGHT/GRID_SIZE)/2},
	{(SCREEN_WIDTH/GRID_SIZE)/2  ,(SCREEN_HEIGHT/GRID_SIZE)/2},
};


// Create colors
u32 clrRec1    = C2D_Color32(0x6A, 0xFC, 0xA9, 0xFF);
u32 EnemyColor = C2D_Color32(0xF0, 0x1F, 0x0F, 0xFF);
u32 clrClear   = C2D_Color32(0x1F, 0x20, 0x0F, 0xFF);

//---------------------------------------------------------------------------------
static void initSprites() {
//---------------------------------------------------------------------------------
	srand(time(NULL));

	for (size_t i = 0; i < MAX_SPRITES; i++)
	{
		Sprite* sprite = &sprites[i];

		// Random image, position, rotation and speed
		C2D_SpriteFromSheet(&sprite->spr, spriteSheet, i);
		C2D_SpriteSetCenter(&sprite->spr, 0.5f, 0.5f);
		C2D_SpriteSetPos(&sprite->spr,SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
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
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	// Initialize sprites
	initSprites();
	// Start measuring time
	gettimeofday(&begin, 0);

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

        if (kDown & KEY_DUP){
            snakeHeading = 1;
        }
        if (kDown & KEY_DDOWN){
            snakeHeading = 2;
        }
        if (kDown & KEY_DLEFT){
            snakeHeading = 3;
        }
        if (kDown & KEY_DRIGHT){
        	snakeHeading = 4;
        }

		printf("\x1b[1;1H");
		for(uint i = 0;i<snakeBodyPos.size();i++)
		{
			printf("snakeBodyPos X = %i, Y = %i\n", snakeBodyPos[i][0],snakeBodyPos[i][1]);
		}		

		// Stop measuring time and calculate the elapsed time
		gettimeofday(&end, 0);
		long seconds = end.tv_sec - begin.tv_sec;
		long microseconds = end.tv_usec - begin.tv_usec;
		double elapsed = seconds + microseconds*1e-6;

		//wait before the next move
		if(elapsed > 0.1){
			// Start measuring time
			gettimeofday(&begin, 0);

			//move all the body parts
			//(this part is before the "move snakes head" stuff cuz it fixes a bug where 2 elements are in the same space)
			for (int i=snakeBodyPos.size();i>0;i--){
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


			//limit the snake head pos
			if(snakeBodyPos[0][0]>=SCREEN_WIDTH/GRID_SIZE){
				snakeBodyPos[0][0] = 0;
			}
			else if(snakeBodyPos[0][1]>=SCREEN_HEIGHT/GRID_SIZE){
				snakeBodyPos[0][1] = 0;
			}
			else if(snakeBodyPos[0][0]<0){
				snakeBodyPos[0][0] = SCREEN_WIDTH/GRID_SIZE-1;
			}
			else if(snakeBodyPos[0][1]<0){
				snakeBodyPos[0][1] = SCREEN_HEIGHT/GRID_SIZE-1;
			}
			
			//add a body part if it colides with an enemy
			if(snakeBodyPos[0][0]==enemyPos[0] && snakeBodyPos[0][1]==enemyPos[1]){
				//generate random enemy pos
				enemyPos[0] = rand()%SCREEN_WIDTH /GRID_SIZE;
				enemyPos[1] = rand()%SCREEN_HEIGHT/GRID_SIZE;
				
				//add a new body part
				snakeBodyPos.push_back(snakeBodyPos[snakeBodyPos.size()]);
				//set the new body part to the last body part position
				snakeBodyPos[snakeBodyPos.size()].push_back(snakeBodyPos[snakeBodyPos.size()-1][0]);
				snakeBodyPos[snakeBodyPos.size()].push_back(snakeBodyPos[snakeBodyPos.size()-1][1]);
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
		C2D_SceneBegin(top);

		//draw snake body
		for(unsigned int i = 0;i<snakeBodyPos.size();i++){
			C2D_DrawRectSolid(snakeBodyPos[i][0]*GRID_SIZE,snakeBodyPos[i][1]*GRID_SIZE,0,GRID_SIZE,GRID_SIZE, clrRec1);
		}
		//draw the enemy
		C2D_DrawRectSolid(enemyPos[0]*GRID_SIZE,enemyPos[1]*GRID_SIZE,0,GRID_SIZE,GRID_SIZE,EnemyColor);

		//draw game over screen
		if(isGameOver){
			C2D_DrawSprite(&sprites[0].spr);
		}
		

		C3D_FrameEnd(0);
		
		//get performance data
		printf("\x1b[22;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[23;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[24;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
} 
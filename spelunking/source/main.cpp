// Simple citro2d untextured shape example
#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define GRID_SIZE 20
#define MAP_SIZE 80
#define CAMERA_SPEED 2
#define PLAYER_SIZE 10
#define NUMBER_OF_SPRITES_IN_THE_GAME 3

//sprite struct
typedef struct
{
    C2D_Sprite spr;
} Sprite;
static C2D_SpriteSheet spriteSheet;
static Sprite sprites[NUMBER_OF_SPRITES_IN_THE_GAME];

// Create colors
u32 playerColor = C2D_Color32(0x6A, 0xFC, 0xA9, 0xFF);
u32 wallColor   = C2D_Color32(0xA0, 0x1F, 0x5F, 0xFF);
u32 clrClear   = C2D_Color32(0x0F, 0x00, 0x0F, 0xFF);

//declare variables
float Pos[2] = {1,0};
float CameraPos[2] = {0,0};
float acceleration = 0.2f;
float Speed = 1.5f;
float Velocity[2] = {0,0};
float RunningSpeed = 3.5f;
float gravityAcceleration = 0.1f;
float maxFallingSpeed = 4.0f;
bool isOnGround = false;
bool isPlayerFacingRight = true;
bool isPlayerFacingUp = false;
bool isPlayerFacingDown = false;

//main map generation
int map[MAP_SIZE][MAP_SIZE] = {{0}};

#include "colisions.hpp"

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

    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if(!spriteSheet) svcBreak(USERBREAK_PANIC);

    //sprite init
    Sprite* sprite = &sprites[0];
    for(int i=0;i<NUMBER_OF_SPRITES_IN_THE_GAME;i++){
        sprite = &sprites[i];
        C2D_SpriteFromSheet(&sprite->spr,spriteSheet,i);
    }

    //printf("\x1b[1;1HScore: %i", Score);

    for(int y=0;y<MAP_SIZE;y++){
        for(int x=0;x<MAP_SIZE;x++){
            map[x][y] = rand()%2;
        }
    }

	// Main loop
	while (aptMainLoop())
	{
        //colisions
        Colide();

        #include "input.hpp"

        //forces
        Velocity[1] += gravityAcceleration;
        if(Velocity[1] > maxFallingSpeed){Velocity[1] = maxFallingSpeed;};
        Pos[0] += Velocity[0];
        Pos[1] += Velocity[1];

        #include "render.hpp"
	}

	// Deinit libs
    C2D_SpriteSheetFree(spriteSheet);
	gfxExit();
    C2D_Fini();
    C3D_Fini();
    romfsExit();
} 

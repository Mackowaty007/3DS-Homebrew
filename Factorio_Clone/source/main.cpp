#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define PLAYER_SIZE 5
#define NUMBER_OF_SPRITES_IN_THE_GAME 7

//sprite struct
typedef struct
{
    C2D_Sprite spr;
} Sprite;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[NUMBER_OF_SPRITES_IN_THE_GAME];

int main(int argc, char* argv[])
{
    //setup
    romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if(!spriteSheet) svcBreak(USERBREAK_PANIC);

    //sprite init
    Sprite* sprite = &sprites[0];
    for(int i=0;i<NUMBER_OF_SPRITES_IN_THE_GAME;i++){
        sprite = &sprites[i];
        C2D_SpriteFromSheet(&sprite->spr,spriteSheet,i);
    }


    //variables
    int mapSize = 40;
    int gridSize = 10;
    float playerSpeed = 2;
    float cameraSpeed = 4;
    float playerPos[2] = {0,0};
    float cameraPos[2] = {0,0};
    u32 clrClear    = C2D_Color32(0xA0,0x90,0xF0,0xFF);

    #include "WorldGeneration.h"
    
	// Main loop
	while (aptMainLoop())
	{
        #include "input.h"

        #include "render.h"
    }
    C2D_SpriteSheetFree(spriteSheet);
	gfxExit();
    C2D_Fini();
    C3D_Fini();
    romfsExit();
}

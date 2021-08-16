#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define PLAYER_SIZE 5

//sprite struct
typedef struct
{
    C2D_Sprite spr;
} Sprite;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[2];

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
    //player
    Sprite* sprite = &sprites[0];
    C2D_SpriteFromSheet(&sprite->spr,spriteSheet,0);
    C2D_SpriteSetPos(&sprite->spr,100,100);
    sprite = &sprites[1];
    C2D_SpriteFromSheet(&sprite->spr,spriteSheet,1);
    C2D_SpriteSetPos(&sprite->spr,120,100);

    //variables
    int mapSize = 85;
    int gridSize = 10;
    float playerSpeed = 2;
    float cameraSpeed = 4;
    float playerPos[2] = {0,0};
    float cameraPos[2] = {0,0};
    u32 PlayerColor = C2D_Color32(0x10,0x20,0xF0,0xFF);
    u32 FloorColor = C2D_Color32(0x40,0x20,0x30,0xFF);
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

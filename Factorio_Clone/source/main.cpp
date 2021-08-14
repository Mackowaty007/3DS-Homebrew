#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define PLAYER_SIZE 5

int main(int argc, char* argv[])
{
    //setup
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    //variables
    int mapSize = 40;
    int gridSize = 10;
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
	gfxExit();
    C2D_Fini();
    C3D_Fini();
}

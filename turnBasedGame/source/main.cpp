// Simple citro2d untextured shape example
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define HEX_SIZE 20

int main(int argc, char* argv[]) {
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Create colors
	u32 clrRec1    = C2D_Color32(0x6A, 0xFC, 0xA9, 0xFF);
	u32 clrClear   = C2D_Color32(0x1F, 0x30, 0x3F, 0xFF);

    //declare variables

    int Pos[2] = {0};
    float Speed = 1;

	// Main loop
	while (aptMainLoop())
	{
        #include "input.hpp"

        //printf("\x1b[1;1Hposition: %i %i", Pos[0], Pos[1]);

        #include "draw.hpp"
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
} 

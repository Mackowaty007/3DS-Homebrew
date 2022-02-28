// Simple citro2d untextured shape example
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define ARM_LENGTH 50
#define MAX_SERVO_ANGLE 90
#define MIN_SERVO_ANGLE -90
#define PI 3.14159265
#define ARM_DEFAULT_X 160
#define ARM_DEFAULT_Y 120

int main(int argc, char* argv[]) {
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
    consoleInit(GFX_TOP, NULL);

	// Initialize the render target
	//C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

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

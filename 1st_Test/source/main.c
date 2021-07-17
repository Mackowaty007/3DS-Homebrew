// Simple citro2d untextured shape example
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Create colors

	u32 clrRec1 = C2D_Color32(0x6A, 0xFC, 0xA9, 0xFF);

	u32 clrClear = C2D_Color32(0x0F, 0x00, 0x0F, 0xFF);

    //declare variables
    int Score = -1;
    int Size = 10;
    float Pos[2] = {(SCREEN_WIDTH-Size)/2 , (SCREEN_HEIGHT-Size)/2};

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

        u32 kHeld = hidKeysHeld();
        if (kHeld & KEY_UP){
            Pos[1] --;
        }
        if (kHeld & KEY_DOWN){
            Pos[1] ++;
        }
        if (kHeld & KEY_LEFT){
            Pos[0] --;
        }
        if (kHeld & KEY_RIGHT){
            Pos[0] ++;
        }

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		C2D_DrawRectSolid(Pos[0], Pos[1],0,Size,Size, clrRec1);

        printf("\x1b[1;1HSCORE: %1.0i",Score);

		C3D_FrameEnd(0);
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}

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

	u32 clrRec1    = C2D_Color32(0x6A, 0xFC, 0xA9, 0xFF);
    u32 EnemyColor = C2D_Color32(0xF0, 0x1F, 0x0F, 0xFF);

	u32 clrClear   = C2D_Color32(0x0F, 0x00, 0x0F, 0xFF);

    //declare variables
    int Score = -1;
    int Size = 10;
    float EnemyPos[2] = {100,100};
    float Pos[2]    = {(SCREEN_WIDTH-Size)/2 , (SCREEN_HEIGHT-Size)/2};
    float Speed = 1.5f;

    printf("\x1b[1;1HScore: %i", Score);

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

        u32 kHeld = hidKeysHeld();
        if (kHeld & KEY_DUP){
            Pos[1] --;
        }
        if (kHeld & KEY_DDOWN){
            Pos[1] ++;
        }
        if (kHeld & KEY_DLEFT){
            Pos[0] --;
        }
        if (kHeld & KEY_DRIGHT){
            Pos[0] ++;
        }

        //circle pad position
        circlePosition pos;
        hidCircleRead(&pos);

        float float_posX = (float)pos.dx;
        float float_posY = (float)pos.dy;

        Pos[0] += float_posX/154 * Speed;
        Pos[1] -= float_posY/154 * Speed;

        //check collisions
        if (Pos[0]+Size>EnemyPos[0]-Size/2 && Pos[0]<EnemyPos[0]+Size/2 && Pos[1]+Size>EnemyPos[1]-Size/2 && Pos[1]<EnemyPos[1]+Size/2){
            Score ++;
            EnemyPos[0] = rand() % SCREEN_WIDTH;
            EnemyPos[1] = rand() % SCREEN_HEIGHT;
            printf("\x1b[1;1HScore: %i    ", Score);
        }


		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		C2D_DrawRectSolid(Pos[0], Pos[1],0,Size,Size, clrRec1);
        C2D_DrawCircleSolid(EnemyPos[0],EnemyPos[1],0,Size/2,EnemyColor);

		C3D_FrameEnd(0);
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
} 

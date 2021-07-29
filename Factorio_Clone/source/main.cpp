#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "WorldGeneration.h"

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define PLAYER_SIZE 5

int main(int argc, char* argv[]){

	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    
    //variables
    int mapSize = 10;
    int gridSize = 10;
    int floorMap[mapSize][mapSize] = {{0}};
    //printf("%i \n",GenerateFloor(10));
    GenerateFloor(10);

    
    float playerPos[2] = {0,0};
    float cameraPos[2] = {0,0};
    u32 PlayerColor = C2D_Color32(0x10,0x20,0xF0,0xFF);
    u32 FloorColor = C2D_Color32(0x40,0x20,0x30,0xFF);
    u32 clrClear    = C2D_Color32(0xA0,0x90,0xF0,0xFF);

	// Main loop
	while (aptMainLoop())
	{
        //input
		hidScanInput();
        circlePosition pos;
        hidCircleRead(&pos);
		u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();


        playerPos[0] += static_cast<float>(pos.dx)/154;
        playerPos[1] -= static_cast<float>(pos.dy)/154;


		if (kDown & KEY_START) break;
        
        if(kHeld & KEY_X){
            cameraPos[0] += static_cast<float>(pos.dx)/154;
            cameraPos[1] -= static_cast<float>(pos.dy)/154;
        }
        else{
            cameraPos[0] = playerPos[0]-SCREEN_WIDTH/2;
            cameraPos[1] = playerPos[1]-SCREEN_HEIGHT/2;
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

        //rendering
        for(int y=0;y<mapSize;y++){
            for(int x=0;x<mapSize;x++){
                switch(floorMap[y][x]){
                    case 0:
                        FloorColor = C2D_Color32(0x40,0x20,0x30,0xFF);
                        break;
                    case 1:
                        FloorColor = C2D_Color32(0x10,0x20,0x10,0xFF);
                        break;
                    case 2:
                        FloorColor = C2D_Color32(0xF0,0xF0,0xF0,0xFF);
                        break;
                    case 3:
                        FloorColor = C2D_Color32(0x10,0x70,0xA0,0xFF);
                        break;
                    case 4:
                        FloorColor = C2D_Color32(0x40,0xFF,0x30,0xFF);
                        break;
                    default:
                        FloorColor = C2D_Color32(0xFF,0x00,0x00,0xFF);
                }
                C2D_DrawRectSolid(x*gridSize-cameraPos[0],y*gridSize-cameraPos[1],0,gridSize,gridSize,FloorColor);
            }
        }
        C2D_DrawRectSolid(playerPos[0]-cameraPos[0],playerPos[1]-cameraPos[1],0,PLAYER_SIZE,PLAYER_SIZE, PlayerColor);
    
	    C3D_FrameEnd(0);
	}

	gfxExit();
    C2D_Fini();
    C3D_Fini();
	return 0;
}

#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define TILE_SIZE 10

int main(int argc, char* argv[]){

	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    //variables
    int playerSize = 5;
    int map[SCREEN_WIDTH/TILE_SIZE][SCREEN_HEIGHT/TILE_SIZE]=
    {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,7,6,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,3,0,3,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,3,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    
    float Pos[2] = {100,100};
    float Velocity[2] = {0,0};
    float Gravity = 0.08f;
    //the closer ground resistance is to 1 the slipperier the ground is (can't be smaller then one cuz then you will speed up slippering)
    float GroundResistance = 1.1f;

    int JumpCounter = 1;
    int MaxJumpNumber = 1;
    float Acceleration = 0.1f;
    float JumpSpeed = 1.7f;
    float MaxFallingSpeed = 2;
    float MaxSpeed = 2;
    bool isOnGround = false;
    int isOnWall = 0;

    u32 PlayerColor = C2D_Color32(0x10,0x20,0xF0,0xFF);
    u32 tileColor   = C2D_Color32(0x10,0xF0,0x90,0xFF);
    u32 clrClear    = C2D_Color32(0xA0,0x90,0xF0,0xFF);

	// Main loop
	while (aptMainLoop())
	{
        //input
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;

        if (kDown & KEY_B){
            if(isOnGround == true){
                Velocity[1] = -JumpSpeed;
            }
            else if(JumpCounter > 0 && isOnWall == 0){
                Velocity[1] = -JumpSpeed;
                JumpCounter --;
            }
            if(isOnWall != 0){
                Velocity[1] = -JumpSpeed;
                Velocity[0] = JumpSpeed * isOnWall;
            }
        }
        Velocity[1] += Gravity;
        

        u32 kHeld = hidKeysHeld();



        if(kHeld & KEY_LEFT){
            Velocity[0] -= Acceleration;
        }
        else if(kHeld & KEY_RIGHT){
            Velocity[0] += Acceleration;
        }
        else{
            Velocity[0] = Velocity[0]/GroundResistance;
        }

        //speed limit
        if(Velocity[0]>MaxSpeed){
            Velocity[0] = MaxSpeed;
        }
        else if(Velocity[0]<-MaxSpeed){
            Velocity[0] = -MaxSpeed;
        }
        if(Velocity[1]>MaxFallingSpeed){
            Velocity[1] = MaxFallingSpeed;
        }
        
        Pos[0] += Velocity[0];
        Pos[1] += Velocity[1];


        //collisions
        isOnWall = 0;
        isOnGround = false;
        //left
        if(map[(int)Pos[1]/TILE_SIZE][(int)Pos[0]/TILE_SIZE] && map[(int)(Pos[1]+playerSize)/TILE_SIZE][(int)Pos[0]/TILE_SIZE]){
            Velocity[0] = 0;
            Pos[0] += (Pos[0]/TILE_SIZE)-((int)Pos[0]/TILE_SIZE);
            isOnWall = 1;
        }
        //right
        if(map[(int)Pos[1]/TILE_SIZE][(int)(Pos[0]+playerSize)/TILE_SIZE] && map[(int)(Pos[1]+playerSize)/TILE_SIZE][(int)(Pos[0]+playerSize)/TILE_SIZE]){
            Velocity[0] = 0;
            Pos[0] -= ((Pos[0]+playerSize)/TILE_SIZE)-((int)(Pos[0]+playerSize)/TILE_SIZE);
            isOnWall = -1;
        }
        //up
        if(map[(int)Pos[1]/TILE_SIZE][(int)Pos[0]/TILE_SIZE] && map[(int)Pos[1]/TILE_SIZE][(int)(Pos[0]+playerSize)/TILE_SIZE]){
            Velocity[1] = 0;
            Pos[1] += (Pos[1]/TILE_SIZE)-((int)Pos[1]/TILE_SIZE);
        }
        //down
        if(map[(int)(Pos[1]+playerSize)/TILE_SIZE][(int)Pos[0]/TILE_SIZE] && map[(int)(Pos[1]+playerSize)/TILE_SIZE][(int)(Pos[0]+playerSize)/TILE_SIZE]){
            Velocity[1] = 0;
            Pos[1] -= ((Pos[1]+playerSize)/TILE_SIZE)-((int)(Pos[1]+playerSize)/TILE_SIZE);
            JumpCounter = MaxJumpNumber;
            isOnGround = true;
        }


        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

        for(int y = 0;y<SCREEN_WIDTH/TILE_SIZE;y++){
            for(int x = 0;x<SCREEN_HEIGHT/TILE_SIZE;x++){
                //set color based on the map number
                if(map[y][x] == 1){
                    tileColor = C2D_Color32(0x10,0xF0,0x90,0xFF);
                }
                else if(map[y][x] == 2){
                    tileColor = C2D_Color32(0x80,0x60,0x40,0xFF);
                }
                else if(map[y][x] == 3){
                    tileColor = C2D_Color32(0x30,0x40,0xAA,0xFF);
                }
                else{
                    tileColor = C2D_Color32(0xFF,0x30,0x29,0xFF);
                }
                //draw the tile
                if(map[y][x]){
                    C2D_DrawRectSolid(x*TILE_SIZE, y*TILE_SIZE,0,TILE_SIZE,TILE_SIZE, tileColor);
                }
            }
        }

        C2D_DrawRectSolid(Pos[0], Pos[1],0,playerSize,playerSize, PlayerColor);
    
	    C3D_FrameEnd(0);
	}

	gfxExit();
    C2D_Fini();
    C3D_Fini();
	return 0;
}

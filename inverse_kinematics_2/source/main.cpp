// Simple citro2d untextured shape example
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "findIntersections.hpp"


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define ARM_LENGTH_1 50
#define ARM_LENGTH_2 30
#define ARM_DEFAULT_X 160
#define ARM_DEFAULT_Y 120
/*
#define NUMBER_OF_ARMS 2
#define MAX_SERVO_ANGLE_1 180
#define MIN_SERVO_ANGLE_1 0
#define MAX_SERVO_ANGLE_2 240
#define MIN_SERVO_ANGLE_2 60
#define MAX_SERVO_ANGLE_3 90
#define MIN_SERVO_ANGLE_3 90
*/
#define PI 3.14159265


int Pos[2] = {0};
float Speed = 1;
/*
void limitTheAngles(){

}
*/
void computeTheAngles(int point1X,int point1Y,int point2X,int point2Y,int point3X,int point3Y){
    int arm1Vector [2] = {point2X - point1X,point2Y - point1Y};
    int arm2Vector [2] = {point3X - point2X, point3Y - point2Y};
    //float alphaAngle =
    printf("\x1b[1;1H %i x and %i y", arm1Vector[0],arm1Vector[1]);

}

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
	//u32 clrRec1    = C2D_Color32(0x6A, 0xFC, 0xA9, 0xFF);
    u32 clrLines   = C2D_Color32(0xF0, 0x0A, 0x0A, 0xFF);
    //u32 clrLines2  = C2D_Color32(0x00, 0xFA, 0x00, 0xFF);
	u32 clrClear   = C2D_Color32(0x1F, 0x30, 0x3F, 0xFF);

	// Main loop
	while (aptMainLoop())
	{
        #include "input.hpp"

        // radius and center of circles
        Circle c1(ARM_LENGTH_1, ARM_DEFAULT_X, ARM_DEFAULT_Y);
        Circle c2(ARM_LENGTH_2, Pos[0],Pos[1]);
        Point2d i1, i2;

        // intersections point(s)
        size_t i_points = c1.intersect(c2, i1, i2);

        computeTheAngles(0,0,i1.x,i1.y,Pos[0],Pos[1])

        #include "draw.hpp"
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
} 

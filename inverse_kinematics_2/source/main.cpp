// Simple citro2d untextured shape example
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


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


float computeTheAngles(int point1X,int point1Y,int point2X,int point2Y){
    float x = point2X-point1X;
    float y = point2Y-point1Y;
    float alphaAngle = atan(y/x);
    if (point2X < point1X){
        alphaAngle = atan(y/x) + PI;
    }

    return alphaAngle;
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
	u32 clrRec1    = C2D_Color32(0xFA, 0xFC, 0xf9, 0xFF);
    u32 clrLines   = C2D_Color32(0xF0, 0x0A, 0x0A, 0xFF);
    u32 clrLines2  = C2D_Color32(0x00, 0x9A, 0x00, 0xFF);
	u32 clrClear   = C2D_Color32(0x1F, 0x30, 0x3F, 0xFF);

	// Main loop
	while (aptMainLoop())
	{
        #include "input.hpp"

        float Bx = Pos[0];
        float By = Pos[1];
        float Ax = ARM_DEFAULT_X;
        float Ay = ARM_DEFAULT_Y;
        float Ar = ARM_LENGTH_1;
        float Br = ARM_LENGTH_2;
        float d = hypot(Bx - Ax, By - Ay);


        float ex = (Bx - Ax) / d;
        float ey = (By - Ay) / d;

        float x = (Ar * Ar - Br * Br + d * d) / (2 * d);
        float y = sqrt(Ar * Ar - x * x);

        float P1[2] = {
            Ax + x * ex - y * ey,
            Ay + x * ey + y * ex
        };

        float P2[2] = {
            Ax + x * ex + y * ey,
            Ay + x * ey - y * ex
        };

        printf("\x1b[2;1H %f -x and %f -y", P2[0],P2[1]);

        printf("\x1b[7;1H alpha angle = %f", computeTheAngles(ARM_DEFAULT_X,ARM_DEFAULT_Y,P2[0],P2[1]) /PI*180);
        printf("\x1b[8;1H betha angle = %f", (computeTheAngles(P2[0],P2[1],Pos[0],Pos[1])-computeTheAngles(ARM_DEFAULT_X,ARM_DEFAULT_Y,P2[0],P2[1])) /PI*180);




        #include "draw.hpp"
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
} 

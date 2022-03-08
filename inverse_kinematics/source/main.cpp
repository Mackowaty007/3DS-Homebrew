// Simple citro2d untextured shape example
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define ARM_LENGTH 50
#define MAX_SERVO_ANGLE 120
#define MIN_SERVO_ANGLE -90
#define MAX_SERVO_ANGLE_2 180
#define MIN_SERVO_ANGLE_2 0
#define PI 3.14159265
#define ARM_DEFAULT_X 160
#define ARM_DEFAULT_Y 120

int Pos[2] = {0};

class MathVariables{
    public:
        float hypotenuse;
        float opposite;
        float adjacent;

        float x;
        float y;

        float alpha_angle;
        float betha_angle;
        float alpha_betha_angle;
        float gamma_angle;
        float delta_angle;
        float gamma_relative_angle;

        void computeTheAngles(int finalPositionX,int finalPositionY){
            //przeciwprostokątna
            hypotenuse = sqrt(pow(ARM_DEFAULT_X-finalPositionX,2)+pow(ARM_DEFAULT_Y-finalPositionY,2));

            //przyprostokątne
            opposite = ARM_DEFAULT_Y-finalPositionY;
            adjacent = ARM_DEFAULT_X-finalPositionX;

            x = hypotenuse/2;
            y = sqrt(pow(ARM_LENGTH,2)-pow(x,2));

            //kąt nachylenia punktu docelowego
            alpha_angle = asin(opposite/hypotenuse);
            betha_angle = atan(y/x);
            alpha_betha_angle = alpha_angle+betha_angle;
            gamma_angle = PI - 2*betha_angle;
            delta_angle = PI - alpha_angle - PI/2;

            //limit the servo angles
            if(alpha_betha_angle/PI*180 > MAX_SERVO_ANGLE_2){
                alpha_betha_angle = MAX_SERVO_ANGLE_2*PI/180;
            }
            else if(alpha_betha_angle/PI*180 < MIN_SERVO_ANGLE_2){
                alpha_betha_angle = MIN_SERVO_ANGLE_2;
            }

            if(gamma_angle/PI*180 < 180-MAX_SERVO_ANGLE){
                gamma_angle = (180-MAX_SERVO_ANGLE)*PI/180;
            }

            if(finalPositionX<ARM_DEFAULT_X){
                printf("\x1b[5;1H works!!      ");
            }
            else{
                printf("\x1b[5;1H doesn't work");
            }

            gamma_relative_angle = gamma_angle - (PI - alpha_betha_angle);
        }
};

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
    u32 clrLines   = C2D_Color32(0xF0, 0x0A, 0x0A, 0XFF);
	u32 clrClear   = C2D_Color32(0x1F, 0x30, 0x3F, 0xFF);

    //declare variables

    //get the minimum and maximum arm length
    const float minArmLength = sqrt(
    pow(cos(MAX_SERVO_ANGLE)*ARM_LENGTH+cos(MAX_SERVO_ANGLE_2)*ARM_LENGTH,2)+
    pow(-sin(MAX_SERVO_ANGLE)*ARM_LENGTH+-sin(MAX_SERVO_ANGLE_2)*ARM_LENGTH,2));

    const float maxArmLength = ARM_LENGTH*2;

    float Speed = 1;

    MathVariables Servo1;
    MathVariables Servo2;

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

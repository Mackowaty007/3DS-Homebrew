#ifndef DRAW_HPP
#define DRAW_HPP

Servo1.computeTheAngles(Pos[0],Pos[1]);
Servo2.computeTheAngles(
    ARM_DEFAULT_X + cos(Servo1.alpha_betha_angle)*ARM_LENGTH,
    ARM_DEFAULT_Y + -sin(Servo1.alpha_betha_angle)*ARM_LENGTH
);

printf("\x1b[1;1Hangle 1: %f", (Servo1.alpha_betha_angle)/PI*180);
printf("\x1b[2;1Hangle 2: %f", (Servo1.gamma_angle)/PI*180);


// Render the scene
C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
//C2D_TargetClear(top, clrClear);
//C2D_SceneBegin(top);

//if(Pos[0] > ARM_DEFAULT_X){

    C2D_TargetClear(bot, clrClear);
    C2D_SceneBegin(bot);

    //arm1
    C2D_DrawLine(ARM_DEFAULT_X,ARM_DEFAULT_Y,clrRec1,ARM_DEFAULT_X + cos(Servo2.alpha_betha_angle)*ARM_LENGTH,ARM_DEFAULT_Y + -sin(Servo2.alpha_betha_angle)*ARM_LENGTH,clrRec1,1,0);

    //arm2
    C2D_DrawLine(ARM_DEFAULT_X + cos(Servo2.alpha_betha_angle)*ARM_LENGTH,ARM_DEFAULT_Y+-sin(Servo2.alpha_betha_angle)*ARM_LENGTH ,clrRec1,
    ARM_DEFAULT_X+cos(Servo2.alpha_betha_angle)*ARM_LENGTH+cos(Servo2.gamma_relative_angle)*ARM_LENGTH,
    ARM_DEFAULT_Y+-sin(Servo2.alpha_betha_angle)*ARM_LENGTH+-sin(Servo2.gamma_relative_angle)*ARM_LENGTH
    ,clrRec1,1,0);

    //arm3
    C2D_DrawLine(
    ARM_DEFAULT_X+cos(Servo2.alpha_betha_angle)*ARM_LENGTH+cos(Servo2.gamma_relative_angle)*ARM_LENGTH,
    ARM_DEFAULT_Y+-sin(Servo2.alpha_betha_angle)*ARM_LENGTH+-sin(Servo2.gamma_relative_angle)*ARM_LENGTH,
    clrRec1,
    ARM_DEFAULT_X+cos(Servo2.alpha_betha_angle)*ARM_LENGTH+cos(Servo2.gamma_relative_angle)*ARM_LENGTH+cos(Servo1.gamma_relative_angle)*ARM_LENGTH,
    ARM_DEFAULT_Y+-sin(Servo2.alpha_betha_angle)*ARM_LENGTH+-sin(Servo2.gamma_relative_angle)*ARM_LENGTH+-sin(Servo1.gamma_relative_angle)*ARM_LENGTH
    ,clrRec1,1,0);

    //THIS IS NO LONGER AN ARM!!
    C2D_DrawLine(ARM_DEFAULT_X,ARM_DEFAULT_Y,clrLines,ARM_DEFAULT_X + cos(Servo1.alpha_betha_angle)*ARM_LENGTH,ARM_DEFAULT_Y + -sin(Servo1.alpha_betha_angle)*ARM_LENGTH,clrLines,1,0);

    //this neither
    C2D_DrawLine(ARM_DEFAULT_X,ARM_DEFAULT_Y,clrLines,
    ARM_DEFAULT_X+cos(Servo1.alpha_betha_angle)*ARM_LENGTH+cos(Servo1.gamma_relative_angle)*ARM_LENGTH,
    ARM_DEFAULT_Y+-sin(Servo1.alpha_betha_angle)*ARM_LENGTH+-sin(Servo1.gamma_relative_angle)*ARM_LENGTH
    ,clrLines,1,0);
/*
}
else{
    //here the arm should turn around
}*/

C3D_FrameEnd(0);

#endif

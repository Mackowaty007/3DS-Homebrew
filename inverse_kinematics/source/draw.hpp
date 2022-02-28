#ifndef DRAW_HPP
#define DRAW_HPP

// Render the scene
C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
//C2D_TargetClear(top, clrClear);
//C2D_SceneBegin(top);


C2D_TargetClear(bot, clrClear);
C2D_SceneBegin(bot);

//przeciwprostokątna
float hypotenuse = sqrt(pow(ARM_DEFAULT_X-Pos[0],2)+pow(ARM_DEFAULT_Y-Pos[1],2));
//przyprostokątne
float opposite = ARM_DEFAULT_Y-Pos[1];
float adjacent = ARM_DEFAULT_X-Pos[0];

float x = hypotenuse/2;
float y = sqrt(pow(ARM_LENGTH,2)-pow(x,2));

//kąt nachylenia punktu docelowego
float alpha_angle = asin(opposite/hypotenuse);

float betha_angle = atan(y/x);

float alpha_betha_angle = alpha_angle+betha_angle;

float gamma_angle = PI - 2*betha_angle;

float delta_angle = PI - alpha_angle - PI/2;

if(alpha_betha_angle/PI*180 > 180){
    alpha_betha_angle = 180*PI/180;
}
else if(alpha_betha_angle/PI*180 < 0){
    alpha_betha_angle = 0;
}

if(gamma_angle/PI*180 < MAX_SERVO_ANGLE){
    gamma_angle = MAX_SERVO_ANGLE*PI/180;
}/*
else if(gamma_angle/PI*180 > MIN_SERVO_ANGLE){
    gamma_angle = MIN_SERVO_ANGLE*PI/180;
    printf("\x1b[4;1H something ain't right ");
}*/

float gamma_relative_angle = gamma_angle - (PI - alpha_betha_angle);

printf("\x1b[1;1Hangle 1: %f", (alpha_betha_angle)/PI*180);
printf("\x1b[2;1Hangle 2: %f", (gamma_angle)/PI*180);

if(Pos[0] > ARM_DEFAULT_X){
    C2D_DrawLine(ARM_DEFAULT_X,ARM_DEFAULT_Y,clrRec1,ARM_DEFAULT_X + cos(alpha_betha_angle)*ARM_LENGTH,ARM_DEFAULT_Y + -sin(alpha_betha_angle)*ARM_LENGTH,clrRec1,1,0);

    C2D_DrawLine(ARM_DEFAULT_X + cos(alpha_betha_angle)*ARM_LENGTH,ARM_DEFAULT_Y+-sin(alpha_betha_angle)*ARM_LENGTH ,clrRec1,
    ARM_DEFAULT_X+cos(alpha_betha_angle)*ARM_LENGTH+cos(gamma_relative_angle)*ARM_LENGTH,
    ARM_DEFAULT_Y+-sin(alpha_betha_angle)*ARM_LENGTH+-sin(gamma_relative_angle)*ARM_LENGTH
    ,clrRec1,1,0);

    C2D_DrawLine(10,10,clrRec1,123,10,clrRec1,1,0);
}
else{
    C2D_DrawLine(ARM_DEFAULT_X,ARM_DEFAULT_Y,clrRec1,ARM_DEFAULT_X + -cos(alpha_betha_angle)*ARM_LENGTH,ARM_DEFAULT_Y + -sin(alpha_betha_angle)*ARM_LENGTH,clrRec1,1,0);

    C2D_DrawLine(ARM_DEFAULT_X + -cos(alpha_betha_angle)*ARM_LENGTH,ARM_DEFAULT_Y+-sin(alpha_betha_angle)*ARM_LENGTH ,clrRec1,
    ARM_DEFAULT_X+-cos(alpha_betha_angle)*ARM_LENGTH+-cos(gamma_relative_angle)*ARM_LENGTH,
    ARM_DEFAULT_Y+-sin(alpha_betha_angle)*ARM_LENGTH+-sin(gamma_relative_angle)*ARM_LENGTH
    ,clrRec1,1,0);
}

C3D_FrameEnd(0);

#endif

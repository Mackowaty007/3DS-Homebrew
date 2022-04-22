#ifndef DRAW_HPP
#define DRAW_HPP

// Render the scene
C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
//C2D_TargetClear(top, clrClear);
//C2D_SceneBegin(top);

//if(Pos[0] > ARM_DEFAULT_X){

    C2D_TargetClear(bot, clrClear);
    C2D_SceneBegin(bot);

    /*
    //draw debug circles
    C2D_DrawCircleSolid(ARM_DEFAULT_X,ARM_DEFAULT_Y,0,ARM_LENGTH_1,clrLines2);
    C2D_DrawCircleSolid(Pos[0],Pos[1],0,ARM_LENGTH_2,clrLines2);
    */

    C2D_DrawLine(
    ARM_DEFAULT_X,
    ARM_DEFAULT_Y,
    clrLines,
    i1.x(),
    i1.y(),
    clrLines,1,0);

    C2D_DrawLine(
    i1.x(),
    i1.y(),
    clrLines,
    Pos[0],
    Pos[1],
    clrLines,1,0);


C3D_FrameEnd(0);

#endif

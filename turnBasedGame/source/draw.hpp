#ifndef DRAW_HPP
#define DRAW_HPP

// Render the scene
C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
C2D_TargetClear(top, clrClear);
C2D_SceneBegin(top);

//variables
int drawHexX = Pos[0] % static_cast<int>(HEX_SIZE * 1.5f) - 2*HEX_SIZE;
int drawHexY = Pos[1] % HEX_SIZE - HEX_SIZE;

//draw half of the hexagons
while(drawHexX<SCREEN_WIDTH){
    while(drawHexY<SCREEN_HEIGHT){
        C2D_DrawLine(drawHexX+HEX_SIZE/4,drawHexY,clrRec1,drawHexX+HEX_SIZE/4*3,drawHexY,clrRec1,1,0);
        C2D_DrawLine(drawHexX+HEX_SIZE/4,drawHexY,clrRec1,drawHexX,drawHexY+HEX_SIZE/2,clrRec1,1,0);
        C2D_DrawLine(drawHexX+HEX_SIZE/4*3,drawHexY,clrRec1,drawHexX+HEX_SIZE,drawHexY+HEX_SIZE/2,clrRec1,1,0);
        drawHexY += HEX_SIZE;
    }
    drawHexX += HEX_SIZE * 1.5f;
    drawHexY = Pos[1] % HEX_SIZE - HEX_SIZE;
}

drawHexX = HEX_SIZE * 0.75f + Pos[0] % static_cast<int>(HEX_SIZE * 1.5f) - 2*HEX_SIZE;
drawHexY = HEX_SIZE/2 + Pos[1] % HEX_SIZE - HEX_SIZE;

//draw half of the hexagons0
while(drawHexX<SCREEN_WIDTH){
    while(drawHexY<SCREEN_HEIGHT){
        C2D_DrawLine(drawHexX+HEX_SIZE/4,drawHexY,clrRec1,drawHexX+HEX_SIZE/4*3,drawHexY,clrRec1,1,0);
        C2D_DrawLine(drawHexX+HEX_SIZE/4,drawHexY,clrRec1,drawHexX,drawHexY+HEX_SIZE/2,clrRec1,1,0);
        C2D_DrawLine(drawHexX+HEX_SIZE/4*3,drawHexY,clrRec1,drawHexX+HEX_SIZE,drawHexY+HEX_SIZE/2,clrRec1,1,0);
        drawHexY += HEX_SIZE;
    }
    drawHexX += HEX_SIZE * 1.5f;
    drawHexY = HEX_SIZE/2 + Pos[1] % HEX_SIZE - HEX_SIZE;
}

drawHexX = 0;
drawHexY = 0;

C2D_DrawRectSolid(Pos[0],Pos[1],0,10,10,clrRec1);

C3D_FrameEnd(0);

#endif

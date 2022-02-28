#ifndef RENDER_HPP
#define RENDER_HPP
// Render the scene
C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
C2D_TargetClear(top, clrClear);
C2D_SceneBegin(top);


for    (int y=Pos[1]/GRID_SIZE;y<(CameraPos[1]+SCREEN_HEIGHT)/GRID_SIZE;y++){
    for(int x=Pos[0]/GRID_SIZE;x<(CameraPos[0]+SCREEN_WIDTH )/GRID_SIZE;x++){
        if(map[x][y] == 1 && MAP_SIZE>x && x>0 && MAP_SIZE>y && y>0){
            C2D_DrawRectSolid(x*GRID_SIZE-CameraPos[0],y*GRID_SIZE-CameraPos[1],0,GRID_SIZE,GRID_SIZE, wallColor);
        }
    }
}

C2D_DrawRectSolid(SCREEN_WIDTH/2-GRID_SIZE/2 + Pos[0]-CameraPos[0], SCREEN_HEIGHT/2-GRID_SIZE/2 + Pos[1]-CameraPos[1],0,PLAYER_SIZE,PLAYER_SIZE,playerColor);
/*
if(isPlayerFacingUp){
    sprite = &sprites[1];
}
else if(isPlayerFacingDown){
    sprite = &sprites[2];
}
else{
    sprite = &sprites[0];
}
C2D_SpriteSetPos(&sprite->spr,SCREEN_WIDTH/2-GRID_SIZE/2 + Pos[0]-CameraPos[0]-GRID_SIZE/4, SCREEN_HEIGHT/2-GRID_SIZE/2 + Pos[1]-CameraPos[1]-GRID_SIZE/2);

if(isPlayerFacingRight){
    C2D_SpriteSetScale(&sprites->spr,-1,1);
}
else{
    C2D_SpriteSetScale(&sprites->spr,1,1);
}

if(isPlayerFacingUp){
    C2D_DrawSprite(&sprites[1].spr);
}
else if(isPlayerFacingDown){
    C2D_DrawSprite(&sprites[2].spr);
}
else{
    C2D_DrawSprite(&sprites[0].spr);
}
*/

C3D_FrameEnd(0);
#endif

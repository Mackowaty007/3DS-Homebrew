#ifndef RENDER_H
#define RENDER_H

C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
C2D_TargetClear(top, clrClear);
C2D_SceneBegin(top);

//RENDERING
//land and water
for(int y=cameraPos[1]/gridSize;y<(cameraPos[1]+SCREEN_HEIGHT)/gridSize;y++){
    for(int x=(cameraPos[0]/gridSize)-1;x<(cameraPos[0]+SCREEN_WIDTH)/gridSize;x++){
        if(x>=0 && y >=0 && x<mapSize && y<mapSize){
            switch(floorMap[y][x]){
                case 0:
                    sprite = &sprites[0];
                    C2D_SpriteSetPos(&sprite->spr,x*gridSize-cameraPos[0],y*gridSize-cameraPos[1]);
                    C2D_DrawSprite(&sprites[0].spr);
                    break;
                case 1:
                    sprite = &sprites[1];
                    C2D_SpriteSetPos(&sprite->spr,x*gridSize-cameraPos[0],y*gridSize-cameraPos[1]);
                    C2D_DrawSprite(&sprites[1].spr);
                    break;
                default:
                    sprite = &sprites[3];
                    C2D_SpriteSetPos(&sprite->spr,x*gridSize-cameraPos[0],y*gridSize-cameraPos[1]);
                    C2D_DrawSprite(&sprites[3].spr);
                    break;
            }
        }
    }
}
//render minerals
/*
for(int y=cameraPos[1]/gridSize;y<(cameraPos[1]+SCREEN_HEIGHT)/gridSize;y++){
    for(int x=(cameraPos[0]/gridSize)-1;x<(cameraPos[0]+SCREEN_WIDTH)/gridSize;x++){
        if(x>=0 && y >=0 && x<mapSize && y<mapSize){
            switch(mineralMap[y][x]){
                case 1:
                    sprite = &sprites[4];
                    C2D_SpriteSetPos(&sprite->spr,x*gridSize-cameraPos[0],y*gridSize-cameraPos[1]);
                    C2D_DrawSprite(&sprites[4].spr);
                    break;
                case 2:
                    sprite = &sprites[5];
                    C2D_SpriteSetPos(&sprite->spr,x*gridSize-cameraPos[0],y*gridSize-cameraPos[1]);
                    C2D_DrawSprite(&sprites[5].spr);
                    break;
                default:
                    break;
                }
        }
    }
}
*/
//draw Player
sprite = &sprites[2];
C2D_SpriteSetPos(&sprite->spr,playerPos[0]-cameraPos[0],playerPos[1]-cameraPos[1]);
C2D_DrawSprite(&sprites[2].spr);


//C2D_DrawLine(10,100,FloorColor,20,200,FloorColor,5,0); draws a line - pretty cool

C3D_FrameEnd(0);

#endif
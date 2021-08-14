#ifndef RENDER_H
#define RENDER_H

C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
C2D_TargetClear(top, clrClear);
C2D_SceneBegin(top);

//rendering
printf("player Pos x:%f y:%f \n",playerPos[0],playerPos[1]);

for(int y=cameraPos[1]/gridSize;y<(cameraPos[1]+SCREEN_HEIGHT)/gridSize;y++){
    for(int x=(cameraPos[0]/gridSize)-1;x<(cameraPos[0]+SCREEN_WIDTH)/gridSize;x++){
        if(x>=0 && y >=0 && x<mapSize && y<mapSize){
            switch(floorMap[y][x]){
                case 0:
                    FloorColor = C2D_Color32(0x40,0x20,0x30,0xFF);
                    break;
                case 1:
                    FloorColor = C2D_Color32(0x10,0x20,0x10,0xFF);
                    break;
                case 2:
                    FloorColor = C2D_Color32(0xF0,0xF0,0xF0,0xFF);
                    break;
                case 3:
                    FloorColor = C2D_Color32(0x10,0x70,0xA0,0xFF);
                    break;
                case 4:
                    FloorColor = C2D_Color32(0x40,0xFF,0x30,0xFF);
                    break;
                default:
                    FloorColor = C2D_Color32(0xFF,0x00,0x00,0xFF);
                }
            C2D_DrawRectSolid(x*gridSize-cameraPos[0],y*gridSize-cameraPos[1],0,gridSize,gridSize,FloorColor);
        }
    }
}
C2D_DrawRectSolid(playerPos[0]-cameraPos[0],playerPos[1]-cameraPos[1],0,PLAYER_SIZE,PLAYER_SIZE, PlayerColor); 
C3D_FrameEnd(0);

#endif
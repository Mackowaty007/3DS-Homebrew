#ifndef INPUT_H
#define INPUT_H

        //input
		hidScanInput();
        circlePosition pos;
        hidCircleRead(&pos);
		u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();


		if (kDown & KEY_START) break;
        
        if(kHeld & KEY_X){
            cameraPos[0] += static_cast<float>(pos.dx)/154*cameraSpeed;
            cameraPos[1] -= static_cast<float>(pos.dy)/154*cameraSpeed;
        }
        else{
            cameraPos[0] = playerPos[0]-SCREEN_WIDTH/2;
            cameraPos[1] = playerPos[1]-SCREEN_HEIGHT/2;

            playerPos[0] += static_cast<float>(pos.dx)/154*playerSpeed;
            playerPos[1] -= static_cast<float>(pos.dy)/154*playerSpeed;
        }
        
#endif
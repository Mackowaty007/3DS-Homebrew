#ifndef INPUT_HPP
#define INPUT_HPP
hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();
        circlePosition pos;
        hidCircleRead(&pos);
        float float_posX = static_cast<float>(pos.dx);
        float float_posY = static_cast<float>(pos.dy);

        //set the correct facing direction
        if(float_posX < 0){
            isPlayerFacingRight = false;
        }
        else if(float_posX > 0){
            isPlayerFacingRight = true;
        }

        if(float_posY < -10){
            isPlayerFacingDown = true;
        }
        else{
            isPlayerFacingDown = false;
        }
        if(float_posY > 10){
            isPlayerFacingUp = true;
        }
        else{
            isPlayerFacingUp = false;
        }

		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

        //set the max speed
        if(kHeld & KEY_R){
            Velocity[0] = float_posX/154 * RunningSpeed;
        }
        else{
            Velocity[0] = float_posX/154 * Speed;
        }

        if (kDown & KEY_B){
            Pos[1] ++;
            Velocity[1] = -3;
        }

        if(kDown & KEY_Y){
            if(isPlayerFacingDown){
                map[static_cast<int>((Pos[0]+SCREEN_WIDTH/2)/GRID_SIZE)][static_cast<int>((Pos[1]+SCREEN_HEIGHT/2-PLAYER_SIZE/2+GRID_SIZE/2)/GRID_SIZE)] = 0;
            }
            else if(isPlayerFacingUp){
                map[static_cast<int>((Pos[0]+SCREEN_WIDTH/2)/GRID_SIZE)][static_cast<int>((Pos[1]+SCREEN_HEIGHT/2-PLAYER_SIZE/2-GRID_SIZE*4/5)/GRID_SIZE)] = 0;
            }
            else{
                if(isPlayerFacingRight){
                    map[static_cast<int>((Pos[0]+SCREEN_WIDTH/2+GRID_SIZE/2)/GRID_SIZE)][static_cast<int>((Pos[1]+SCREEN_HEIGHT/2-PLAYER_SIZE/2)/GRID_SIZE)] = 0;
                }
                else{
                    map[static_cast<int>((Pos[0]+SCREEN_WIDTH/2-GRID_SIZE)/GRID_SIZE)][static_cast<int>((Pos[1]+SCREEN_HEIGHT/2-PLAYER_SIZE/2)/GRID_SIZE)] = 0;
                }
            }
        }

        //camera controls
        if (kHeld & KEY_DUP){
            if (kHeld & KEY_R){
                CameraPos[1] -= CAMERA_SPEED;
            }
            else{
                CameraPos[1] -= CAMERA_SPEED * 2;
            }
        }
        else if (kHeld & KEY_DDOWN){
            if (kHeld & KEY_R){
                CameraPos[1] += CAMERA_SPEED;
            }
            else{
                CameraPos[1] += CAMERA_SPEED * 2;
            }
        }
        else{
            CameraPos[1] = Pos[1];
        }
        if (kHeld & KEY_DLEFT){
            if (kHeld & KEY_R){
                CameraPos[0] -= CAMERA_SPEED;
            }
            else{
                CameraPos[0] -= CAMERA_SPEED * 2;
            }
        }
        else if (kHeld & KEY_DRIGHT){
            if (kHeld & KEY_R){
                CameraPos[0] += CAMERA_SPEED;
            }
            else{
                CameraPos[0] += CAMERA_SPEED * 2;
            }
        }
        else{
            CameraPos[0] = Pos[0];
        }
#endif

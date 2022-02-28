#ifndef INPUT_HPP
#define INPUT_HPP
hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

        u32 kHeld = hidKeysHeld();
        if (kHeld & KEY_DUP){
            Pos[1] --;
        }
        if (kHeld & KEY_DDOWN){
            Pos[1] ++;
        }
        if (kHeld & KEY_DLEFT){
            Pos[0] --;
        }
        if (kHeld & KEY_DRIGHT){
            Pos[0] ++;
        }

        //circle pad position
        circlePosition pos;
        hidCircleRead(&pos);

        float float_posX = (float)pos.dx;
        float float_posY = (float)pos.dy;

        Pos[0] += float_posX/154 * Speed;
        Pos[1] -= float_posY/154 * Speed;
#endif

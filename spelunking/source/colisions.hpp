#ifndef COLISIONS_HPP
#define COLISIONS_HPP

//colisions
void Colide(){
    if(map[static_cast<int>((Pos[0]+SCREEN_WIDTH/2-PLAYER_SIZE/2)/GRID_SIZE)][static_cast<int>((Pos[1]+SCREEN_HEIGHT/2-PLAYER_SIZE)/GRID_SIZE)]){
        printf("\x1b[1;1H collision T!!");
        //allign to Y
        if(Pos[1]>0){
            Pos[1] = static_cast<int>(Pos[1]) - static_cast<int>(Pos[1])%GRID_SIZE + PLAYER_SIZE;
        }
        else{
            Pos[1] = static_cast<int>(Pos[1]) - static_cast<int>(Pos[1])%GRID_SIZE + PLAYER_SIZE - GRID_SIZE;
        }
        Velocity[1]=0;
    }
    else{
        printf("\x1b[1;1H                ");
    }
    if(map[static_cast<int>((Pos[0]+SCREEN_WIDTH/2-PLAYER_SIZE/2)/GRID_SIZE)][static_cast<int>((Pos[1]+SCREEN_HEIGHT/2)/GRID_SIZE)]){
        printf("\x1b[2;2H collision B!!");
        //allign to Y
        if(Pos[1]>0){
            Pos[1] = static_cast<int>(Pos[1]) - static_cast<int>(Pos[1])%GRID_SIZE;
        }
        else{
            Pos[1] = static_cast<int>(Pos[1]) - static_cast<int>(Pos[1])%GRID_SIZE - GRID_SIZE;
        }
        Velocity[1]= 0;
    }
    else{
        printf("\x1b[2;2H                    ");
    }
    if(map[static_cast<int>((Pos[0]+SCREEN_WIDTH/2)/GRID_SIZE)][static_cast<int>((Pos[1]+SCREEN_HEIGHT/2-PLAYER_SIZE/2)/GRID_SIZE)]){
        printf("\x1b[3;3H collision R!!");
        //allign to X
        if(Pos[0]>0){
            Pos[0] = static_cast<int>(Pos[0]) - static_cast<int>(Pos[0])%GRID_SIZE;
        }
        else{
            Pos[0] = static_cast<int>(Pos[0]) - static_cast<int>(Pos[0])%GRID_SIZE - PLAYER_SIZE ;//ADSFJKJ;KLDFJ;SADVJKCJD WAHAT THE FUCK IT THIS BULLLSHIT!!!!!!!!!!!!!!!
        }
        Velocity[0]=0;
    }
    else{
        printf("\x1b[3;3H                    ");
    }
    if(map[static_cast<int>((Pos[0]+SCREEN_WIDTH/2-PLAYER_SIZE)/GRID_SIZE)][static_cast<int>((Pos[1]+SCREEN_HEIGHT/2-PLAYER_SIZE/2)/GRID_SIZE)]){
        printf("\x1b[4;4H collision L!!");

        //allign to X
        if(Pos[0]>0){
            Pos[0] = static_cast<int>(Pos[0]) - static_cast<int>(Pos[0])%GRID_SIZE + PLAYER_SIZE;
        }
        else{
            Pos[0] = static_cast<int>(Pos[0]) - static_cast<int>(Pos[0])%GRID_SIZE + PLAYER_SIZE - GRID_SIZE;
        }
        Velocity[0]=0;
    }
    else{
        printf("\x1b[4;4H                 ");
    }
}
#endif

#ifndef WORLD_GENERATION_H
#define WORLD_GENERATION_H

#define MAX_RANDOM_NUMBER 999

//generate the floor
int floorMap[mapSize][mapSize];
//generate the first tile
floorMap[0][0] = rand() % MAX_RANDOM_NUMBER;
//generate the first horizontal line
for(int x=1;x<mapSize;x++){
    floorMap[0][x] = floorMap[0][x-1] + (rand() % 500) - 250;
    //block the value between 999 and 0
    if(floorMap[0][x] > MAX_RANDOM_NUMBER){
        floorMap[0][x] = MAX_RANDOM_NUMBER;
    }
    else if(floorMap[0][x] < 0){
        floorMap[0][x] = 0;
    }
}
//generate the first vertical line
for(int y=1;y<mapSize;y++){
    floorMap[y][0] = floorMap[y-1][0] + (rand() % 500) - 250;
    //block the value between 999 and 0
    if(floorMap[y][0] > MAX_RANDOM_NUMBER){
        floorMap[y][0] = MAX_RANDOM_NUMBER;
    }
    else if(floorMap[y][0] < 0){
        floorMap[y][0] = 0;
    }
}
//generate everything else
for(int x=1;x<mapSize;x++){
    for(int y=1;y<mapSize;y++){
        floorMap[y][x] = ((floorMap[y-1][x] + floorMap[y][x-1]) /2) + (rand() % 250) - 125;

        //block the value between 999 and 0
        if(floorMap[y][x] > MAX_RANDOM_NUMBER){
            floorMap[y][x] = MAX_RANDOM_NUMBER;
        }
        else if(floorMap[y][x] < 0){
            floorMap[y][x] = 0;
        }
    }
}

//round everything
for(int x=0;x<mapSize;x++){
    for(int y=0;y<mapSize;y++){
        floorMap[y][x] = floorMap[y][x] / 500;
    }
}



/*
for(int x=0;x<mapSize;x++){
    for(int y=0;y<mapSize;y++){
        floorMap[y][x] = rand() % 1;
    }
}
*/


//generate minerals
int mineralMap[mapSize][mapSize];
for(int x=0;x<mapSize;x++){
    for(int y=0;y<mapSize;y++){
       mineralMap[y][x] = rand() % 3;
    }
}
#endif

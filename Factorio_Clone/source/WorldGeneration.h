#ifndef WORLD_GENERATION_H
#define WORLD_GENERATION_H

#define MAX_RANDOM_NUMBER 999

//generate the floor
int floorMap[mapSize][mapSize];


for(int x=0;x<mapSize;x++){
    for(int y=0;y<mapSize;y++){
        floorMap[y][x] = rand() % 2;
    }
}



//generate minerals
int mineralMap[mapSize][mapSize];
for(int x=0;x<mapSize;x++){
    for(int y=0;y<mapSize;y++){
       mineralMap[y][x] = rand() % 3;
    }
}
#endif

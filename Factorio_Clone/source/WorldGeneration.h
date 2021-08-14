#ifndef WORLD_GENERATION_H
#define WORLD_GENERATION_H

//generate the floor
int floorMap[mapSize][mapSize];;
for (int x=0;x<mapSize;x++){
    for (int y=0;y<mapSize;y++){
        floorMap[y][x] = rand() % 5;
    }
}
#endif

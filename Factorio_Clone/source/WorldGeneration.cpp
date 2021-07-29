#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int GenerateFloor(int mapSize){
    int map[mapSize][mapSize] = {
        {1,1,0,0,1,0,1,1,1,1},
        {1,2,3,4,5,6,7,8,9,0},
        {1,1,0,0,1,0,1,1,1,1},
        {1,1,0,0,1,0,1,1,1,1},
        {1,1,0,0,1,0,1,1,1,1},
        {1,1,0,0,1,0,1,1,1,1},
        {1,1,0,0,1,0,1,1,1,1},
        {1,1,0,0,1,0,1,1,1,1},
        {1,1,0,0,1,0,1,1,1,1},
        {1,1,0,0,1,0,1,1,1,1}
    };
    printf("%i\n",&map);
    return 10;
}

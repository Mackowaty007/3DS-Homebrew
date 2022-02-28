#ifndef _PERLIN_H_
#define _PERLIN_H_

#define PI 3.14159265
#define GRID_S 50

int lerp(int A,int B,float t){
    return A+t*(B-A);
}
int slerp(int A,int B,float t){
    return A+((1+sin(t*PI-PI/2))/2)*(B-A);
}
float returnNoise(int posX, int posY){

}

#endif

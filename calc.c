#include <stdio.h>
#include "wrappers.h"

int ADD(int x, int y){
    int result;
    result = x + y;
    return result;
}

int MUL(int x, int y){
    int result;
    result = x * y;
    return result;
}

float DIV(float x, float y){
    float result;
    result = x / y;
    return result;
}
int MOD(int x, int y){
    int result;
    result = x % y;
    return result;
}

void INFO(){{
    
}}




int main(){
    int x = 100;
    int y = 30;
    printf("x+y= %d\n", ADD(x,y));
    printf("x*y= %d\n", MUL(x,y));
    printf("x/y= %f\n", DIV(x,y));
    printf("x mod y= %d\n", MOD(x,y));

    return 0;
}
#include <stdio.h>
#define BINPRINT(x)\
    if (sizeof(x) != sizeof(long long)){\
        for (int i = 8*sizeof(x)-1; i>=0; i--)\
        {\
            if (x & (1<<i))\
                {printf("1");}\
            else\
                {printf("0");}\
        }\
    }\
    else{\
        for (int i = 8*sizeof(x)-1; i>=0; i--)\
        {\
            if (x & ((long long)1<<i))\
                {printf("1");}\
            else\
                {printf("0");}\
        }\
    }\



int main(){
    char c = -3;
    if (c){
        printf("binprint char c = -3:\n");
        BINPRINT(c);
        printf("\n");
    }
    else{
        printf("Zero!\n");
    }
    printf("binprint short 7:\n");
    BINPRINT((short)7);
    printf("\n");
    printf("binprint int y = 25:\n");
    int y = 25;
    BINPRINT(y);
    printf("\n");
    long x = 15;
    if (x){
        printf("binprint long x = 15 :\n");
        BINPRINT(x);
        printf("\n");
    }
    else
        {printf("Zero!\n");}
    printf("binprint long long x=15 :\n");
    BINPRINT((long long)x);
    return 0;
}

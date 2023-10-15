#include <stdio.h>

int main(){
    unsigned i, j;
    for (i=0, j=0; i<10; i += 1, j+=2)
        if (j<i)
            return 1;
        
    return 0;
}
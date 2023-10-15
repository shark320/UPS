#include <stdio.h>

#define OUT_BYTES 1
#define OUT_CHARS 2

void var_dump(void* var, unsigned long bytes, int flag){
	unsigned long i = 0;
	char byte;
	if (flag == OUT_BYTES){
		printf("Var dump flag set to BYTES\n");
	}else if (flag == OUT_CHARS){
		printf("Var dump flag set to CHARS\n");
	}
	for (;i<bytes;++i){
		byte = *((char *)(var+i));
		if (flag == OUT_BYTES){
			printf("%02x ", byte);
		}else if (flag == OUT_CHARS){
			printf("%c ", byte);
		}
		if ((bytes)%8==7){
			printf("\n");
		}
	}
}

int main(void){
    int test = 10;
    var_dump(&test, sizeof(test), OUT_BYTES);

    return 0;
}
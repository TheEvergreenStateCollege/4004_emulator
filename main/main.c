#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* TODO
 * Open file and error check
 *	Make the file read always add 256 values to the ROM with it adding 0's after the file is over
 * Main loop to progress through program
 * Functions for instructions
 * Add remaining registers
*/
typedef unsigned _BitInt(4) uint4_t;
typedef struct i4004_flags {
	 uint8_t chip_select;
} i4004_flags;
typedef struct i4004_registers {
	union {
		struct {
			unsigned char r0;
			unsigned char r1;
		};
		unsigned short r01;
	};
} i4004_registers;

i4004_flags flags;
i4004_registers registers;
uint8_t i4001[256];
uint4_t i4002[20];

void NOP(void);

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage is %s <filename> where <filename> is the desired 4004 program.\n", argv[0]);
		return 1;
	}
	
        //open file
	FILE *file = fopen(argv[1], "r");
	if(!file) {
		printf("%s is not a valid file.\n", argv[1]);
		exit(1);
	}

	uint8_t WORD;
	int i = 0;
	while((WORD = (uint8_t) fgetc(file)) != EOF) { 
		i4001[i] = WORD;
		++i;
	}
	
	//Free memory and close file
	free(file);
	return 0;
}

void NOP(void) {
	return;
}

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
	uint8_t r01, r23, r45, r67, r89, r1011, r1213, r1415;
} i4004_registers;

i4004_flags flags;
i4004_registers registers;
uint8_t i4001[256];
uint4_t i4002[20];

//Memory operation function prototypes
void insertInRegister(uint4_t value, uint8_t reg);
uint4_t fetchFromRegister(uint8_t reg);

//Instruction function prototypes
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
	while(i < 256) { 
		i4001[i] = 0;
		WORD = (uint8_t) fgetc(file);
		i4001[i] = WORD;
		// (WORD = (uint8_t) fgetc(file)) 
		++i;
	}

	for(i = 0; i < 256; ++i) {
		printf("%d\n", (int) i4001[i]);
	}
	
	//Free memory and close file
	fclose(file);
	return 0;
}

void insertInRegister(uint4_t value, uint8_t reg) {
	switch (reg) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		default:
	}

	return;
}

void NOP(void) {
	return;
}

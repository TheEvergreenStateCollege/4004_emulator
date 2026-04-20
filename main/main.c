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
typedef unsigned _BitInt(12) uint12_t;
typedef struct i4004_flags {
	 uint8_t chip_select;
} i4004_flags_t;
typedef struct i4004_registers {
	uint8_t r01, r23, r45, r67, r89, r1011, r1213, r1415;
	uint12_t pc;
	uint12_t s1, s2, s3;
	uint4_t ac;
} i4004_registers_t;

i4004_flags_t flags;
i4004_registers_t registers;
uint8_t i4001[256];
uint4_t i4002[20];

//Memory operation function prototypes
void insertInRegister(uint4_t value, uint8_t reg);
uint4_t fetchFromRegister(int reg);

//Instruction function prototypes
void NOP(void);

int main(int argc, char *argv[]) {
	/*if(argc != 2) {
		printf("Usage is %s <filename> where <filename> is the desired 4004 program.\n", argv[0]);
		return 1;
	}*/
	
	/*
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
	*/

	registers.r01 = 0xAB;
	printf("%x, %x\n", (int)fetchFromRegister(0), (int)fetchFromRegister(1));

	// Start of the Actual Emulator
	registers.pc = 0;  // Initialize program counter to 0
	int i = 0;
	uint8_t instruction;
	
	while(i < 1000) {
		instruction = i4001[registers.pc];
		// get the first 4 bits from the instruction
		++i;
	}


	//Free memory and close file
	//fclose(file);
	return 0;
}
/*
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
*/
uint4_t fetchFromRegister(int reg) {
	uint4_t value = 0;
	printf("%d\n", reg);
	switch (reg) {
		case 0:
			value = (uint4_t) ((registers.r01 & 0xF0) >> 4);
			break;
		case 1:
			value = (uint4_t) (registers.r01 & 0x0F);
			break;
		case 2:
			value = (uint4_t) ((registers.r23 & 0xF0) >> 4);
			break;
		case 3:
			value = (uint4_t) (registers.r23 & 0x0F);
			break;
		case 4:
			value = (uint4_t) ((registers.r45 & 0xF0) >> 4);
			break;
		case 5:
			value = (uint4_t) (registers.r45 & 0x0F);
			break;
		case 6:
			value = (uint4_t) ((registers.r67 & 0xF0) >> 4);
			break;
		case 7:
			value = (uint4_t) (registers.r67 & 0x0F);
			break;
		case 8:
			value = (uint4_t) ((registers.r89 & 0xF0) >> 4);
			break;
		case 9:
			value = (uint4_t) (registers.r89 & 0x0F);
			break;
		case 10:
			value = (uint4_t) ((registers.r1011 & 0xF0) >> 4);
			break;
		case 11:
			value = (uint4_t) (registers.r1011 & 0x0F);
			break;
		case 12:
			value = (uint4_t) ((registers.r1213 & 0xF0) >> 4);
			break;
		case 13:
			value = (uint4_t) (registers.r1213 & 0x0F);
			break;
		case 14:
			value = (uint4_t) ((registers.r1415 & 0xF0) >> 4);
			break;
		case 15:
			value = (uint4_t) (registers.r1415 & 0x0F);
			break;
	}
	return value;
}

void NOP(void) {
	return;
}

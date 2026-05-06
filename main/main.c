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
void IAC(void);

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage is %s <filename> where <filename> is the desired 4004 program.\n", argv[0]);
		return 1;
	}

	uint8_t readBuffer[512];
	int buffer;
	int i = 0;
	
	
	//open file
	FILE *fp = fopen(argv[1], "r");
	if(!fp) {
		printf("%s is not a valid file.\n", argv[1]);
		exit(1);
	}

	while(i < 512) {
		buffer = fgetc(fp);
		if(buffer == EOF) {
			for(; i < 512; i++) {
				readBuffer[i] = 0x0;
			}
			i = 512;
		} else {
			switch (buffer) {
				case '0': readBuffer[i] = 0x0; i++; break;
				case '1': readBuffer[i] = 0x1; i++; break;
				case '2': readBuffer[i] = 0x2; i++; break;
				case '3': readBuffer[i] = 0x3; i++; break;
				case '4': readBuffer[i] = 0x4; i++; break;
				case '5': readBuffer[i] = 0x5; i++; break;
				case '6': readBuffer[i] = 0x6; i++; break;
				case '7': readBuffer[i] = 0x7; i++; break;
				case '8': readBuffer[i] = 0x8; i++; break;
				case '9': readBuffer[i] = 0x9; i++; break;
				case 'A': readBuffer[i] = 0xA; i++; break;
				case 'B': readBuffer[i] = 0xB; i++; break;
				case 'C': readBuffer[i] = 0xC; i++; break;
				case 'D': readBuffer[i] = 0xD; i++; break;
				case 'E': readBuffer[i] = 0xE; i++; break;
				case 'F': readBuffer[i] = 0xF; i++; break;
				default: break;
			}
		}
		
	}
	fclose(fp);
	
	int j = 0;
	for(i = 0; i < 512; i += 2) {
		uint8_t buf = (readBuffer[i] << 4) | readBuffer[i+1];
		i4001[j] = buf;
		j++;
	}

	for(j = 0; j < 256; j++) {
		printf("%x ", (int) i4001[j]);
	}
	

	// Start of the Actual Emulator
	registers.pc = 0;  // Initialize program counter to 0
	i = 0;
	uint8_t instruction;
	uint4_t opr, opa;
	
	while(i < 1000) {
		instruction = i4001[registers.pc];
		opr = (instruction & 0xF0) >> 4;
		opa = (instruction & 0x0F);
		switch (opr) {
			case 0x0: NOP(); registers.pc += 1; break;
			case 0xF:
				switch (opa) {
					case 0x0: break;
					case 0x1: break;
					case 0x2: IAC(); registers.pc += 1; break;
					case 0x3: break;
				} break;
			case 0x4: JUN(); break;
		}
		++i;
	}


	printf("accumulator = %d, pc = %d\n", (int) registers.ac, (int) registers.pc);
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
		case 0: value = (uint4_t) ((registers.r01 & 0xF0) >> 4); break;
		case 1: value = (uint4_t) (registers.r01 & 0x0F); break;
		case 2: value = (uint4_t) ((registers.r23 & 0xF0) >> 4); break;
		case 3: value = (uint4_t) (registers.r23 & 0x0F); break;
		case 4: value = (uint4_t) ((registers.r45 & 0xF0) >> 4); break;
		case 5: value = (uint4_t) (registers.r45 & 0x0F); break;
		case 6: value = (uint4_t) ((registers.r67 & 0xF0) >> 4); break;
		case 7: value = (uint4_t) (registers.r67 & 0x0F); break;
		case 8: value = (uint4_t) ((registers.r89 & 0xF0) >> 4); break;
		case 9: value = (uint4_t) (registers.r89 & 0x0F); break;
		case 10: value = (uint4_t) ((registers.r1011 & 0xF0) >> 4); break;
		case 11: value = (uint4_t) (registers.r1011 & 0x0F); break;
		case 12: value = (uint4_t) ((registers.r1213 & 0xF0) >> 4); break;
		case 13: value = (uint4_t) (registers.r1213 & 0x0F); break;
		case 14: value = (uint4_t) ((registers.r1415 & 0xF0) >> 4); break;
		case 15: value = (uint4_t) (registers.r1415 & 0x0F); break;
	}
	return value;
}

void NOP(void) {
	return;
}

void IAC(void) {
	registers.ac += 1;
	return;
}

void JUN(void) {
	registers.pc = i4001[registers.pc+1];
	return;
}

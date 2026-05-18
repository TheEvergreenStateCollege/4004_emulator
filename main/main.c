#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* TODO
 * Functions for instructions
 * Add remaining registers
*/

typedef unsigned _BitInt(4) uint4_t;
typedef unsigned _BitInt(12) uint12_t;
typedef struct i4004_flags {
	uint8_t chip_select;
	uint8_t cb;
} i4004_flags_t;
typedef struct i4004_registers {
	uint8_t r01, r23, r45, r67, r89, r1011, r1213, r1415;
	uint12_t pc;
	uint12_t s1, s2, s3;
	uint4_t ac;
} i4004_registers_t;

uint8_t stack_level;
i4004_flags_t flags;
i4004_registers_t registers;
uint8_t i4001[256];
uint4_t i4002[20];

//Memory operation function prototypes
void insertInRegister(uint4_t value, uint8_t reg);
uint4_t fetchFromRegister(int reg);

//Instruction function prototypes
void NOP(void);
// Control flow instruction prototypes
void JUN(void);

// Fx instruction prototypes
void CLB(void);
void CLC(void);
void IAC(void);
void CMC(void);
void CMA(void);
void RAL(void);
void RAR(void);
void TCC(void);
void DAC(void);
void TCS(void);
void STC(void);
void DAA(void);
void KBP(void);
void DCL(void);

int main(int argc, char *argv[]) {
	if(argc != 3) {
		printf("Usage is %s <filename> <num> where <filename> is the desired 4004 program and <num> is the number of cycles the emulator should run before halting.\n", argv[0]);
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
	
	while(i < atoi(argv[2])) {
		instruction = i4001[registers.pc];
		opr = (instruction & 0xF0) >> 4;
		opa = (instruction & 0x0F);
		switch (opr) {
			case 0x0: NOP(); registers.pc += 1; break;
			case 0x4: JUN(); break;
			case 0xF:
				switch (opa) {
					case 0x0: CLB(); registers.pc += 1; break;
					case 0x1: CLC(); registers.pc += 1; break;
					case 0x2: IAC(); registers.pc += 1; break;
					case 0x3: CMC(); registers.pc += 1; break;
					case 0x4: CMA(); registers.pc += 1; break;
					case 0x5: RAL(); registers.pc += 1; break;
					case 0x6: RAR(); registers.pc += 1; break;
					case 0x7: TCC(); registers.pc += 1; break;
					case 0x8: DAC(); registers.pc += 1; break;
					case 0x9: TCS(); registers.pc += 1; break;
					case 0xA: STC(); registers.pc += 1; break;
					case 0xB: DAA(); registers.pc += 1; break;
					case 0xC: KBP(); registers.pc += 1; break;
					case 0xD: DCL(); registers.pc += 1; break;
				} break;
		}
		++i;
	}


	printf("accumulator = %d, pc = %d\n", (int) registers.ac, (int) registers.pc);
	return 0;
}

/*
void insertInRegister(uint4_t value, uint8_t reg) {
	uint8_t lr = 0;
	uint8_t ur = 0;
	switch (reg) {
		case 0: lr = (registers.01 & 0x0F); ur = (uint8_t) value; ur = (g << 4); registers.01 = (ur | lr);
		case 1: lr = ((uint8_t) value) & 0x0F; ur = registers.01 & 0xF0; registers.01 = (ur | lr);
		case 2: lr = (registers.23 & 0x0F); ur = (uint8_t) value; ur = (g << 4); registers.23 = (ur | lr);
		case 3: lr = ((uint8_t) value) & 0x0F; ur = registers.23 & 0xF0; registers.23 = (ur | lr);
		case 4: lr = (registers.45 & 0x0F); ur = (uint8_t) value; ur = (g << 4); registers.45 = (ur | lr);
		case 5: lr = ((uint8_t) value) & 0x0F; ur = registers.45 & 0xF0; registers.45 = (ur | lr);
		case 6: lr = (registers.67 & 0x0F); ur = (uint8_t) value; ur = (g << 4); registers.67 = (ur | lr);
		case 7: lr = ((uint8_t) value) & 0x0F; ur = registers.67 & 0xF0; registers.67 = (ur | lr);
		case 8: lr = (registers.89 & 0x0F); ur = (uint8_t) value; ur = (g << 4); registers.89 = (ur | lr);
		case 9: lr = ((uint8_t) value) & 0x0F; ur = registers.89 & 0xF0; registers.89 = (ur | lr);
		case 10: lr = (registers.1011 & 0x0F); ur = (uint8_t) value; ur = (g << 4); registers.1011 = (ur | lr);
		case 11: lr = ((uint8_t) value) & 0x0F; ur = registers.1011 & 0xF0; registers.1011 = (ur | lr);
		case 12: lr = (registers.1213 & 0x0F); ur = (uint8_t) value; ur = (g << 4); registers.1213 = (ur | lr);
		case 13: lr = ((uint8_t) value) & 0x0F; ur = registers.1213 & 0xF0; registers.1213 = (ur | lr);
		case 14: lr = (registers.1415 & 0x0F); ur = (uint8_t) value; ur = (g << 4); registers.1415 = (ur | lr);
		case 15: lr = ((uint8_t) value) & 0x0F; ur = registers.1415 & 0xF0; registers.1415 = (ur | lr);
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

void NOP(void) { return; }
// Control flow instruction definitions
void JUN(void) {
	registers.pc = i4001[registers.pc+1];
	return;
}

// Fx instruction definitions
void CLB(void) {
	flags.cb = 0;
	registers.ac = 0;
	return;
}
void CLC(void) {
	flags.cb = 0;
	return;
}
void IAC(void) {
	registers.ac += 1;
	return;
}
void CMC(void) {
	if (flags.cb == 0) {flags.cb = 1;}
	else {flags.cb = 0;}
	return;
}

void CMA(void) {
	registers.ac = ~(registers.ac);
	return;
}
void RAL(void) { return; }
void RAR(void) { return; }
void TCC(void) {
	registers.ac = (uint4_t) flags.cb;
	flags.cb = 0;
	return;
}
void DAC(void) {
	registers.ac -= 1;
	return;
}
void TCS(void) { return; }
void STC(void) {
	flags.cb = 1;
	return;
}
void DAA(void) { return; }
void KBP(void) { return; }
void DCL(void) { return; }

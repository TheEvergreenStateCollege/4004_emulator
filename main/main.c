#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* TODO
*/

typedef unsigned _BitInt(2) uint2_t;
typedef unsigned _BitInt(4) uint4_t;
typedef unsigned _BitInt(12) uint12_t;
typedef struct i4004_flags {
	uint8_t cb;
	uint8_t test;
	uint8_t select;
} i4004_flags_t;
typedef struct i4004_registers {
	uint8_t r01, r23, r45, r67, r89, r1011, r1213, r1415;
	uint2_t pc;
	uint12_t stack[4];
	uint4_t ac;
} i4004_registers_t;

uint8_t stack_level;
i4004_flags_t flags;
i4004_registers_t registers;
uint8_t i4001[256];
uint4_t i4002[20];

//Memory operation function prototypes
void insertInRegister(uint4_t value, int reg);
uint4_t fetchFromRegister(int reg);
uint8_t fetchDouble(int reg);
void insertDoubel(uint8_t val, int reg);

//Instruction function prototypes
void NOP(void);
void LDM(uint4_t opa);
void LD(uint4_t opa);
void XCH(uint4_t opa);
void ADD(uint4_t opa);
void SUB(uint4_t opa);
void INC(uint4_t opa);

// Control flow instruction prototypes
void JCN(uint4_t condition);
void JIN(uint4_t j_reg);
void JUN(void);
void JMS(uint4_t opa);
void BBL(uint4_t opa);
void ISZ(uint4_t opa);

// ROM and RAM instruction prototypes
void FIM(uint4_t opa);
void FIN(uint4_t opa);
void SRC(uint4_t opa);
void WRM(void);
void WMP(void); // IO
void WRR(void); // IO
void WPM(void); // N/A for setup
void WR0(void);
void WR1(void);
void WR2(void);
void WR3(void);
void SBM(void);
void RDM(void);
void RDR(void); // IO
void ADM(void);
void RD0(void);
void RD1(void);
void RD2(void);
void RD3(void);

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
	
	FILE *mem_fp = fopen("4004.memory", "w+");
	for(j = 0; j < 256; j++) {
		fprintf(mem_fp, "%x ", (int) i4001[j]);
	}
	fclose(mem_fp);
	

	// Start of the Actual Emulator
	registers.pc = 0;  // Initialize program counter to 0
	i = 0;
	uint8_t instruction;
	uint4_t opr, opa;

	
	FILE *reg_fp = fopen("4004.regtrace", "w+");
	while(i < atoi(argv[2])) {
		instruction = i4001[registers.stack[registers.pc]];
		opr = (instruction & 0xF0) >> 4;
		opa = (instruction & 0x0F);
		switch (opr) {
			case 0x0: NOP(); registers.stack[registers.pc] += 1; break;
			case 0x1: JCN(opa); break;
			case 0x2:
				switch ((opa & 0x1)) {
					case 0x0: FIM(opa); registers.stack[registers.pc] += 2; break;
					case 0x1: SRC(opa); registers.stack[registers.pc] += 1; break;
				}
			case 0x3:
				switch ((opa & 0x1)) {
					case 0x0: FIN(opa); registers.stack[registers.pc] += 1; break;
					case 0x1: JIN(opa); break;
				}
			case 0x4: JUN(); break;
			case 0x5: JMS(opa); break;
			case 0x6: INC(opa); registers.stack[registers.pc] += 1; break;
			case 0x7: ISZ(opa); break;
			case 0x8: ADD(opa); registers.stack[registers.pc] += 1; break;
			case 0x9: SUB(opa); registers.stack[registers.pc] += 1; break;
			case 0xA: LD(opa); registers.stack[registers.pc] += 1; break;
			case 0xB: XCH(opa); registers.stack[registers.pc] += 1; break;
			case 0xC: BBL(opa); break;
			case 0xD: LDM(opa); registers.stack[registers.pc] += 1; break;
			case 0xE:
				switch (opa) {
					case 0x0: WRM(); registers.stack[registers.pc] += 1; break;
					case 0x1: WMP(); registers.stack[registers.pc] += 1; break;
					case 0x2: WRR(); registers.stack[registers.pc] += 1; break;
					case 0x3: WPM(); registers.stack[registers.pc] += 1; break;
					case 0x4: WR0(); registers.stack[registers.pc] += 1; break;
					case 0x5: WR1(); registers.stack[registers.pc] += 1; break;
					case 0x6: WR2(); registers.stack[registers.pc] += 1; break;
					case 0x7: WR3(); registers.stack[registers.pc] += 1; break;
					case 0x8: SBM(); registers.stack[registers.pc] += 1; break;
					case 0x9: RDM(); registers.stack[registers.pc] += 1; break;
					case 0xA: RDR(); registers.stack[registers.pc] += 1; break;
					case 0xB: ADM(); registers.stack[registers.pc] += 1; break;
					case 0xC: RD0(); registers.stack[registers.pc] += 1; break;
					case 0xD: RD1(); registers.stack[registers.pc] += 1; break;
					case 0xE: RD2(); registers.stack[registers.pc] += 1; break;
					case 0xF: RD3(); registers.stack[registers.pc] += 1; break;
				}
			case 0xF:
				switch (opa) {
					case 0x0: CLB(); registers.stack[registers.pc] += 1; break;
					case 0x1: CLC(); registers.stack[registers.pc] += 1; break;
					case 0x2: IAC(); registers.stack[registers.pc] += 1; break;
					case 0x3: CMC(); registers.stack[registers.pc] += 1; break;
					case 0x4: CMA(); registers.stack[registers.pc] += 1; break;
					case 0x5: RAL(); registers.stack[registers.pc] += 1; break;
					case 0x6: RAR(); registers.stack[registers.pc] += 1; break;
					case 0x7: TCC(); registers.stack[registers.pc] += 1; break;
					case 0x8: DAC(); registers.stack[registers.pc] += 1; break;
					case 0x9: TCS(); registers.stack[registers.pc] += 1; break;
					case 0xA: STC(); registers.stack[registers.pc] += 1; break;
					case 0xB: DAA(); registers.stack[registers.pc] += 1; break;
					case 0xC: KBP(); registers.stack[registers.pc] += 1; break;
					case 0xD: DCL(); registers.stack[registers.pc] += 1; break;
				} break;
		}
		fprintf(reg_fp, "carry = %d, accumulator = %d, pc = %d\n", (int) flags.cb, (int) registers.ac, (int) registers.stack[registers.pc]);
		++i;
	}


	printf("carry = %d, accumulator = %d, pc = %d\n", (int) flags.cb, (int) registers.ac, (int) registers.stack[registers.pc]);
	return 0;
}


void insertInRegister(uint4_t value, int reg) {
	uint8_t lr = 0;
	uint8_t ur = 0;
	switch (reg) {
		case 0: lr = (registers.r01 & 0x0F); ur = (uint8_t) value; ur = (((uint8_t) value) << 4); registers.r01 = (ur | lr);
		case 1: lr = ((uint8_t) value) & 0x0F; ur = registers.r01 & 0xF0; registers.r01 = (ur | lr);
		case 2: lr = (registers.r23 & 0x0F); ur = (uint8_t) value; ur = (((uint8_t) value) << 4); registers.r23 = (ur | lr);
		case 3: lr = ((uint8_t) value) & 0x0F; ur = registers.r23 & 0xF0; registers.r23 = (ur | lr);
		case 4: lr = (registers.r45 & 0x0F); ur = (uint8_t) value; ur = (((uint8_t) value) << 4); registers.r45 = (ur | lr);
		case 5: lr = ((uint8_t) value) & 0x0F; ur = registers.r45 & 0xF0; registers.r45 = (ur | lr);
		case 6: lr = (registers.r67 & 0x0F); ur = (uint8_t) value; ur = (((uint8_t) value) << 4); registers.r67 = (ur | lr);
		case 7: lr = ((uint8_t) value) & 0x0F; ur = registers.r67 & 0xF0; registers.r67 = (ur | lr);
		case 8: lr = (registers.r89 & 0x0F); ur = (uint8_t) value; ur = (((uint8_t) value) << 4); registers.r89 = (ur | lr);
		case 9: lr = ((uint8_t) value) & 0x0F; ur = registers.r89 & 0xF0; registers.r89 = (ur | lr);
		case 10: lr = (registers.r1011 & 0x0F); ur = (uint8_t) value; ur = (((uint8_t) value) << 4); registers.r1011 = (ur | lr);
		case 11: lr = ((uint8_t) value) & 0x0F; ur = registers.r1011 & 0xF0; registers.r1011 = (ur | lr);
		case 12: lr = (registers.r1213 & 0x0F); ur = (uint8_t) value; ur = (((uint8_t) value) << 4); registers.r1213 = (ur | lr);
		case 13: lr = ((uint8_t) value) & 0x0F; ur = registers.r1213 & 0xF0; registers.r1213 = (ur | lr);
		case 14: lr = (registers.r1415 & 0x0F); ur = (uint8_t) value; ur = (((uint8_t) value) << 4); registers.r1415 = (ur | lr);
		case 15: lr = ((uint8_t) value) & 0x0F; ur = registers.r1415 & 0xF0; registers.r1415 = (ur | lr);
	}

	return;
}

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

uint8_t fetchDouble(int reg) {
	uint8_t ret_val;
	switch (reg) {
		case 0: ret_val = registers.r01; break;
		case 1: ret_val = registers.r23; break;
		case 2: ret_val = registers.r45; break;
		case 3: ret_val = registers.r67; break;
		case 4: ret_val = registers.r89; break;
		case 5: ret_val = registers.r1011; break;
		case 6: ret_val = registers.r1213; break;
		case 7: ret_val = registers.r1415; break;
	}
	return ret_val;
}

void insertDouble(uint8_t val, int reg) {
	switch (reg) {
		case 0: registers.r01 = val; break;
		case 1: registers.r23 = val; break;
		case 2: registers.r45 = val; break;
		case 3: registers.r67 = val; break;
		case 4: registers.r89 = val; break;
		case 5: registers.r1011 = val; break;
		case 6: registers.r1213 = val; break;
		case 7: registers.r1415 = val; break;
	}
	return;
}

// Instruction definitions
void NOP(void) { return; }
void LDM(uint4_t opa) {
	registers.ac = opa;
	return;
}
void LD(uint4_t opa) {
	registers.ac = fetchFromRegister((int) opa);
	return;
}
void XCH(uint4_t opa) {
	uint4_t buf = registers.ac;
	registers.ac = fetchFromRegister((int) opa);
	insertInRegister(buf, (int) opa);
	return;
}
void ADD(uint4_t opa) {
	uint4_t reg_val = fetchFromRegister((int) opa);
	uint4_t test_ac = registers.ac;
	registers.ac = registers.ac + reg_val + (uint4_t) (flags.cb & 0x1);

	int test = (int) reg_val + (int) test_ac + (int) (flags.cb & 0x1);
	if (test > 15) {
		flags.cb = 1;
	} else {
		flags.cb = 0;
	}

	return;
}
void SUB(uint4_t opa) {
	uint4_t reg_val = (fetchFromRegister((int) opa));
	uint4_t test_ac = registers.ac;
	registers.ac = registers.ac - (reg_val + (uint4_t) (flags.cb & 0x1));

	int test = (int) test_ac - ((int) reg_val + (int) (flags.cb & 0x1));
	if (test >= 0) {
		flags.cb = 1;
	} else {
		flags.cb = 0;
	}

	return;
}
void INC(uint4_t opa) {
	uint4_t reg = fetchFromRegister((int) opa);
	reg = reg + 1;
	insertInRegister(reg, (int) opa);
	return;
}


// Control flow instruction definitions
void JCN(uint4_t condition) {
	uint4_t fcn1, fcn2, fcn3, fcn4;
	uint8_t jump = 0;
	fcn4 = condition & 0x1;
	fcn3 = (condition & 0x2) >> 1;
	fcn2 = (condition & 0x4) >> 2;
	fcn1 = (condition & 0x8) >> 3;

	if ( fcn1 == 0 ) {
		if (fcn2 == 1 && registers.ac == 0) {
			jump = 1;
		}
		if (fcn3 == 1 && flags.cb == 1) {
			jump = 1;
		}
		if (fcn4 == 1 && flags.test == 0) {
			jump = 1;
		}
	} else if (fcn1 == 1) {
		if (fcn2 == 1 && registers.ac != 0) {
			jump = 1;
		}
		if (fcn3 == 1 && flags.cb == 0) {
			jump = 1;
		}
		if (fcn4 == 1 && flags.test != 0) {
			jump = 1;
		}
	}

	if (jump) {
		registers.stack[registers.pc] = (uint12_t) i4001[registers.stack[registers.pc]+1];
	} else {
		registers.stack[registers.pc] += 2;
	}

	return;
}
void JIN(uint4_t j_reg) {
	uint12_t new_pc = registers.stack[registers.pc] & 0xF00;
	uint12_t jump_adr = (uint12_t) fetchDouble((int) (j_reg >> 1));
	registers.stack[registers.pc] = new_pc | jump_adr;
	return;
}
void JUN(void) {
	registers.stack[registers.pc] = i4001[registers.stack[registers.pc]+1];
	return;
}
void JMS(uint4_t opa) {
	(void) opa;

	registers.stack[registers.pc+1] = i4001[registers.stack[registers.pc]+1];
	registers.stack[registers.pc] += 2;
	registers.pc += 1;

	return;
}
void BBL(uint4_t opa) {
	registers.pc -= 1;
	registers.ac = opa;
	return;
}
void ISZ(uint4_t opa) {
	uint4_t ireg = fetchFromRegister((int) opa);
	ireg += 1;
	if (ireg == 0) {
		registers.stack[registers.pc] += 2;
	} else if (ireg != 0) {
		registers.stack[registers.pc] = i4001[registers.stack[registers.pc]+1];
	}
	insertInRegister(ireg, (int) opa);

	return;
}

// ROM and RAM instruction definitions
void FIM(uint4_t opa) {
	insertDouble((uint8_t) i4001[registers.stack[registers.pc]+1], (int) (opa >> 1));
	return;
}
void FIN(uint4_t opa) {
	insertDouble((uint8_t) i4001[registers.r01], (int) (opa >> 1));
	return;
}
void SRC(uint4_t opa) {
	flags.select = (fetchDouble((int) opa) & 0x0F);
	return;
}
void WRM(void) {
	i4002[flags.select] = registers.ac;
	return;
}
void WMP(void) { return; } // IO
void WRR(void) { return; } // IO
void WPM(void) { return; } // N/A for setup
void WR0(void) {
	i4002[0+16] = registers.ac;
	return;
}
void WR1(void) {
	i4002[1+16] = registers.ac;
	return;
}
void WR2(void) {
	i4002[2+16] = registers.ac;
	return;
}
void WR3(void) {
	i4002[3+16] = registers.ac;
	return;
}
void SBM(void) { 
	uint4_t sub_val = i4002[flags.select];
	uint4_t test_ac = registers.ac;
	registers.ac = registers.ac - (sub_val + (uint4_t) (flags.cb & 0x1));

	int test = (int) test_ac - ((int) sub_val + (int) (flags.cb & 0x1));
	if (test >= 0) {
		flags.cb = 1;
	} else {
		flags.cb = 0;
	}
	return;
}
void RDM(void) {
	registers.ac = i4002[flags.select];
	return;
}
void RDR(void) { return; } // IO
void ADM(void) {
	uint4_t add_val = i4002[flags.select];
	uint4_t test_ac = registers.ac;
	registers.ac = registers.ac + add_val + (uint4_t) (flags.cb & 0x1);

	int test = (int) add_val + (int) test_ac + (int) (flags.cb & 0x1);
	if (test > 15) {
		flags.cb = 1;
	} else {
		flags.cb = 0;
	}

	return;
}
void RD0(void) {
	registers.ac = i4002[0+16];
	return;
}
void RD1(void) {
	registers.ac = i4002[1+16];
	return;
}
void RD2(void) {
	registers.ac = i4002[2+16];
	return;
}
void RD3(void) {
	registers.ac = i4002[3+16];
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
	if(registers.ac == 0xF) {
		flags.cb = 1;
	}
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
void RAL(void) {
	uint8_t tmp_cb = flags.cb;
	flags.cb = (uint8_t) (registers.ac & 0x8) >> 3;
	uint4_t tmp_ac = registers.ac;
	registers.ac = tmp_ac << 1;
	registers.ac = registers.ac | tmp_cb;
	return;
}
void RAR(void) {
	uint8_t tmp_cb = (uint8_t) (registers.ac & 0x1);
	uint4_t tmp_ac = registers.ac >> 1;
	registers.ac = tmp_ac | (uint4_t) (flags.cb << 3);
	flags.cb = tmp_cb;
	return;
}
void TCC(void) {
	registers.ac = (uint4_t) flags.cb;
	flags.cb = 0;
	return;
}
void DAC(void) {
	if(registers.ac > 0) {
		flags.cb = 1;
	}
	registers.ac -= 1;
	return;
}
void TCS(void) {
	if (flags.cb == 0) {
		registers.ac = 9;
	} else if (flags.cb == 1) {
		registers.ac = 10;
	}
	flags.cb = 0;
	return;
}
void STC(void) {
	flags.cb = 1;
	return;
}
void DAA(void) {
	uint8_t acc = (uint8_t) registers.ac;
	uint8_t car = flags.cb;
	if(car == 1 || acc > 9) {
		acc += 6;
	}
	if(acc > 15) {
		car = 1;
		acc -= 15;
	}
	registers.ac = (uint4_t) acc;
	flags.cb = car;
	return;
}
void KBP(void) {
	uint4_t acc = registers.ac;
	switch (acc) {
		case 0: acc = 0; break;
		case 1: acc = 1; break;
		case 2: acc = 2; break;
		case 4: acc = 3; break;
		case 8: acc = 4; break;
		default: acc = 15; break;
	}
	return;
}
void DCL(void) { return; }

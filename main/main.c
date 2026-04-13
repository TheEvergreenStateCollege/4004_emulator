#include <stdio.h>
#include <stdlib.h>

/* TODO
 * Open file and error check
 * Main loop to progress through program
 * Functions for instructions
 * Add remaining registers
*/

struct registers {
	union {
		struct {
			unsigned char r0;
			unsigned char r1;
		};
		unsigned short r01;
	};
};

void NOP(void);

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage is %s <filename> where <filename> is the desired 4004 program.\n", argv[0]);
		return 1;
	}

	

	// test code, please ignore
	struct registers registers;
	registers.r01 = 0;
	registers.r1 = 6;
	registers.r0 = atoi(argv[1]);
        //open file

	while(registers.r0 > 0) { 
		/*read next line and run it*/ 
		registers.r0 = registers.r0 - 1;
		printf("%d\n", registers.01);
	}
	
	//Free memory and close file
	
	return 0;
}

void NOP(void) {
	return;
}

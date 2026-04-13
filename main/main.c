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
			unsigned char a;
			unsigned char b;
		};
		unsigned short ab;
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
	registers.ab = 0;
	registers.b = 6;
	registers.a = atoi(argv[1]);
        //open file

	while(registers.a > 0) { 
		/*read next line and run it*/ 
		registers.a = registers.a - 1;
		printf("%d\n", registers.ab);
	}
	
	//Free memory and close file
	
	return 0;
}

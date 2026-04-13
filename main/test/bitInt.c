#include <stdio.h>
#include <stdint.h>

typedef unsigned _BitInt(4) gpRegister;
typedef unsigned _BitInt(8) cbRegister;

struct registers {
	union {
		struct {
			gpRegister r0;
			gpRegister r1;
		};
		cbRegister r01;
	};
};

int main(void) {
	struct registers registers;
	registers.r01 = 0;
	//registers.r0 = 1;
	//registers.r1 = 6;
	printf("r01 = %d, r0 = %d, r1 = %d\n", (int)registers.r01, (int)registers.r0, (int)registers.r1);
	return 0;
}

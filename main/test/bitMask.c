#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef unsigned _BitInt(4) uint4_t;

int main(void) {
	uint8_t source = 0xAB;
	uint4_t nibbleHigh = (uint4_t) ((source & 0xF0) >> 4);
	uint4_t nibbleLow = (uint4_t) (source & 0x0F);
	printf("%x, %x, %x\n", (int)source, (int)nibbleHigh, (int)nibbleLow);
	return 0;
}

#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint8_t readBuffer[512];
	uint8_t i4001[256];
	int buffer;
	int i = 0;

	FILE* fp = fopen("rom.txt", "r");

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
	
	int j = 0;
	for(i = 0; i < 512; i += 2) {
		uint8_t buf = (readBuffer[i] << 4) | readBuffer[i+1];
		i4001[j] = buf;
		j++;
	}

	for(j = 0; j < 256; j++) {
		printf("%x ", (int) i4001[j]);
	}

	printf("\n");
	return 0;
}

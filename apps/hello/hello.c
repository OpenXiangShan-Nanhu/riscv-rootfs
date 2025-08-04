#include <stdio.h>

int main() {
	printf("Hello, RISC-V World!\n");
    __asm__ volatile(".word 0x0005006b");
	while(1);
}

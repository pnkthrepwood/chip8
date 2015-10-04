#include "chip8.h"

chip8 cpu;

int main(int argc, char** argv) 
{
	cpu.init();
	cpu.load();

	while(1)
	{
		cpu.cycle();
	}		
}

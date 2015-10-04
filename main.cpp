#include <iostream>

#include "chip8.h"
chip8 cpu;

int main(int argc, char** argv) 
{
	cpu.init();
	cpu.load();

	while(1)
	{
		cpu.cycle();

		if (cpu.drawFlag)
		{
			for (int i = 0; i < SCR_H; ++i)
			{
				for (int j = 0; j < SCR_W; ++j)
				{
					std::cout << (cpu.gfx[i*SCR_H + j])?'*':' ';
				}
				std::cout << std::endl;
			}
		}

	}		
}

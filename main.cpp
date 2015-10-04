#include <iostream>
#include <unistd.h>
#include "chip8.h"
chip8 cpu;

#include <SFML/Graphics.hpp>

int main() 
{

 	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

	cpu.init();
	cpu.load("gaems\\PONG");

	while(1)
	{
		cpu.cycle();


		for (int z = 0; z < 999; ++z) std::cout << std::endl;

		if (cpu.drawFlag)
		{
			for (int i = 0; i < SCR_H; ++i)
			{
				for (int j = 0; j < SCR_W; ++j)
				{
					if(cpu.gfx[i*SCR_W + j] > 0) std::cout << "X";
					else std::cout << ".";
				}
				std::cout << std::endl;
			}
		}

	}		
}

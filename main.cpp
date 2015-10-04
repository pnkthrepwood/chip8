#include <unistd.h>
#include <string.h>
#include "chip8.h"
chip8 cpu;

#include <SFML/Graphics.hpp>

int main(int argc, char** argv) 
{

 	sf::RenderWindow window(sf::VideoMode(SCR_W*10, 
										  SCR_H*10), 
							"Chip8 \t--pnkThrepwood");

	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(10, 10));
	shape.setFillColor(sf::Color::White);

	cpu.init();
	if (argc > 1) 
	{
		if (strcmp(argv[1], "-d") == 0) cpu.dbg_mode = true;
	}
	cpu.load("gaems/PONG");

	while(1)
	{
		cpu.cycle();

		if (cpu.drawFlag)
		{
			window.clear();
			for (int y = 0; y < SCR_H; ++y)
			{
				for (int x = 0; x < SCR_W; ++x)
				{
					int pixel = cpu.gfx[x +y*64];

					pixel?
						shape.setFillColor(sf::Color::White):
						shape.setFillColor(sf::Color::White);

					shape.setPosition(0,0);//x*10, y*10);
					window.draw(shape);
				}
			}
			window.display();
		}
		
		sf::Clock clock;
		sf::Time dt;
		while (clock.getElapsedTime().asSeconds() < 1.0f/60.0f);
	}		
}

#include <unistd.h>
#include <string.h>
#include "chip8.h"
chip8 cpu;

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

void updateInput()
{
	//1 2 3 C
	//4 5 6 D
	//7 8 9 E
	//A 0 B F

	cpu.key[0x1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
	cpu.key[0x2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num2);
	cpu.key[0x3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num3);
	cpu.key[0xC] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num4);

	cpu.key[0x4] = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
	cpu.key[0x5] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	cpu.key[0x6] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
	cpu.key[0xD] = sf::Keyboard::isKeyPressed(sf::Keyboard::R);

	cpu.key[0x7] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	cpu.key[0x8] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	cpu.key[0x9] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	cpu.key[0xE] = sf::Keyboard::isKeyPressed(sf::Keyboard::F);

	cpu.key[0xA] = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	cpu.key[0x0] = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
	cpu.key[0xB] = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
	cpu.key[0xF] = sf::Keyboard::isKeyPressed(sf::Keyboard::V);
}
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
		updateInput();
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
						shape.setFillColor(sf::Color::Green):
						shape.setFillColor(sf::Color::Black);

					shape.setPosition(x*10, y*10);
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

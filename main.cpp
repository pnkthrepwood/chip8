#include <unistd.h>
#include <string.h>
#include "chip8.h"
chip8 cpu;

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <dirent.h>
#include <stdio.h>

using namespace std;

int BIOS(int argc, char** argv)
{

	if (argc < 2) 
	{
		cout <<"Buenax, pica el -h si eres imbecil..."<<endl;
		exit(0);
	}

	if (strcmp(argv[1], "-h") == 0)
	{
		cout << "LAS OPCIONES" << endl;
		cout << "  -h\t\t\tTe muestra esta mierda" << endl;
		cout << "  -lg -losgaems\t\tMira los juegacos"<<endl;
		cout << "  -g  -gaem <juegal>\tCarga el <juegal>"<<endl;
		exit(0);
	}
	
	if (strcmp(argv[1], "-losgaems") == 0 || 
		strcmp(argv[1], "-lg") == 0)
	{
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir ("./gaems")) != NULL) 
		{
			printf("Mira que juegacos:\n\n");

		  /* print all the files and directories within directory */
		  while ((ent = readdir (dir)) != NULL) 
		  {
			if (strcmp(ent->d_name, ".") != 0 &&
				strcmp(ent->d_name, "..") != 0)
		    printf ("\t* %s\n", ent->d_name);
		  }
	  	  closedir (dir);
		
			printf("Pica -gaem <juegal> para viciar.\n");
		  exit(0);
		} 
		else 
		{
		  /* could not open directory */
		  exit(0);
		}
		
	}

	if (strcmp(argv[1], "-gaem") == 0 || 
		strcmp(argv[1], "-g") == 0)
	{
		if (argc < 3) 
		{
			printf("Pero pon el nombre del juego, colega.\n");
			exit(0);
		}

		return 0x70AD;			
	}

	cout << "Pos va a ser siendo que nops." << endl;
	exit(0);

}

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

	int b = BIOS(argc, argv);
	char* rom = argv[2];

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

	char gamepath[6+strlen(argv[2])+1];
	strcpy(gamepath, "gaems/");
	strcat(gamepath, argv[2]);

	cpu.load(gamepath);

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

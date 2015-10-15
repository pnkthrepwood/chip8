#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#include "chip8.h"
#include "fontset.h"

void wrong_opcode(unsigned short op)
{
	printf("Puto opcode esta puto mal: 0x%X\n", op);
	exit(0);
}


void chip8::init()
{
	memset(mem, 0, MEM_SIZE);
	memset(V,   0, REG_SIZE);
	memset(gfx, 0, SCR_SIZE);
	memset(key, 0, KEY_SIZE);

	opcode = 0;
	pc = 0x200;
	sp = 0;	
	I = 0;

	memcpy(mem, chip8_fontset, 80*2);

	drawFlag = true;

	dbg_mode = false;
}

void chip8::load(char* rom_name)
{
	FILE* rom = fopen(rom_name, "rb");
	fseek(rom, 0L, SEEK_END);
	long size = ftell(rom);
	fseek(rom, 0L, SEEK_SET);

	fread(&mem[I+0x200], size, 1, rom);

	fclose(rom);
}

void chip8::cycle()
{
	drawFlag = 0;

	//Read opcode
	opcode = mem[pc] << 8 | mem[pc+1];

	//Debug stuff
#ifdef DEBUGGAH
	printf("> Executing instruction: 0x%X", opcode);
	printf("\tat location 0x%X\n", pc);
	for (int i = 0; i <= 0xF; i++)
	{
		printf("V[%X] %X\t", i, V[i]);
		if (i == 7) printf("\n");
	}
	printf("\n");
	printf("I: %X\n", I);
	printf("pc: %X\n", pc);	
	printf("...\n");
		
	for (int i = -10, j = 0; i <= 10; i +=2, j++)
	{
		unsigned short m = (mem[pc+i] << 8) | mem[pc+i+1];
		if (i == 0) printf("->");
		printf("\t0x%X: %x\t", pc+i, m);


		if (sp == j) printf("*");
		printf("S[%d]: %X\n", j, stack[j]);
	}
	printf("...\n");

	char c;
	if (dbg_mode) std::cin >> c;
#endif
	
	//Las things
	pc += 2;
	if (timer_delay > 0) timer_delay--;
	if (timer_sound > 0) timer_sound--;
	if (timer_sound == 1) printf("<LE SOUND>\n");
	
	//Opcode stuff
	unsigned char x = (opcode&0x0F00) >> 8;
	unsigned char y = (opcode&0x00F0) >> 4;
	switch (opcode & 0xF000)
	{
		case 0x0000:
			if ((opcode | 0x00E0) == 0x00E0) 
			{
				memset(gfx, 0, SCR_SIZE); //CLS
				drawFlag = 1;
			}
			else if ((opcode | 0x00EE) == 0x00EE) 
				pc = stack[--sp]; //RET
			else
				wrong_opcode(opcode);
		break;	

		case 0x1000:
			pc = opcode & 0x0FFF;
		break;

		case 0x2000:
			stack[sp++] = pc;
			pc = opcode & 0x0FFF;
		break;	

		case 0x3000:
			pc += (V[x] == (opcode&0x00FF))? 2:0;			
		break;

		case 0x4000:
			pc += (V[x] == (opcode&0x00FF))? 0:2;				
		break;

		case 0x5000:
			pc += (V[x] == (V[y]))? 2:0;		
		break;	

		case 0x6000:
			V[x] = opcode & 0x00FF;
		break;

		case 0x7000:
			V[x] += opcode & 0x00FF;
		break;

		case 0x8000:
			switch(opcode&0x000F)
			{
				case 0x0: //Vx = Vy
					V[x] = V[y];
				break;
				
				case 0x1:
					V[x] = V[x] | V[y];	
				break;

				case 0x2:
					V[x] = V[x] & V[y];		
				break;
				
				case 0x3:
					V[x] = V[x] ^ V[y];	
				break;

				case 0x4:
					V[0xF] = ((V[x] + V[y]) > 255) ? 1 : 0;
					V[x] += V[y]; 
				break;

				case 0x5:
					V[0xF] = (V[x] > V[y]) ? 1 : 0;
					V[x] -= V[y];
				break;

				case 0x6:
					V[0xF] = V[x] & 1;
					V[x] = V[x] >> 1;
				break;

				case 0x7:
					V[0xF] = (V[y] > V[x]) ? 1 : 0;
					V[y] -= V[x];	
				break;

				case 0xE:
					V[0xF] = V[x] & 0x8;
					V[x] = V[x] << 1;
				break;
			}
		break;

		case 0x9000:
			pc += (V[x] != V[y]) ? 2 : 0;
		break;

		case 0xA000:
			I = opcode&0xFFF;
		break;

		case 0xB000:
			pc = opcode&0xFFF + V[0];
		break;

		case 0xC000:
			V[x] = (rand()%0xFF) & (opcode&0xFF);
		break;

		case 0xD000:
		{
			unsigned short height = opcode&0x000F;
			unsigned short pixel;

			V[0xF] = 0;
			for (unsigned short y_pos = 0; y_pos < height; y_pos++)
			{
				pixel = mem[I+y_pos];
#ifdef DEBUGGAH
				printf("mem[%X]: %x\t",(I+y_pos), mem[I+y_pos]);
				printf("px: 0x%X \n", pixel);
#endif
				for (unsigned short x_pos = 0; x_pos < 8; x_pos++)
				{
					if ((pixel & (0x80 >> x_pos)) != 0)
			      	{
				        if(gfx[(V[x]+x_pos + ((V[y]+y_pos)*64))] == 1)
				          V[0xF] = 1;

				        gfx[V[x]+x_pos + ((V[y]+y_pos)*64)] ^= 1;
			      	}
#ifdef DEBUGGAH
						printf("gfx[%d +%d]: %x\n", 
								V[x]+x_pos,
								(V[y]+y_pos),
								gfx[V[x]+x_pos+((V[y]+y_pos)*64)]);
#endif
				}
#ifdef DEBUGGAH
				printf("\n");
#endif
			}

			drawFlag = 1;
			}
		break;

		case 0xE000:

			switch(opcode&0xFF)
			{
				case 0x9E:
					pc += key[V[x]]?2:0;
				break;

				case 0xA1:
					pc += key[V[x]]?0:2;
				break;

				default:
					wrong_opcode(opcode);
				break;
			}

		break;

		case 0xF000:
			switch(opcode&0x00FF)
			{
				case 0x07:
					V[x] = timer_delay;
				break;

				case 0x0A:
				{
					char c;
					printf("> Reading key...");
					scanf("%c", &c);
				}
				break;	

				case 0x15:
					timer_delay = V[x];
				break;

				case 0x18:
					timer_sound = V[x];
				break;

				case 0x1E:
					I = (I+V[x])&0xFFF;
				break;

				case 0x29:
					I = V[x]*5; //font address
				break;

				case 0x33:
					mem[I] = V[x]/100;
					mem[I+1] = (V[x]/10)%10;
					mem[I+2] = V[x]%10;
				break;

				case 0x55:
					memcpy(&mem[I], V, x);
				break;

				case 0x65:
					memcpy(V, &mem[I], x);
				break;

				default:
					wrong_opcode(opcode);
				break;
			}
		break;

		default:
				wrong_opcode(opcode);
	}


}

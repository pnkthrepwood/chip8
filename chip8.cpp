#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "chip8.h"

void wrong_opcode(unsigned short op)
{
	printf("Puto opcode esta puto mal: 0x%X\n", op);
	exit(0);
}

void chip8::init()
{
	memset(mem, 0, MEM_SIZE);
	memset(V,   0, REG_SIZE);
	memset(gfx, 0, GFX_SIZE);
	memset(key, 0, KEY_SIZE);

	opcode = 0;
	pc = 0x200;
	sp = 0;	
	I = 0;
}

void chip8::load()
{
	//TO-DO
}

void chip8::cycle()
{

	drawFlag = 0;

	//Read opcode
	opcode = mem[pc] << 8 | mem[pc+1];
	pc += 2;

	if (pc >= 0x300) pc = 0x200;

	printf("> Executing instruction: 0x%X\n", opcode);

	unsigned char x = (opcode&0x0F00) >> 8;
	unsigned char y = (opcode&0x00F0) >> 4;

	switch (opcode & 0xF000)
	{

		case 0x0000:
			if ((opcode | 0x00E0) == 0x00E0) 
			{
				memset(gfx, 0, GFX_SIZE); //CLS
				drawFlag = 1;
			}
			else if ((opcode | 0x00EE) == 0x00EE) 
				pc = stack[sp--]; //RET
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
			pc += (V[x] == opcode&0x00FF)? 2:0;			
		break;

		case 0x4000:
			pc += (V[x] == opcode&0x00FF)? 0:2;				
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
					V[0xF] = (V[x] + V[y] > 255) ? 1 : 0;
					V[x] += V[y]; 
				break;

				case 0x5:
					V[0xF] = V[x] > V[y] ? 1 : 0;
					V[x] -= V[y];
				break;

				case 0x6:
					V[0xF] = V[x] & 1;
					V[x] = V[x] >> 1;
				break;

				case 0x7:
					V[0xF] = V[y] > V[x] ? 1 : 0;
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
			//TODO: draw
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
			}

		break;

		case 0xF000:
			switch(opcode&0x00FF)
			{
				case 0x07:
					V[x] = timer_delay;
				break;

				case 0x0A:
					
				break;
			}
		break;

		default:
				wrong_opcode(opcode);
	}


}
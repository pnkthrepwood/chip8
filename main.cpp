#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

//Current opcode
unsigned short opcode;

//Memory & Registers
#define MEM_SIZE 4096
unsigned char mem[MEM_SIZE];
#define REG_SIZE 16
unsigned char V[REG_SIZE];

//Instruction and pc
unsigned short I;
unsigned short pc;

//Graphics mem
#define GFX_H 64
#define GFX_V 32
#define GFX_SIZE GFX_H*GFX_V
unsigned char gfx[GFX_H * GFX_V];

//General timers
unsigned char timer_delay;
unsigned char timer_sound;

//Stack!
unsigned short stack[16];
unsigned short sp;

//Keyboard State
unsigned char keyboard[16];

//Draw flag
unsigned char drawFlag;

void wrong_opcode(unsigned short op)
{
	printf("Puto opcode esta puto mal: 0x%X\n", op);
	exit(0);
}

int main(int argc, char** argv) 
{
	//Init
	memset(mem, 0, MEM_SIZE);
	memset(V,   0, REG_SIZE);
	memset(gfx, 0, GFX_SIZE);

	opcode = 0;
	pc = 0x200;
	sp = 0;	
	I = 0;

	while(1)
	{
		drawFlag = 0;

		//Read opcode
		opcode = mem[pc] << 8 | mem[pc+1];
		pc += 2;

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
				pc += (V[(opcode&0x0F00)>>8] == opcode&0x00FF)? 2:0;			
			break;

			case 0x4000:
				pc += (V[(opcode&0x0F00)>>8] == opcode&0x00FF)? 0:2;				
			break;

			case 0x5000:
				pc += (V[(opcode&0x0F00)>>8] == (V[opcode&0x00F0]>>4))? 2:0;		
			break;	

			case 0x6000:
				V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			break;

			default:
					wrong_opcode(opcode);
		}
		
	}		
}

#pragma once 
#define MEM_SIZE 4096
#define REG_SIZE 16

#define GFX_H 64
#define GFX_V 32
#define GFX_SIZE GFX_H*GFX_V

#define STACK_SIZE 16

#define KEY_SIZE 16

extern unsigned char chip8_fontset[80];

struct chip8
{
	unsigned short opcode; //Current opcode

	unsigned char mem[MEM_SIZE]; //Memory
	unsigned char V[REG_SIZE]; //Regs

	unsigned short I;
	unsigned short pc; 

	unsigned char gfx[GFX_SIZE];

	unsigned char timer_delay;
	unsigned char timer_sound;

	unsigned short stack[STACK_SIZE];
	unsigned short sp;

	unsigned char key[16];

	unsigned char drawFlag;
	void init();
	void load();
	void cycle();
};
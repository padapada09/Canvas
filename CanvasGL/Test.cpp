#include "Canvas.h"
#include <stdio.h>
#define WIDTH 1100
#define HEIGHT 650

int setUp(int &width, int &height)
{
	//Creo la consola
	width = WIDTH;
	height = HEIGHT;
	return 0;
}

int loop(double time)
{
	// fillRect(0,0,HEIGHT,WIDTH,0x990033);
	drawLine(0,0,300,300);
	// if (GetAsyncKeyState(0x28) && 0x8000) player_2.y -= vy * time;
	// if (GetAsyncKeyState(0x26) && 0x8000) player_2.y += vy * time;		
	// if (GetAsyncKeyState(0x53) && 0x8000) player_1.y -= vy * time;
	// if (GetAsyncKeyState(0x57) && 0x8000) player_1.y += vy * time;		
	return 1;
}

#include "Canvas.h"
#include <stdio.h>
#define WIDTH 1500
#define HEIGHT 800

int setUp(int &width, int &height)
{
	//Creo la consola
	width = WIDTH;
	height = HEIGHT;
	return 0;
}

double x = 30, y = 30, vx = 600, vy = 600;

int loop(double time)
{
	fillRect(0,0,HEIGHT,WIDTH,0xffffff);
	fillCircle(x,y,20,0x0000ff);
	if (GetAsyncKeyState(0x28) && 0x8000) if (!(y-20 <= 0)) y -= vy * time;
	if (GetAsyncKeyState(0x27) && 0x8000) if (!(x+20 >= WIDTH)) x += vx * time;
	if (GetAsyncKeyState(0x26) && 0x8000) if (!(y+20 >= HEIGHT)) y += vy * time;
	if (GetAsyncKeyState(0x25) && 0x8000) if (!(x-20 <= 0)) x -= vx * time;		
	return 1;
}

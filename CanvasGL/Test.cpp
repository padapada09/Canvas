#include "Canvas.h"
#include <stdio.h>

class Screen: public Canvas
{
public:

	double x = 50, y = 50, vx = 130, vy = 130;
		
	int loop(double time)
	{
		// fillRect(0,0,this->height,this->width,0xff00ff);
		// drawLine(0,0.85*this->height,this->width,0.85*this->height,0x000000);
		// fillRect(0,0,0.03*this->height,0.02*this->width,0x3333ff);
		// fillCircle(0.01*this->width,0.015*this->height,0.0075*this->height,0xff3300);
		// fillRect(0.02*this->width,0,0.03*this->height,0.02*this->width,0x3333ff);
		// fillCircle(0.03*this->width,0.015*this->height,0.0075*this->height,0xffff00);
		// fillRect(0.04*this->width,0,0.03*this->height,0.02*this->width,0x3333ff);
		// fillCircle(0.05*this->width,0.015*this->height,0.0075*this->height,0xffffff);
		fillCircle(x,y,20,0x000000);
		y += vy * time;
		x += vx * time;
		// if (GetAsyncKeyState(0x28) && 0x8000) y += vy * time;
		// if (GetAsyncKeyState(0x27) && 0x8000) x += vx * time;
		// if (GetAsyncKeyState(0x26) && 0x8000) y -= vy * time;
		// if (GetAsyncKeyState(0x25) && 0x8000) x -= vx * time;		
		if (x + 20 >= this->width && vx > 0) vx *= -1;
		if (x - 20 <= 0 && vx < 0) vx *= -1;
		if (y + (20) >= this->height && vy > 0) vy *= -1;
		if (y - 20 < 0 && vy < 0) vy *= -1;
		return 1;
	}
};

int main()
{
	//Creo la consola
	Screen screen;
	screen.start(800,720);

	return 0;
}

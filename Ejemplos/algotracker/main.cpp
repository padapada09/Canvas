#include "../../Canvas.h"
#include <fstream>
#define WIDTH 700
#define HEIGHT 500

using namespace std;

Canvas* _canvas;
string legajo;
string contrasena;
int menu = 0;
string nombre_equipo,integrantes[3][4];  //[Integrante][Dato]
ifstream usuarios("usuarios.txt");
int intentos=3;

void printLogo(float x, float y, float size, int color_main, int color_back)
{
	fillSemiCircle(x+size/2,y+0,size/2,180,0,color_main);
	fillSemiCircle(x+size/2,y+0,size/3,180,0,color_back);
	fillSemiCircle(x+size/2,y+size,size/2,180,360,color_main);
	fillSemiCircle(x+size/2,y+size,size/3,180,360,color_back);
	fillRect(x+0,y+size/2.65,size,size/4,color_main);
	fillRect(x+size/2.7,y+0,size/4,size,color_main);
}

void perfil()
{
	fillRect(0,0,_canvas->width,_canvas->height,0x333333);
	fillRect(0.2*_canvas->width,0.1*_canvas->height,0.6*_canvas->width,0.8*_canvas->height);
	printLogo(0.2*_canvas->width+10,_canvas->height-(0.2*_canvas->height + 10),0.1*_canvas->height,0x000000,0xffffff);
	write(legajo,_canvas->width*0.3+5,_canvas->height-(0.2*_canvas->height),13,0x000000);
}

void error_login()
{
	fillRect(0,0,_canvas->width,_canvas->height,0xff0000);
	fillRect(0.2*_canvas->width,0.1*_canvas->height,0.6*_canvas->width,0.8*_canvas->height);
	printLogo(0.2*_canvas->width+10,_canvas->height-(0.2*_canvas->height + 10),0.1*_canvas->height,0x000000,0xff0000);
	write(legajo,_canvas->width*0.3+5,_canvas->height-(0.2*_canvas->height),13,0x000000);
}

void login()
{
	fillRect(0,0,_canvas->width,_canvas->height,0x333333);
	fillRect(0.2*_canvas->width,0.1*_canvas->height,0.6*_canvas->width,0.8*_canvas->height);
	printLogo(0.2*_canvas->width+10,_canvas->height-(0.2*_canvas->height + 10),0.1*_canvas->height,0x000000,0xffffff);
	write("ALGO-TRACKER",_canvas->width*0.3+5,_canvas->height-(0.2*_canvas->height),13,0x000000);
	write("LEGAJO:",0.2*_canvas->width+20,_canvas->height-(0.4*_canvas->height),5,0x000000);
	write(legajo,0.3*_canvas->width+20,_canvas->height-(0.4*_canvas->height),5,0x000000);
	write("CONTRASEÑA:",0.2*_canvas->width+20,_canvas->height-(0.6*_canvas->height),5,0x000000);
	std::string blocked_string;
	for (int i = 0; i < contrasena.size(); i++) blocked_string += '.';
	write(blocked_string,0.5*_canvas->width+20,_canvas->height-(0.6*_canvas->height),5,0x000000);
}

void inputContrasena(int vk)
{
	if (vk == VK_BACK && contrasena.length()!=0)
	{
		contrasena.pop_back();
		return;
	}
	if (vk == 13)
	{
		for(int i=0;i<3;i++){
			if(legajo==integrantes[i][0]){
				if(contrasena==integrantes[i][1]){
					menu=2;
				}else{
					menu=1;
					break;
				}
			}else{
				menu=1;
				break;
			}
		}		
	}
	contrasena += (char) vk;
}

void inputLegajo(int vk)
{
	if (vk == VK_BACK && contrasena.length()!=0)
	{
		legajo.pop_back();
		return;
	}
	if (vk == 13)
	{
		_canvas->onKeyDown = inputContrasena;
		return;
	}
	legajo += (char) vk;
}

void click(int x, int y)
{
}

int setUp(Canvas &canvas)
{   
	canvas.width = WIDTH;
	canvas.height = HEIGHT;
	usuarios>>nombre_equipo;
	for(int i=0;i<3;i++)
	{
		usuarios>>integrantes[i][0]>>integrantes[i][1]>>integrantes[i][2]>>integrantes[i][3];
	}
	canvas.onKeyDown = inputLegajo;
	canvas.onLeftClickDown = click;
	_canvas = &canvas;
	return 1;
}

int loop(double time)
{
	switch (menu)
	{
	case 0:
		login();
		break;
	case 1:
		error_login();
		break;
	case 2:
		perfil();
		break;
	}
	return 1;
}

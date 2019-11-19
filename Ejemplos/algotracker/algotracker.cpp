#include "../../Canvas.h"
#include <fstream>
#define WIDTH 700
#define HEIGHT 500

using namespace std;

// char legajo[255];
// char contrasena[255];
int menu = 0;
char nombre_equipo[255],integrantes[3][4][255];  //[Integrante][Dato]
// ifstream usuarios("usuarios.txt");
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
	fillRect(0,0,canvas.width,canvas.height,0x333333);
	fillRect(0.2*canvas.width,0.1*canvas.height,0.6*canvas.width,0.8*canvas.height);
	printLogo(0.2*canvas.width+10,canvas.height-(0.2*canvas.height + 10),0.1*canvas.height,0x000000,0xffffff);
	// write(legajo,canvas.width*0.3+5,canvas.height-(0.2*canvas.height),13,0,0x000000);
}

void error_login()
{
	fillRect(0,0,canvas.width,canvas.height,0xff0000);
	fillRect(0.2*canvas.width,0.1*canvas.height,0.6*canvas.width,0.8*canvas.height);
	printLogo(0.2*canvas.width+10,canvas.height-(0.2*canvas.height + 10),0.1*canvas.height,0x000000,0xff0000);
	// write(legajo,canvas.width*0.3+5,canvas.height-(0.2*canvas.height),13,0,0x000000);
}

void login()
{
	fillRect(0,0,canvas.width,canvas.height,0x333333);
	fillRect(0.2*canvas.width,0.1*canvas.height,0.6*canvas.width,0.8*canvas.height);
	printLogo(0.2*canvas.width+10,canvas.height-(0.2*canvas.height + 10),0.1*canvas.height,0x000000,0xffffff);
	write("ALGO-TRACKER",canvas.width*0.3+5,canvas.height-(0.2*canvas.height),13,0,0x000000);
	write("LEGAJO:",0.2*canvas.width+20,canvas.height-(0.4*canvas.height),5,0,0x000000);
	// write(legajo,0.3*canvas.width+20,canvas.height-(0.4*canvas.height),5,0,0x000000);
	write("CONTRASENA:",0.2*canvas.width+20,canvas.height-(0.6*canvas.height),5,0,0x000000);
	// char* blocked_string;
	// for (int i = 0; i < contrasena.size(); i++) blocked_string += '*';
	// write(blocked_string,0.37*canvas.width+20,canvas.height-(0.6*canvas.height),5,0,0x000000);
}

void inputContrasena(int vk)
{
// 	if (vk == VK_BACK && contrasena.length()!=0)
// 	{
// 		contrasena.pop_back();
// 		return;
// 	}
// 	if (vk == 13)
// 	{
// 		for(int i=0;i<3;i++){
// 			if(legajo==integrantes[i][0]){
// 				if(contrasena==integrantes[i][1]){
// 					menu=2;
// 				}else{
// 					menu=1;
// 					break;
// 				}
// 			}else{
// 				menu=1;
// 				break;
// 			}
// 		}		
// 	}
// 	contrasena += (char) vk;
}

void inputLegajo(int vk)
{
	// if (vk == VK_BACK && contrasena.length()!=0)
	// {
	// 	legajo.pop_back();
	// 	return;
	// }
	// if (vk == 13)
	// {
	// 	canvas.onKeyDown = inputContrasena;
	// 	return;
	// }
	// legajo += (char) vk;
}

void click(int x, int y)
{
}

int setUp()
{   
	canvas.width = WIDTH;
	canvas.height = HEIGHT;
	// usuarios >> nombre_equipo;
	// for(int i=0;i<3;i++)
	// {
	// 	usuarios>>integrantes[i][0]>>integrantes[i][1]>>integrantes[i][2]>>integrantes[i][3];
	// }
	canvas.onKeyDown = inputLegajo;
	canvas.onLeftClickDown = click;
	return 1;
}

int loop()
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
	return 0;
}

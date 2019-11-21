#include "../../CanvasUX.h"
#include <fstream>

struct Persona
{
    char legajo[255];
    char password[255];
}integrantes[3];

char nombre_equipo[255];
char credenciales[255];
std::ifstream usuarios("usuarios.txt");

void printLogo(float x, float y, float size, int color_main, int color_back)
{
    fillSemiCircle(x+size/2,y+0,size/2,180,0,color_main);
    fillSemiCircle(x+size/2,y+0,size/3,180,0,color_back);
    fillSemiCircle(x+size/2,y+size,size/2,180,360,color_main);
    fillSemiCircle(x+size/2,y+size,size/3,180,360,color_back);
    fillRect(x+0,y+size/2.65,size,size/4,color_main);
    fillRect(x+size/2.7,y+0,size/4,size,color_main);
}

void menuLogin();
void menuMain();
void menuActualizarFicha();

#include "menuLogin.h"
#include "menuPrincipal.h"
#include "menuActualizarFicha.h"

int setUp()
{
    canvas.width = 500;
    canvas.height = 500;
    memset(&string_buffer[0][0], '\0', sizeof(string_buffer[0]));
    string_buffer_size[0] = 0;
    active_buffer = 0;
    canvas.onKeyDown = listen;
    canvas.onKeyUp = listen_up;
    showMenu = menuLogin;
    usuarios >> nombre_equipo;
	for(int i=0;i<3;i++)
	{
		usuarios>>integrantes[i].legajo>>integrantes[i].password;
	}    
    return 0;
}

int loop()
{
    showMenu();
    return 0;
}
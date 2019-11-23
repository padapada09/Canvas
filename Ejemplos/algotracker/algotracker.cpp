#include "../../CanvasUX.h"
#include <fstream>
#include <iostream>

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
#include "menuMain.h"
#include "menuActualizarFicha.h"

int setUp()
{
    canvas.width = 500;
    canvas.height = 500;
    ui.clearBuffer(-1);
    ui.active_buffer = 0;
    canvas.onKeyDown = listen;
    canvas.onKeyUp = listenUp;
    showMenu = menuLogin;
    db.load((char*)"db.pyj");
    db.save((char*)"db.pyj");
    db.loadServidor((char*)"problemas.txt");
    return 0;
}

int loop()
{
    showMenu();
    return 0;
}
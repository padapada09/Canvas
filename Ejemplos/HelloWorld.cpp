#include "../Canvas.h" //Replace with yout path lo library

int setUp()
{
    canvas.width = 500;
    canvas.height = 500;
    return 0;
}

int loop()
{
    fillRect(0.15*500,0.15*500,0.7*500,0.7*500,0xffffff); //Imprimimos un cuadrado blanco con sus coordenadas x e y, y su  ancho y alto.
    write("HOLA MUNDO",0.15*500+10,0.15*500+10,10,0,0xff0000); //Imprimimos dentro del cuadrado 'HOLA MUNDO' en Rojo.
    return 0;
}
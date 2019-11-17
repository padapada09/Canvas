#include "../Canvas.h"

int setUp()
{
    canvas.width = 500;
    canvas.height = 500;
    return 1;
}

int loop(float time)
{
    drawLine(0,0,500,500);
    drawLine(500,0,0,500);    
    return 1;
}
#include "../Canvas.h"

struct Boton
{
    std::string text;
    float x;
    float y;
    int size;
    bool selected = false;

    void show()
    {
        if (selected) write(text,x,y,size,0xff0000);
        else write(text,x,y,size,0xffffff); 
    }

}up,down,left,right;

int setUp(int &width, int &height)
{   
    width = 500;
    height = 500;
    up.text = "UP";
    up.x = 200;
    up.y = 400;
    up.size = 6;
    down.text = "DOWN";
    down.x = 150;
    down.y = 10;
    down.size = 6;
    left.text = "left";
    left.x = 5;
    left.y = 200;
    left.size = 6;
    right.text = "right";
    right.x = 300;
    right.y = 200;
    right.size = 6;
    return 1;
}

int loop(double time)
{
    if (isPressed(VK_UP)) up.selected = true;
    else up.selected=false;
    if (isPressed(VK_DOWN)) down.selected = true;
    else down.selected=false;
    if (isPressed(VK_LEFT)) left.selected = true;
    else left.selected=false;
    if (isPressed(VK_RIGHT)) right.selected = true;
    else right.selected=false;
    up.show();
    down.show();
    left.show();
    right.show();
    return 1;
}
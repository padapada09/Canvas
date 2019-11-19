#include "../Canvas.h"
#define HEIGHT 500
#define WIDTH 700

int maximo(int a, int b)
{
    if (a >= b) return a;
    else return b;
}

struct Field
{
    float x = 0.1*WIDTH;
    float y = 0.1*HEIGHT;
    float width = 0.8*WIDTH;
    float height = 0.8*HEIGHT; 
    int color = 0xffffff;

    void draw()
    {
        fillRect(this->x,this->y,this->width,this->height,this->color);
    }

}field;

struct Player
{
    float x = field.x + 10;
    float y = field.y + 10;
    float vy = 0;
    int width = 10;
    int height = 50;
    int color = 0x6699ff;

    void moveY(int direction, float time)
    {
        float aux = this->y + this->vy * direction *time; 
        if (aux + this->height < (field.height+field.y) && aux > field.y) this->y = aux;
    }

    void draw()
    {
        fillRect(this->x,this->y,this->width,this->height,this->color);
    }

}first_player,second_player;

struct Ball
{
    int MAXYSPEED = 350;
    int MINXSPEED = 300;
    int MAXXSPEED = 400;
    float y = field.y + 40 ;
    float x = field.x + (field.width/2);
    float vx = MAXXSPEED/2;
    float vy = MAXYSPEED/2;
    int radius = 10;
    int color = 0x000000;

    void draw()
    {
        fillCircle(x,y,radius,0,color);
    }

    int move(float time)
    {
        float x_aux = this->x + this->vx * time;
        float y_aux = this->y + this->vy * time;
        if (y_aux + this->radius > field.height + field.y || y_aux - radius < field.y) this->vy *= -1;
        if (x_aux + this->radius > field.width + field.x) return 1;
        if (x_aux - radius < field.x) return 2;
        if (this->vx > 0)
        {
            if (y_aux < second_player.y + second_player.height + this->radius 
            && y_aux > second_player.y - this->radius)
            {
                if (this->x + this->radius >= second_player.x)
                {
                    float max_distancia = (this->radius + (second_player.height/2));
                    float distancia = (this->y - (second_player.y + (second_player.height/2)));
                    this->vx = -1 * maximo(MAXXSPEED++ * (1 - (abs(distancia)/max_distancia)),MINXSPEED++);
                    this->vy = 1 * MAXYSPEED++ * (distancia/max_distancia);
                }
            }
        }else{
            if (y_aux < first_player.y + first_player.height + this->radius 
            && y_aux > first_player.y - this->radius)
            {
                if (this->x - this->radius <= first_player.x + first_player.width)
                {
                    float max_distancia = (this->radius + (first_player.height/2));
                    float distancia = (this->y - (first_player.y + (first_player.height/2)));
                    this->vx = 1 * maximo(MAXXSPEED++ * (1 - (abs(distancia)/max_distancia)),MINXSPEED++);
                    this->vy = 1 * MAXYSPEED++ * (distancia/max_distancia);
                }
            }
        }
        this->x = this->x + this->vx * time;
        this->y = this->y + this->vy * time;
        return 0;
    }

}ball;

struct Marcador
{
    int p1 = 0;
    int p2 = 0;
    int color = 0xffffff;
    void show()
    {
        char *player_1_points = new char[255], *player_2_points = new char[255];
        sprintf(player_1_points,"%d",this->p1);
        sprintf(player_2_points,"%d",this->p2);
        write(player_1_points,(0.33)*WIDTH,(0.92)*HEIGHT,16,1,this->color);
        write(player_2_points,(0.66)*WIDTH,(0.92)*HEIGHT,16,1,this->color);
    }
}marcador;

void onKeyDown(int vk)
{
    if (vk == VK_UP) second_player.vy = 250;
    if (vk == VK_DOWN) second_player.vy = -250;
    if (vk == 0x57) first_player.vy = 250;
    if (vk == 0x53) first_player.vy = -250;
}

void onKeyUp(int vk)
{
    if (vk == VK_UP) second_player.vy = 0;
    if (vk == VK_DOWN) second_player.vy = 0;
    if (vk == 0x57) first_player.vy = 0;
    if (vk == 0x53) first_player.vy = 0;
}

void click(int x, int y)
{
    ball.x = x;
    ball.y = canvas.height - y;
}

int setUp()
{
    canvas.width = WIDTH;
    canvas.height = HEIGHT;
    second_player.x = (field.x*2 + field.width) - second_player.x - second_player.width;
    canvas.onKeyDown = onKeyDown;
    canvas.onKeyUp = onKeyUp;
    canvas.onLeftClickDown = click;
    canvas.onRightClickDown = click;
    canvas.onLeftClickUp = click;
    canvas.onRightClickUp = click;
    return 1;
}

int loop()
{
    fillRect(0,0,WIDTH,HEIGHT,0x003399);
    field.draw();
    int lx = -65;
    int ly = 330;
    fillSemiCircle(100+lx,100+ly,30,1,180,0x6699ff);
    fillSemiCircle(100+lx,100+ly,15,0,180,0x003399);
    fillSemiCircle(100+lx,160+ly,30,0,-180,0x6699ff);
    fillSemiCircle(100+lx,160+ly,15,0,-180,0x003399);
    fillRect(93+lx,100+ly,14,60,0x6699ff);
    fillRect(70+lx,123+ly,60,14,0x6699ff);
    write("UTN",70+lx,65+ly,10,1);
    first_player.moveY(1,canvas.time);
    first_player.draw();
    second_player.moveY(1,canvas.time);
    second_player.draw();
    ball.draw();
    marcador.show();
    int movement = ball.move(canvas.time);
    switch (movement)
    {
        case 1:
            marcador.p1++;
            ball.MAXYSPEED = 350;
            ball.MINXSPEED = 300;
            ball.MAXXSPEED = 400;
            ball.x = WIDTH/2;
            ball.y = HEIGHT/2;
            break;
        case 2:
            marcador.p2++;
            ball.MAXYSPEED = 350;
            ball.MINXSPEED = 300;
            ball.MAXXSPEED = 400;
            ball.x = WIDTH/2;
            ball.y = HEIGHT/2;
            break;
    }
    return 0;
}
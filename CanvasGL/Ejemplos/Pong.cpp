#include "../Canvas.h"
#define HEIGHT 400
#define WIDTH 800

int maximo(int a, int b)
{
    if (a >= b) return a;
    else return b;
}

struct Player
{
    float x = 10;
    float y = 10;
    float vy = 250;
    int width = 10;
    int height = 50;
    int color = 0xffffff;

    void moveY(int direction, float time)
    {
        float aux = this->y + this->vy * direction *time; 
        if (aux + this->height < HEIGHT && aux > 0) this->y = aux;
    }

    void draw()
    {
        fillRect(this->x,this->y,this->width,this->height,this->color);
    }

}first_player,second_player;

struct Ball
{
    int MAXYSPEED = 250;
    int MINXSPEED = 200;
    int MAXXSPEED = 300;
    float y = 40 ;
    float x = WIDTH/2;
    float vx = MAXXSPEED/2;
    float vy = MAXYSPEED/2;
    int radius = 10;
    int color = 0xffffff;

    void draw()
    {
        fillCircle(x,y,radius,color);
    }

    int move(float time)
    {
        float x_aux = this->x + this->vx * time;
        float y_aux = this->y + this->vy * time;
        if (y_aux + this->radius > HEIGHT || y_aux - radius < 0) this->vy *= -1;
        if (x_aux + this->radius > WIDTH) return 1;
        if (x_aux - radius < 0) return 2;
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

int setUp(int &width, int &height)
{
    width = WIDTH;
    height = HEIGHT;
    second_player.x = WIDTH - second_player.x - second_player.width;
    return 1;
}

int loop(double time)
{
    first_player.draw();
    second_player.draw();
    ball.draw();
    if (isPressed(VK_UP)) second_player.moveY(1,time);
    if (isPressed(VK_DOWN)) second_player.moveY(-1,time);
    if (isPressed(0x57)) first_player.moveY(1,time);
    if (isPressed(0x53)) first_player.moveY(-1,time);
    int movement = ball.move(time);
    switch (movement)
    {
        case 1:
            std::cout << "PLAYER 1 WINS!";
            return 0;
            break;
        case 2:
            std::cout << "PLAYER 2 WINS!";
            return 0;
            break;
    }
    return 1;
}

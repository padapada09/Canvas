#include "Canvas.h"

char string_buffer[10][500];
int string_buffer_size[10] = {0,0,0,0,0,0,0,0,0,0};
int active_buffer = 0;


void onClickIgnore(){}

void listen(int vk)
{
    if (vk == VK_SHIFT) canvas.on_shift = true;
    else if(vk == VK_TAB) for (int i = 0; i < 3; i++) string_buffer[active_buffer][string_buffer_size[active_buffer]++] = ' ';
    else if(vk == VK_BACK) string_buffer[active_buffer][--string_buffer_size[active_buffer]] = '\000';
    else if (canvas.on_shift && vk >= 65 && vk <= 90) string_buffer[active_buffer][string_buffer_size[active_buffer]++] = (char)vk;
    else if (vk >= 65 && vk <= 90) string_buffer[active_buffer][string_buffer_size[active_buffer]++] = (char) (vk+32);
    else if (vk >= 0x30 && vk <= 0x39) string_buffer[active_buffer][string_buffer_size[active_buffer]++] = (char) vk;
    else if (vk == VK_SPACE) string_buffer[active_buffer][string_buffer_size[active_buffer]++] = (char) vk;
    else if (vk == 13) string_buffer[active_buffer][string_buffer_size[active_buffer]] = 13;
}

void listen_up(int vk)
{
    if (vk == VK_SHIFT) canvas.on_shift = false;
}

struct Button
{
    float x;
    float y;
    float width;
    float height;
    char * text;
    int color = 0x005555;
    int main_color = 0x005555;
    int hover_color = 0x007777;
    int click_color = 0x00aaaa;
    void (*onClick)() = onClickIgnore;

    Button(char * text,float x, float y, float size, int color = 0x005555)
    {
        this->text = text;
        this->x = x;
        this->y = y;
        this->height = size;
        this->color = color;
    }

    void draw()
    {
        int size = strlen(text);
        width = size * height * 0.46;
        fillRect(this->x,this->y,this->width,this->height,this->color);
        write(text,x+(width*0.07),y+(height*0.3),height*0.2,0,this->click_color);
    }

    void onMouseMove(int x, int y)
    {
        if (x >= this->x && x <= this->x+this->width && y >= this->y && y <= this->y+this->height)
        {
            SetCursor(canvas.selection_cursor);
            this->color = this->hover_color;
        }
        else
        {
            this->color = this->main_color;   
        }
    }

    void onMouseClick(int x, int y)
    {
        if (x >= this->x && x <= this->x+this->width && y >= this->y && y <= this->y+this->height)
        {
            this->color = this->click_color;
            this->onClick();
        }        
    }

};
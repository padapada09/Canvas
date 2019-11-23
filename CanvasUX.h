#include "Canvas.h"
#include <fstream>

void onClickIgnore();
void listen(int vk);
void listenUp(int vk);
void writeAs(char* text,float x,float y,int size,char c);

struct Problema
{
    char id[10];
    char name[40];
};

struct Persona
{
    char legajo[255];
    char password[255];
    char nombre[255];
    char apellido[255];
    Problema solved [50];
    int solved_size = 0;
    char notas[2][10];
};

struct DB
{
    std::fstream db_f;
    char nombre_equipo[255];
    Persona personas[3];
    Problema problemas[50];

    void loadServidor(char* server_data_file)
    {
        db_f.open(server_data_file);
        char aux[255];
        char c;
        int i = 0;
        while(db_f >> aux)
        {
            strcpy(this->problemas[i].id,aux);
            db_f.getline(this->problemas[i].name,sizeof(this->problemas[i].name));
            i++;
        }
        db_f.close();
    }

    void load(char* data_base_file)
    {
        this->db_f.open(data_base_file);
        char aux[255];
        db_f >> aux;
        while(strcmp(aux,"<titulo>")) db_f >> aux;
        db_f >> this->nombre_equipo;
        while(strcmp(aux,"<credenciales>")) db_f >> aux;
        for (int i = 0; i < 3; i++) db_f >> personas[i].legajo >> personas[i].password >> personas[i].nombre >> personas[i].apellido;
        /*USER 1*/
        while(strcmp(aux,"<usuariouno>")) db_f >> aux;
        while(strcmp(aux,"<resueltos>")) db_f >> aux;
        db_f >> aux;
        while(strcmp(aux,"<parciales>"))
        {
            strcpy(this->personas[0].solved[this->personas[0].solved_size++].id,aux);
            db_f >> aux;
        }
        while(strcmp(aux,"<parcialuno>")) db_f >> aux;
        db_f >> aux;
        while(strcmp(aux,"<parcialdos>"))
        {
            strcpy(personas[0].notas[0],aux);
            db_f >> aux;
            break;
        }
        while(strcmp(aux,"<parcialdos>")) db_f >> aux;
        db_f >> aux;
        while(strcmp(aux,"<usuariodos>"))
        {
            strcpy(personas[0].notas[1],aux);
            db_f >> aux;
            break;
        }
        /*USER 2*/
        while(strcmp(aux,"<usuariodos>")) db_f >> aux;
        while(strcmp(aux,"<resueltos>")) db_f >> aux;
        db_f >> aux;
        while(strcmp(aux,"<parciales>"))
        {
            strcpy(personas[1].solved[this->personas[1].solved_size++].id,aux);
            db_f >> aux;
        }
        while(strcmp(aux,"<parcialuno>")) db_f >> aux;
        db_f >> aux;
        while(strcmp(aux,"<parcialdos>"))
        {
            strcpy(personas[1].notas[0],aux);
            db_f >> aux;
            break;
        }
        while(strcmp(aux,"<parcialdos>")) db_f >> aux;
        db_f >> aux;
        while(strcmp(aux,"<usuariodos>"))
        {
            strcpy(personas[1].notas[1],aux);
            db_f >> aux;
            break;
        }
        /*USER 3*/
        while(strcmp(aux,"<usuariotres>")) db_f >> aux;
        while(strcmp(aux,"<resueltos>")) db_f >> aux;
        db_f >> aux;
        while(strcmp(aux,"<parciales>"))
        {
            strcpy(personas[2].solved[this->personas[2].solved_size++].id,aux);
            db_f >> aux;
        }
        while(strcmp(aux,"<parcialuno>")) db_f >> aux;
        db_f >> aux;
        while(strcmp(aux,"<parcialdos>"))
        {
            strcpy(personas[2].notas[0],aux);
            db_f >> aux;
            break;
        }
        while(strcmp(aux,"<parcialdos>")) db_f >> aux;
        db_f >> aux;
        while(strcmp(aux,"<usuariodos>"))
        {
            strcpy(personas[2].notas[1],aux);
            db_f >> aux;
            break;
        }
        db_f.close();
    }

    void save(char* data_base_file)
    {
        this->db_f.open(data_base_file,std::fstream::out);
        char aux[255];
        db_f << "<titulo>" << "\n";
        db_f << this->nombre_equipo << "\n";
        db_f << "<credenciales>" << "\n";
        for (int i = 0; i < 3; i++) db_f << personas[i].legajo << " " << personas[i].password << " " << personas[i].nombre << " " << personas[i].apellido << "\n";
        /*USER 1*/
        db_f << "<usuariouno>" << "\n";
        db_f << "<resueltos>" << "\n";
        for (int i = 0; i < this->personas[0].solved_size; i++)
        {
            db_f << this->personas[0].solved[i].id << "\n";
        }
        db_f << "<parciales>" << "\n";
        db_f << "<parcialuno>" << "\n";
        db_f << this->personas[0].notas[0] << "\n";
        db_f << "<parcialdos>" << "\n";
        db_f << this->personas[0].notas[1] << "\n";        
        /*USER 2*/
        db_f << "<usuariodos>" << "\n";
        db_f << "<resueltos>" << "\n";
        for (int i = 0; i < this->personas[1].solved_size; i++) db_f << this->personas[1].solved[i].id << "\n";
        db_f << "<parciales>" << "\n";
        db_f << "<parcialuno>" << "\n";
        db_f << this->personas[1].notas[0] << "\n";
        db_f << "<parcialdos>" << "\n";
        db_f << this->personas[1].notas[1] << "\n";
        /*USER 3*/
        db_f << "<usuariotres>" << "\n";
        db_f << "<resueltos>" << "\n";
        for (int i = 0; i < this->personas[2].solved_size; i++) db_f << this->personas[1].solved[i].id << "\n";
        db_f << "<parciales>" << "\n";
        db_f << "<parcialuno>" << "\n";
        db_f << this->personas[2].notas[0] << "\n";
        db_f << "<parcialdos>" << "\n";
        db_f << this->personas[2].notas[1] << "\n";
        db_f.close();
    }

    void print()
    {
        for (int i = 0; i < 3; i++)
        {
            printf("Usuario %s %s %s\n",this->personas[i].legajo,this->personas[i].nombre,this->personas[i].apellido);
            printf("\tProblemas resuelto: %d\n",this->personas[i].solved_size);
            printf("\tProblemas resuelto:\n");
            for (int j = 0; j < this->personas[i].solved_size; j++) printf("\t\tproblema: %s\n",this->personas[i].solved[j].id);
        }
        // printf("Servidor:\n");
        // for (int i = 0; i < 40; i++) printf("id: %s <-> name: %s\n",this->problemas[i].id,this->problemas[i].name);
    }

}db;

struct Sesion
{
    int index;
    char legajo[255];
    int mistakes = 0;
}sesion;

struct UI
{
    char string_buffer[10][500];
    int string_buffer_size[10] = {0,0,0,0,0,0,0,0,0,0};
    int active_buffer = 0;

    void clearBuffer(int buffer)
    {
        if (buffer < 0)
        {
            for (int i = 0; i < 10; i++)
            {
                memset(&string_buffer[i][0], '\0', sizeof(string_buffer[i]));
                string_buffer_size[i] = 0;
            }
        }
        else
        {
            memset(&string_buffer[buffer][0], '\0', sizeof(string_buffer[buffer]));
            string_buffer_size[buffer] = 0;
        }
    }
}ui;

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

void onClickIgnore(){}

void listen(int vk)
{
    if (vk == VK_SHIFT) canvas.on_shift = true;
    else if(vk == VK_TAB) for (int i = 0; i < 3; i++) ui.string_buffer[ui.active_buffer][ui.string_buffer_size[ui.active_buffer]++] = ' ';
    else if(vk == VK_BACK) ui.string_buffer[ui.active_buffer][--ui.string_buffer_size[ui.active_buffer]] = '\000';
    else if (canvas.on_shift && vk >= 65 && vk <= 90) ui.string_buffer[ui.active_buffer][ui.string_buffer_size[ui.active_buffer]++] = (char)vk;
    else if (vk >= 65 && vk <= 90) ui.string_buffer[ui.active_buffer][ui.string_buffer_size[ui.active_buffer]++] = (char) (vk+32);
    else if (vk >= 0x30 && vk <= 0x39) ui.string_buffer[ui.active_buffer][ui.string_buffer_size[ui.active_buffer]++] = (char) vk;
    else if (vk == VK_SPACE) ui.string_buffer[ui.active_buffer][ui.string_buffer_size[ui.active_buffer]++] = (char) vk;
    else if (vk == 13) ui.string_buffer[ui.active_buffer][ui.string_buffer_size[ui.active_buffer]] = 13;
}

void listenUp(int vk)
{
    if (vk == VK_SHIFT) canvas.on_shift = false;
}

void writeAs(char* text,float x,float y,int size,char c)
{
    char coded[255] = "";
    int text_size = 0;
    while(text[text_size] != '\0' && text[text_size] != 13) text_size++;
    for (int i = 0; i < text_size; i++) coded[i] = c;
    write(coded,190,500-300,8);
}

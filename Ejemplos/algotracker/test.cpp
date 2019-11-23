#include <fstream>
#include <iostream>
#include <cstring>
#include <stdio.h>

struct Persona
{
    char legajo[255];
    char password[255];
    char nombre[255];
    char apellido[255];
    char solved [50][10];
    int solved_size = 0;
    char notas[2][10];
};

struct Problema
{
    char id[10];
    char name[40];
    char tema;
};

struct DB
{
    std::fstream db_f;
    char nombre_equipo[255];
    Persona personas[3];
    Problema problemas[50];

    DB()
    {
        this->db_f.open("db.pyj");
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
            strcpy(this->personas[0].solved[this->personas[0].solved_size++],aux);
            db_f >> aux;
        }
        while(strcmp(aux,"<parcialuno>")) db_f >> aux;
        db_f >> aux;
        strcpy(personas[0].notas[0],aux);
        while(strcmp(aux,"<parcialdos>")) db_f >> aux;
        db_f >> aux;
        strcpy(this->personas[0].notas[1],aux);
        /*USER 2*/
        while(strcmp(aux,"<usuariodos>")) db_f >> aux;
        while(strcmp(aux,"<resueltos>")) db_f >> aux;
        while(strcmp(aux,"<parciales>"))
        {
            strcpy(this->personas[1].solved[this->personas[1].solved_size++],aux);
            db_f >> aux;
        }
        while(strcmp(aux,"<parcialuno>")) db_f >> aux;
        db_f >> aux;
        strcpy(personas[1].notas[0],aux);
        while(strcmp(aux,"<parcialdos>")) db_f >> aux;
        db_f >> aux;
        strcpy(personas[1].notas[1],aux);
        /*USER 3*/
        while(strcmp(aux,"<usuariotres>")) db_f >> aux;
        while(strcmp(aux,"<resueltos>")) db_f >> aux;
        while(strcmp(aux,"<parciales>"))
        {
            strcpy(personas[2].solved[this->personas[2].solved_size++],aux);
            db_f >> aux;
        }
        while(strcmp(aux,"<parcialuno>")) db_f >> aux;
        db_f >> aux;
        strcpy(this->personas[2].notas[0],aux);
        while(strcmp(aux,"<parcialdos>")) db_f >> aux;
        db_f >> aux;
        strcpy(this->personas[2].notas[1],aux);
        db_f.close();
        this->db_f.open("problemas.txt");
        int i=0;
        /*
        while (db_f << problemas[i].id)
        {
            db_f.getline(this->problemas[i].name,sizeof(this->problemas[i].name));
            printf(this->problemas[i++].name);
        }
        */
        for (int i = 0; i < 46; i++)
        {
            db_f<<this->problemas[i].id;
            db_f.getline(this->problemas[i].name,40);
        }
        
        this->db_f.close();
    }

    void save()
    {
        this->db_f.open("db.pyj",std::ofstream::trunc);
        char aux[255];
        db_f << "<titulo>" << "\n";
        db_f << this->nombre_equipo << "\n";
        db_f << "<credenciales>" << "\n";
        for (int i = 0; i < 3; i++) db_f << personas[i].legajo << " " << personas[i].password << " " << personas[i].nombre << " " << personas[i].apellido << "\n";
        /*USER 1*/
        db_f << "<usuario-uno>" << "\n";
        db_f << "<resueltos>" << "\n";
        for (int i = 0; i < this->personas[0].solved_size; i++) db_f << this->personas[0].solved[i] << "\n";
        db_f << "<parciales>" << "\n";
        db_f << "<parcial-uno>" << "\n";
        db_f << this->personas[0].notas[0] << "\n";
        db_f << "<parcial-dos>" << "\n";
        db_f << this->personas[0].notas[1] << "\n";        
        /*USER 2*/
        db_f << "<usuario-dos>" << "\n";
        db_f << "<resueltos>" << "\n";
        for (int i = 0; i < this->personas[1].solved_size; i++) db_f << this->personas[1].solved[i] << "\n";
        db_f << "<parciales>" << "\n";
        db_f << "<parcial-uno>" << "\n";
        db_f << this->personas[1].notas[0] << "\n";
        db_f << "<parcial-dos>" << "\n";
        db_f << this->personas[1].notas[1] << "\n";
        /*USER 3*/
        db_f << "<usuario-tres>" << "\n";
        db_f << "<resueltos>" << "\n";
        for (int i = 0; i < this->personas[2].solved_size; i++) db_f << this->personas[1].solved[i] << "\n";
        db_f << "<parciales>" << "\n";
        db_f << "<parcial-uno>" << "\n";
        db_f << this->personas[2].notas[0] << "\n";
        db_f << "<parcial-dos>" << "\n";
        db_f << this->personas[2].notas[1] << "\n";
        db_f.close();
    }

}db;

using namespace std;

int main()
{
    cout<<db.problemas[20].id<<" - "<<db.problemas[20].name;
    db.save();
    return 0;
}
Button boton_ir_registrar_problema((char*)"Registrar un problema",10,90,30);
Button boton_ir_registrar_calificacion((char*)"Registrar calificacion",10,50,30);
Button boton_volver_a_main((char*)"Volver",10,10,30);
Button boton_volver_a_actualizar_ficha((char*)"Volver",10,50,30);

void menuRegistrarProblema();
void menuRegistrarCalificacion();

void volverActualizarFicha()
{
    showMenu = menuActualizarFicha;
    db.save((char*)"db.pyj");
}

#include "menuRegistrarProblema.h"
#include "menuRegistrarCalificacion.h"

void onMouseMoveActualizarFicha(int x, int y)
{
    boton_ir_registrar_problema.onMouseMove(x,y);    
    boton_ir_registrar_calificacion.onMouseMove(x,y);    
    boton_volver_a_main.onMouseMove(x,y);    
}

void onMouseClickActualizarFicha(int x, int y)
{
    boton_ir_registrar_problema.onMouseClick(x,y);
    boton_ir_registrar_calificacion.onMouseClick(x,y);  
    boton_volver_a_main.onMouseClick(x,y);    
}

void volverMain()
{
    showMenu = menuMain;
}

void irMenuRegistrarProblema()
{
    showMenu = menuRegistrarProblema;
    ui.active_buffer = 0;
    ui.clearBuffer(0);
}

void irMenuRegistrarCalificacion()
{
    showMenu = menuRegistrarCalificacion;
    ui.active_buffer = 0;
    ui.clearBuffer(0);
}

void menuActualizarFicha()
{
    fillRect(0,0,canvas.height,canvas.width,0x333333);
    write((char*)"Actualizar Ficha",10,500-30,10);
    printLogo(400,10,90,0xffffff,0x333333);
    canvas.onMouseMove = onMouseMoveActualizarFicha;
    canvas.onLeftClickDown = onMouseClickActualizarFicha;
    boton_ir_registrar_problema.onClick = irMenuRegistrarProblema;
    boton_ir_registrar_calificacion.onClick = irMenuRegistrarCalificacion;
    boton_ir_registrar_problema.draw();
    boton_ir_registrar_calificacion.draw();
    boton_volver_a_main.draw();
    boton_volver_a_main.onClick = volverMain;
}

// void menuRegistrarCalificacion()
// {
//     fillRect(0,0,canvas.height,canvas.width,0x333333);
//     write((char*)"Registrar Calificacion",10,500-30,10);
//     printLogo(400,10,90,0xffffff,0x333333);
//     write((char*)"Nota Parc 1",10,500-200,10);
//     write(string_buffer[0],250,500-200,10);
//     if (string_buffer[0][string_buffer_size[0]] == 13)
//     {
//         string_buffer[0][string_buffer_size[0]] = '\0';
//         for(int i=0;i<46;i++)
//         {            
//             if(!strcmp(string_buffer[0],problemas[i].id))
//             {                
//                 strcpy(string_buffer[1],problemas[i].name);
//                 strcat(string_buffer[1],(char*)" resuelto");
//                 results << credenciales << " " << string_buffer[0] <<  "\n";
//             }
//         }
//         memset(&string_buffer[0][0], '\0', sizeof(string_buffer[0]));
//         string_buffer_size[0] = 0;
//     }
//     boton_salir.onClick = volverActualizarFicha;
//     boton_salir.draw();
// }
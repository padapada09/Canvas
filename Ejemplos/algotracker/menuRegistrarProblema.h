Button boton_registrar_problema((char*)"Registrar",100,50,30);

void onMouseMoveRegistrarProblema(int x, int y)
{
    boton_volver_a_actualizar_ficha.onMouseMove(x,y);    
    boton_registrar_problema.onMouseMove(x,y);     
}

void onMouseClickRegistrarProblema(int x, int y)
{
    boton_volver_a_actualizar_ficha.onMouseClick(x,y);
    boton_registrar_problema.onMouseClick(x,y);
}

void registrarProblema()
{
    char * problema = ui.string_buffer[0];
    int i;
    for (i = 0; i < 50; i++) if (!strcmp(problema,db.personas[sesion.index].solved[i].id))
    {
        strcpy(ui.string_buffer[1],"Ya lo registraste");
        ui.clearBuffer(0);
        return;
    }
    for (i = 0; i < 50; i++) if (!strcmp(problema,db.problemas[i].id))
    {   
        strcpy(ui.string_buffer[1],db.problemas[i].name);
        strcat(ui.string_buffer[1],(char*)" resuelto");
        strcpy(db.personas[sesion.index].solved[db.personas[sesion.index].solved_size++].id,db.problemas[i].id);
        strcpy(db.personas[sesion.index].solved[db.personas[sesion.index].solved_size].name,db.problemas[i].name);
        ui.clearBuffer(0);
        return;
    }
    strcpy(ui.string_buffer[1],"No existe");
    ui.clearBuffer(0);
}

void menuRegistrarProblema()
{
    //Background
    fillRect(0,0,canvas.width,canvas.height,0x333333);
    
    //Titulo y logo
    write((char*)"Registrar Problemas",10,500-30,10);
    printLogo(400,10,90,0xffffff,0x333333);
    
    //Input
    write((char*)"ID Problema",10,500-200,10);
    //Id problema
    write(ui.string_buffer[0],250,500-200,10);

    //Output del nombre del problema
    write(ui.string_buffer[1],10,500-300,5);
    if (ui.string_buffer[0][ui.string_buffer_size[0]] == 13)
    {
        ui.string_buffer[0][ui.string_buffer_size[0]] = '\0';
        registrarProblema();
    }
    canvas.onMouseMove = onMouseMoveRegistrarProblema;
    canvas.onLeftClickDown = onMouseClickRegistrarProblema;
    boton_volver_a_actualizar_ficha.onClick = volverActualizarFicha;
    boton_volver_a_actualizar_ficha.draw();
    boton_registrar_problema.onClick = registrarProblema;
    boton_registrar_problema.draw();
}
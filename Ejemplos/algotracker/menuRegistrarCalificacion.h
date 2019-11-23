Button boton_registrar_calificacion((char*)"Registrar",100,50,30);

void onMouseMoveRegistrarCalificacion(int x, int y)
{
    boton_volver_a_actualizar_ficha.onMouseMove(x,y);    
    boton_registrar_calificacion.onMouseMove(x,y);     
}

void onMouseClickRegistrarCalificacion(int x, int y)
{
    boton_volver_a_actualizar_ficha.onMouseClick(x,y);
    boton_registrar_calificacion.onMouseClick(x,y);
}

void registrarCalificacion()
{
    int calificacion = atoi(ui.string_buffer[0]);
    if (calificacion > 100 || calificacion < 0)
    {
        strcpy(ui.string_buffer[1],"Valor invalido");
    }else{
        strcpy(db.personas[sesion.index].notas[0],ui.string_buffer[0]);
        strcpy(ui.string_buffer[1],"Nota cargada");
    }
}

void menuRegistrarCalificacion()
{
    //Background
    fillRect(0,0,canvas.width,canvas.height,0x333333);
    
    //Titulo y logo
    write((char*)"Registrar Calificaciones",10,500-30,10);
    printLogo(400,10,90,0xffffff,0x333333);
    
    //Input
    write((char*)"Tu nota",10,500-200,10);
    //Id Calificacion
    write(ui.string_buffer[0],250,500-200,10);

    //Output del nombre del Calificacion
    write(ui.string_buffer[1],10,500-300,5);
    if (ui.string_buffer[0][ui.string_buffer_size[0]] == 13)
    {
        ui.string_buffer[0][ui.string_buffer_size[0]] = '\0';
        registrarCalificacion();
    }
    canvas.onMouseMove = onMouseMoveRegistrarCalificacion;
    canvas.onLeftClickDown = onMouseClickRegistrarCalificacion;
    boton_volver_a_actualizar_ficha.onClick = volverActualizarFicha;
    boton_volver_a_actualizar_ficha.draw();
    boton_registrar_calificacion.onClick = registrarCalificacion;
    boton_registrar_calificacion.draw();
}
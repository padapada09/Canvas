Button boton_registrar_problema((char*)"Registrar problema",10,90,30);
Button boton_registrar_calificacion((char*)"Registrar calificacion",10,50,30);

void onMouseMove3(int x, int y)
{
    boton_registrar_problema.onMouseMove(x,y);    
    boton_registrar_calificacion.onMouseMove(x,y);    
    boton_salir.onMouseMove(x,y);    
}

void onMouseClick3(int x, int y)
{
    boton_registrar_problema.onMouseClick(x,y);
    boton_registrar_calificacion.onMouseClick(x,y);  
    boton_salir.onMouseClick(x,y);    
}

void salirDeActualizarFicha()
{
    showMenu = menuMain;
}

void menuActualizarFicha()
{
    fillRect(0,0,canvas.height,canvas.width,0x333333);
    write((char*)"Actualizar Ficha",10,500-30,10);
    printLogo(400,10,90,0xffffff,0x333333);
    canvas.onMouseMove = onMouseMove3;
    canvas.onLeftClickDown = onMouseClick3;
    boton_salir.onClick = salirDeActualizarFicha;
    boton_registrar_problema.draw();
    boton_registrar_calificacion.draw();
    boton_salir.draw();
}
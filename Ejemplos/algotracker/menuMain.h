Button boton_ir_actualizar((char*)"Actualizar ficha",10,90,30);
Button boton_informe((char*)"Informes de desempeno",10,50,30);
Button boton_salir((char*)"Salir",10,10,30);

void onMouseMoveMain(int x, int y)
{
    boton_ir_actualizar.onMouseMove(x,y);
    boton_informe.onMouseMove(x,y);   
    boton_salir.onMouseMove(x,y);    
}

void onMouseClickMain(int x, int y)
{
    boton_ir_actualizar.onMouseClick(x,y);
    boton_informe.onMouseClick(x,y);    
    boton_salir.onMouseClick(x,y);    
}

void salir()
{    
    db.save((char*)"db.pyj");
    showMenu = menuLogin;
    ui.active_buffer = 0;
}

void irActualizarFicha()
{
    showMenu = menuActualizarFicha;
    ui.active_buffer = 0;
}

void menuMain()
{
    fillRect(0,0,canvas.height,canvas.width,0x333333);
    write((char*)"Bienvenido",10,500-30,10);
    write(sesion.legajo,220,500-30,10);
    printLogo(400,10,90,0xffffff,0x333333);
    canvas.onMouseMove = onMouseMoveMain;
    canvas.onLeftClickDown = onMouseClickMain;
    boton_salir.onClick = salir;
    boton_ir_actualizar.onClick = irActualizarFicha;
    boton_ir_actualizar.draw();
    boton_informe.draw();
    boton_salir.draw();
}
Button boton_ingresar((char*)"Ingresar",10,10,40);
void (*showMenu)();

void onMouseMoveLogin(int x, int y)
{
    boton_ingresar.onMouseMove(x,y);    
}

void onMouseClickLogin(int x, int y)
{
    boton_ingresar.onMouseClick(x,y);    
}

void ingresar()
{   
    bool legajo = false;
    bool password = false;
    for (int i = 0; i < 2; i++)
    {        
        legajo = !strcmp(ui.string_buffer[0],db.personas[i].legajo);
        password = !strcmp(ui.string_buffer[1],db.personas[i].password);
        if (legajo) break;
    }
    //If no password given, don't check it
    if (!strcmp(ui.string_buffer[1],"")) 
    {
        if (legajo)
        {
            ui.active_buffer = 1;
        }else{
            ui.clearBuffer(0);
        }
    }else{
        if (legajo && password)
        {
            ui.clearBuffer(-1);
            ui.active_buffer = 0;
            showMenu = menuMain;
            sesion.mistakes = 0;
        }else{
            ui.clearBuffer(-1);
            ui.active_buffer = 0;
            sesion.mistakes++;
        }
    }
}

void menuLogin()
{
    //Background
    fillRect(0,0,canvas.width,canvas.height,0x333333);

    //Welcome msg
    char welcome_text[255];
    sprintf(welcome_text,"Bienvenidos %s",db.nombre_equipo);
    write(welcome_text,10,canvas.height-25,7);
    
    //Instructions
    write((char*)"Por favor, ingrese sus credenciales",10,canvas.height-100,7);
    
    //Input
    write((char*)"Legajo: ",10,canvas.height-250,7);
    write(ui.string_buffer[0],190,canvas.height-250,8);
    write((char*)"Contrasena: ",10,canvas.height-300,7);
    writeAs(ui.string_buffer[1],190,canvas.height-300,8,'*');

    if (sesion.mistakes > 0)
    {
        if (sesion.mistakes > 2) exit(EXIT_SUCCESS);
        char warning[255];
        sprintf(warning,"Te quedan %d intentos",3-sesion.mistakes);
        write(warning,10,100,5,0xff0000);
    }

    //Logo
    printLogo(400,10,90,0xffffff,0x333333);

    //Event handlers
    canvas.onMouseMove = onMouseMoveLogin;
    canvas.onLeftClickDown = onMouseClickLogin;
    boton_ingresar.onClick = ingresar;
    
    //Submit button
    boton_ingresar.draw();
    
    //Enter handler
    if (ui.string_buffer[ui.active_buffer][ui.string_buffer_size[ui.active_buffer]] == 13)
    {
        ui.string_buffer[ui.active_buffer][ui.string_buffer_size[ui.active_buffer]] = '\0';
        ingresar();
    }
}
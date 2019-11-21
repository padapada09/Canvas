Button boton_ingresar((char*)"Ingresar",10,10,40);
void (*showMenu)();
int wrong_submits = 0;

void onMouseMove1(int x, int y)
{
    boton_ingresar.onMouseMove(x,y);    
}

void onMouseClick1(int x, int y)
{
    boton_ingresar.onMouseClick(x,y);    
}

void ingresar()
{   
    bool legajo = false;
    bool password = false;
    for (int i = 0; i < 3; i++)
    {        
        legajo = !strcmp(string_buffer[0],integrantes[i].legajo);
        password = !strcmp(string_buffer[1],integrantes[i].password);
        if (legajo || password) break;
    }
    if (legajo)
    {
        if (password)
        {
            showMenu = menuMain;
            wrong_submits = 0;
            sprintf(credenciales,string_buffer[0]);        
        }        
    }
    else
    {
        showMenu = menuLogin;
        active_buffer = 0;
        wrong_submits++;
    }
    memset(&string_buffer[0][0], '\0', sizeof(string_buffer[0]));
    string_buffer_size[0] = 0;
    memset(&string_buffer[1][0], '\0', sizeof(string_buffer[1]));
    string_buffer_size[1] = 0;
}

void menuLogin2()
{
    fillRect(0,0,canvas.height,canvas.width,0x333333);
    write((char*)"Por favor, ingrese sus credenciales",10,500-30,7);
    write((char*)"Legajo: ",10,500-100,7);
    write(string_buffer[0],190,500-100,8);
    write((char*)"Contrasena: ",10,500-300,7);
    write(string_buffer[1],190,500-300,8);
    if (wrong_submits > 0)
    {
        if (wrong_submits > 2) exit(EXIT_SUCCESS);
        char warning[255];
        sprintf(warning,"Te quedan %d intentos",3-wrong_submits);
        write(warning,10,100,5,0xff0000);
    }
    printLogo(400,10,90,0xffffff,0x333333);
    canvas.onMouseMove = onMouseMove1;
    canvas.onLeftClickDown = onMouseClick1;
    boton_ingresar.onClick = ingresar;
    boton_ingresar.draw();
    if (string_buffer[1][string_buffer_size[1]] == 13)
    {
        string_buffer[1][string_buffer_size[1]] = '\0';
        boton_ingresar.onClick();
    }
}

void menuLogin()
{
    fillRect(0,0,canvas.height,canvas.width,0x333333);
    write((char*)"Por favor, ingrese sus credenciales",10,500-30,7);
    write((char*)"Legajo: ",10,500-100,7);
    write(string_buffer[0],190,500-100,8);    
    if (wrong_submits > 0)
    {
        if (wrong_submits > 2) exit(EXIT_SUCCESS);
        char warning[255];
        sprintf(warning,"Te quedan %d intentos",3-wrong_submits);
        write(warning,10,100,5,0xff0000);
    }
    printLogo(400,10,90,0xffffff,0x333333);
    canvas.onMouseMove = onMouseMove1;
    canvas.onLeftClickDown = onMouseClick1;
    boton_ingresar.onClick = ingresar;
    boton_ingresar.draw();
    if (string_buffer[0][string_buffer_size[0]] == 13)
    {
        string_buffer[0][string_buffer_size[0]] = '\0';
        active_buffer = 1;
        showMenu = menuLogin2;
    }
}
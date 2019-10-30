int Canvas::loop(double time)
{
    return 0;
}

DWORD WINAPI background_render(LPVOID canvas)
{
    
    //Aca tengo que empezar el cronometro
    auto start = std::chrono::high_resolution_clock::now();
    ((Canvas*)canvas)->render();
    while(1)
    {
        //Aca tengo que tomar el tiempo y ejecutar el loop, según el tiempo que haya pasado
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        ((Canvas*)canvas)->render();
    };
    DWORD response;
    return response;
}

///////////////////////////////
/*INICIALIZADOR | CONSTRUCTOR*/
int Canvas::start(int width, int height, int pixel_x_size, int pixel_y_size)
{

    this->width = width;
    this->height = height;
    this->pixel_x_size = pixel_x_size;
    this->pixel_y_size = pixel_y_size;

    WNDCLASS wc;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = olc_WindowEvent;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpszMenuName = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszClassName = "OLC_PIXEL_GAME_ENGINE";

    RegisterClass(&wc);

    // Define window furniture
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

    // Keep client size as requested
    AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);    

    olc_hWnd = CreateWindowEx(dwExStyle, "OLC_PIXEL_GAME_ENGINE", L"", dwStyle,nCosmeticOffset, nCosmeticOffset, width, height, NULL, NULL, GetModuleHandle(nullptr), this);

    LPVOID variable;
    DWORD thread_id;
    HANDLE second_process = CreateThread(
        NULL,
        0,
        background_render,
        this,
        0,
        &thread_id
    );
    
    WaitForSingleObject(second_process, INFINITE);

    return 0;
}

///////////////////
/*MANEJO DE COLOR*/
void Canvas::findColor(int hex, int color[2], HANDLE output_handler)
{
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    info.cbSize = sizeof(info);
    GetConsoleScreenBufferInfoEx(output_handler, &info);
    int diferences[16];
    for (int color_index = 0; color_index < 16; color_index++)
    {
        int color_table[3];
        color_table[0] = GetRValue(info.ColorTable[color_index]);
        color_table[1] = GetGValue(info.ColorTable[color_index]);
        color_table[2] = GetBValue(info.ColorTable[color_index]);
        diferences[color_index] = abs(color_table[0] - ((hex/256)/256)%256) + abs(color_table[1] - (hex/256)%256) + abs(color_table[2] - hex%256);
    }
    int smallest_index = 0;
    for (int i = 1; i < 16; i++)
    {
        if (diferences[i] < diferences[smallest_index]) smallest_index = i;
    }
    color[0] = 219;
    color[1] = smallest_index;    
}

void Canvas::addColor(int hex)
{
    if (this->pallet_size == 16);
    else
    {
        bool already_added = false;
        CONSOLE_SCREEN_BUFFER_INFOEX info;
        info.cbSize = sizeof(info);
        int diferences[16];
        GetConsoleScreenBufferInfoEx(this->output_handler, &info);
        for (int color_index = 0; color_index < 16; color_index++)
        {
            int color_table[3];
            color_table[0] = GetRValue(info.ColorTable[color_index]);
            color_table[1] = GetGValue(info.ColorTable[color_index]);
            color_table[2] = GetBValue(info.ColorTable[color_index]);
            diferences[color_index] = abs(color_table[0] - ((hex/256)/256)%256) + abs(color_table[1] - (hex/256)%256) + abs(color_table[2] - hex%256);
            if (diferences[color_index] == 0) already_added = 1;
        }
        if (already_added);
        else
        {  
            info.ColorTable[pallet_size++] = hex;
            info.dwSize.Y -= 1; //Por alguna razón, windows agranda este valor al resetearlo, por eso le resto uno antes.
            SetConsoleScreenBufferInfoEx(this->output_handler, &info);
        }
    }
}

//////////
/*DIBUJO*/

void Canvas::render()
{           
    gl
}

void Canvas::clear()
{
    memset(this->buffer, 0 , sizeof(CHAR_INFO) * this->width * this->height);
}

///////////////////////////
/*INSTRUCCIONES DE DIBUJO*/

void Canvas::drawPixel(int x, int y, int color[2])
{
    if (x >= this->width || y >= this->height || x < 0 || y < 0);
    else
    {
        this->buffer[(y * this->width) + x].Char.AsciiChar = 219;
        this->buffer[(y * this->width) + x].Attributes = color[1];
    }
}

void Canvas::drawLine(double x0, double y0, double x1, double y1, int color = 0xffffff)
{
    int console_color[2];
    findColor(color,console_color,this->output_handler);
    double diferencia[2] = {x1-x0,y1-y0};
    double pendiente;
    if (diferencia[0]) pendiente = diferencia[1]/diferencia[0];
    else pendiente = 1000; 
    double ordenada = y0 - (pendiente*x0);
    if (abs(pendiente) > 1)
    {
        for (int y = min(y0,y1); y < max(y0,y1); y++)
        {
            int x = (y-ordenada)/pendiente;            
            this->drawPixel(x,y,console_color);
        }
    }
    else
    {
        for (int x = min(x0,x1); x < max(x0,x1); x++)
        {
            int y = pendiente*x + ordenada;
            this->drawPixel(x,y,console_color);
        }
    }
}

void Canvas::drawLineByAngle(int x0, int y0, double angle, double length, int color = 0xffffff)
{
    int console_color[2];
    angle = (angle / 180) * PI;
    findColor(color,console_color,this->output_handler);
    drawLine(x0,y0,x0 + (int) (cos(angle)*length), y0 + (int) (sin(angle)*length), color);
}

void Canvas::drawCircle(double x0, double y0, double radius, int color = 0xffffff)
{
    int console_color[2];
    findColor(color,console_color,this->output_handler);
    for (int x = -radius; x < radius; x++)
    {
        //Obtengo la imagen
        int y = sqrt((radius*radius) - (x*x));

        //Uso la imagen como y
        this->drawPixel(x0+x,y0+y,console_color);
        this->drawPixel(x0+x,y0-y,console_color);

        //Invierto x por y
        this->drawPixel(x0+y,y0+x,console_color);
        this->drawPixel(x0-y,y0+x,console_color);
    }
}

void Canvas::fillCircle(double x0, double y0, double radius, int color = 0xffffff)
{
    int console_color[2];
    findColor(color,console_color,this->output_handler);
    for(int y=-radius; y<=radius; y++) for(int x=-radius; x<=radius; x++) if(x*x+y*y <= radius*radius) this->drawPixel(x0+x, y0+y, console_color);
}

void Canvas::drawRect(int x, int y, int height, int width, int color = 0xffffff)
{
    int console_color[2];
    findColor(color,console_color,this->output_handler);
    for (int i = x; i <= width + x; i++)
    {
        this->drawPixel(i,y,console_color);
        this->drawPixel(i,y+height,console_color);
    }
    for (int i = y; i <= height + y; i++)
    {
        this->drawPixel(x,i,console_color);
        this->drawPixel(x+width,i,console_color);
    }
}

void Canvas::fillRect(int x0, int y0, int height, int width, int color = 0xffffff)
{
    int console_color[2];
    findColor(color,console_color,this->output_handler);
    for (int x = x0; x <= width + x0; x++) for (int y = y0; y <= height + y0; y++) drawPixel(x,y,console_color);
}

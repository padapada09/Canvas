void Canvas::startWindow()
{
    //Step 1: Registering the Window Class
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = windowProcess;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = GetModuleHandle(nullptr);
    window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    window_class.lpszMenuName = NULL;
    window_class.lpszClassName = "CanvasGL";
    window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&window_class);

    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

    window_handle = CreateWindowEx(dwExStyle,"CanvasGL","UTN FRSF - Canvas",dwStyle,0,0, this->width, this->height,NULL, NULL, GetModuleHandle(nullptr), NULL);


	pfd = {sizeof(PIXELFORMATDESCRIPTOR),1,PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,32,  0, 0, 0, 0, 0, 0,0,0,0,0, 0, 0, 0,24,8,0,PFD_MAIN_PLANE,0,0, 0, 0};

	device_context = GetDC(window_handle);
}

LRESULT CALLBACK Canvas::windowProcess(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(window_handle);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(window_handle, msg, wParam, lParam);
    }
    return 0;
}

void Canvas::setContext()
{
	pixel_format_number = ChoosePixelFormat(device_context,&pfd);
	SetPixelFormat(device_context,pixel_format_number,&pfd);
    opengl_context = wglCreateContext(device_context);
    wglMakeCurrent(device_context,opengl_context);
}


int Canvas::loop(double time)
{
    return 0;
}

DWORD WINAPI background_render(LPVOID canvas)
{
    ((Canvas*)canvas)->setContext();
    DWORD response;
    //Aca tengo que empezar el cronometro
    glViewport(0,0,((Canvas*)canvas)->width,((Canvas*)canvas)->height);
    glClearColor(1,0,0,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers(((Canvas*)canvas)->device_context);    
    auto start = std::chrono::high_resolution_clock::now();
    while(1)
    {
        //Aca tengo que tomar el tiempo y ejecutar el loop, según el tiempo que haya pasado
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        double time = elapsed.count();
        if (!((Canvas*)canvas)->loop(time)) break;
        start = finish;
        SwapBuffers(((Canvas*)canvas)->device_context);
        glClear(GL_COLOR_BUFFER_BIT);
    };
    return response;
}

///////////////////////////////
/*INICIALIZADOR | CONSTRUCTOR*/
int Canvas::start(int width, int height)
{

    this->width = width;
    this->height = height;

    this->startWindow();

    ShowWindow(this->window_handle,1);

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

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    
    WaitForSingleObject(second_process, INFINITE);

    return 0;
}

//////////
/*DIBUJO*/

///////////////////////////
/*INSTRUCCIONES DE DIBUJO*/

void Canvas::drawPixel(double x, double y, int color)
{
    glPointSize(1);
    glBegin(GL_POINTS);
        glColor3f(((color/16)/16)%16,(color/16)%16,color%16);        
        glVertex2f((((x/this->width)*2)-1),(((y/this->height)*2)-1));
    glEnd();
}

void Canvas::drawLine(double x0, double y0, double x1, double y1, int color = 0xffffff)
{
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
            this->drawPixel(x,y,color);
        }
    }
    else
    {
        for (int x = min(x0,x1); x < max(x0,x1); x++)
        {
            int y = pendiente*x + ordenada;
            this->drawPixel(x,y,color);
        }
    }
}

void Canvas::drawLineByAngle(int x0, int y0, double angle, double length, int color = 0xffffff)
{    
    angle = (angle / 180) * PI;    
    drawLine(x0,y0,x0 + (int) (cos(angle)*length), y0 + (int) (sin(angle)*length), color);
}

void Canvas::drawCircle(double x0, double y0, double radius, int color = 0xffffff)
{    
    for (int x = -radius; x < radius; x++)
    {
        //Obtengo la imagen
        int y = sqrt((radius*radius) - (x*x));

        //Uso la imagen como y
        this->drawPixel(x0+x,y0+y,color);
        this->drawPixel(x0+x,y0-y,color);

        //Invierto x por y
        this->drawPixel(x0+y,y0+x,color);
        this->drawPixel(x0-y,y0+x,color);
    }
}

void Canvas::fillCircle(double x0, double y0, double radius, int color = 0xffffff)
{    
    for(int y=-radius; y<=radius; y++) for(int x=-radius; x<=radius; x++) if(x*x+y*y <= radius*radius) this->drawPixel(x0+x, y0+y, color);
}

void Canvas::drawRect(int x, int y, int height, int width, int color = 0xffffff)
{        
    for (int i = x; i <= width + x; i++)
    {
        this->drawPixel(i,y,color);
        this->drawPixel(i,y+height,color);
    }
    for (int i = y; i <= height + y; i++)
    {
        this->drawPixel(x,i,color);
        this->drawPixel(x+width,i,color);
    }
}

void Canvas::fillRect(int x0, int y0, int height, int width, int color = 0xffffff)
{    
    for (int x = x0; x <= width + x0; x++) for (int y = y0; y <= height + y0; y++) drawPixel(x,y,color);
}

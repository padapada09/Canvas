#define _WIN32_WINNT 0x0601
#define PI 3.141592653
#include <windows.h>
#include <chrono>
#include <string>
#include <math.h>
#include <GL/gl.h>
#include <iostream>

/*ÁREA: ACLARACIONES*/
/*		Glosario:
			#Usuario = La persona que implemente la librería en su programa.
			#Librería = Todo el codigo definido en este archvio.
			#Hilo asincrónico = El hilo de ejecución manejado unicamente en la librería (creado en la función "start()").
			#Programa = El programa desarrollado por el usuario.
			#Cliente = Quien usa el programa.
			#Área = Porción de codigo dedicada a una tarea puntual de la librería.

		Para poder compilar se debe avisar al compilador que debe linkear las librerias necesarias para trabajar
		con openGL. Tenes que agregar esto como parametros de compilación: "-l gdi32 -l opengl32".
		Para hacerlo con zinjai:
*///	->Run->Configure->Extra arguments for compiler => En el textbox agrega: "-l gdi32 -l opengl32"
/*FÍN ÁREA: ACLARACIONES*/

/*ÁREA: DECLARACIÓN DE PROTOTIPOS Y ESTRUCTURAS*/
void setContext();
void drawCircle(double x_center, double y_center, double radius, int color);
void drawSemiCircle(double x_center, double y_center, double radius, double from , double to, int color);
void fillCircle(double x_center, double y_center, double radius, int color);
void fillSemiCircle(double x0, double y0, double radius, double from, double to, int color);
void drawLineByAngle(int x0, int y0, double angle, double length, int color);
void drawLine(double x1, double y1, double x2, double y2, int color);
void drawRect(double x, double y, double height, double width, int color);
void fillRect(double x, double y, double height, double width, int color);
void drawTriangle(double x0, double y0, double x1, double y1, double x2, double y2, int color);
void drawPixel(double x, double y, int color);
void clear();
void addColor(int hex);
std::string intToString(int value);
void startWindow();
DWORD WINAPI background_render(LPVOID null);
static LRESULT CALLBACK windowProcess(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam);
void findColor(int hex,int color[2], HANDLE hStdout);

	/*SUB-ÁREA: DECLARACIÓN DE PROTOTIPOS VIRTUALES*/
	/*	Estas dos funciones no se implementan en la librería. El usuario las debe implementar.
			
			*/int setUp(int &width, int &height);/*
			Llamada por el hilo asincrónico una unica vez 
			para configurar la pantalla (y posiblemente despues la librería permita hacer
			más configuraciones).

			*/int loop(double time);/*
			El hilo asincrónico llama a loop por cada frame que pone en la pantalla.
			Le manda como parametro un double time, que le avisa al programa, 
	*///	cuanto tiempo paso desde el útlimo frame mostrado al cliente.
	/*FÍN SUB-ÁREA: DECLARACIÓN DE PROTOTIPOS VIRTUALES*/

/*Estructura para poder almacenar toda la información pertinente a la venta y su contexto*/
struct Canvas
{
	HANDLE output_handler;
	HANDLE input_handler;
	HGLRC opengl_context;
	MSG Msg;
	WNDCLASSEX window_class;
	PIXELFORMATDESCRIPTOR pfd;
	int pixel_format_number;
	int width;
	int height;
	HWND window_handle;
	HDC device_context;
}canvas;

int main()
{
	canvas.output_handler = GetStdHandle(STD_OUTPUT_HANDLE);
	canvas.input_handler = GetStdHandle(STD_INPUT_HANDLE);
	setUp(canvas.width, canvas.height);
	startWindow();
	ShowWindow(canvas.window_handle,1);
	LPVOID variable;
	DWORD thread_id;
	HANDLE second_process = CreateThread(NULL,0,background_render,nullptr,0,&thread_id);
	
	while(GetMessage(&canvas.Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&canvas.Msg);
		DispatchMessage(&canvas.Msg);
	}
}

/*	ÁREA: CONFIGURACIÓN DE VENTANA Y SU CONTEXTO
		Índice:
			#startWindow(): Crea una ventana, define un formato de pixel y almacena el "device context" de la ventana creada.
			#setContext(): Crea un contexto y lo conecta con el "device context" de nuestra ventana.
			#windowProcess(): Es una función que se llama cada vez que ocurre un evento en la venta (mouse,teclado,etc).
		Tips:
			#La mayoría de las cosas acá seguro tienen una mejor forma de hacerse. Pero para el objetivo de la librería, no
*///		tiene sentido algo mucho mas complejo.
void startWindow()
{
	canvas.window_class.cbSize = sizeof(WNDCLASSEX);
	canvas.window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	canvas.window_class.lpfnWndProc = windowProcess;
	canvas.window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	canvas.window_class.hInstance = GetModuleHandle(nullptr);
	canvas.window_class.lpszClassName = "CanvasGL";
	RegisterClassEx(&canvas.window_class);
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_BORDER | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	canvas.window_handle = CreateWindowEx(dwExStyle,"CanvasGL","UTN FRSF - Canvas",dwStyle,0,0, canvas.width, canvas.height,NULL, NULL, GetModuleHandle(nullptr), NULL);
	canvas.pfd = {sizeof(PIXELFORMATDESCRIPTOR),1,PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,32,  0, 0, 0, 0, 0, 0,0,0,0,0, 0, 0, 0,24,8,0,PFD_MAIN_PLANE,0,0, 0, 0};
	canvas.device_context = GetDC(canvas.window_handle);
	canvas.pixel_format_number = ChoosePixelFormat(canvas.device_context,&canvas.pfd);
	SetPixelFormat(canvas.device_context,canvas.pixel_format_number,&canvas.pfd);
}

void setContext()
{
	canvas.opengl_context = wglCreateContext(canvas.device_context);
	wglMakeCurrent(canvas.device_context,canvas.opengl_context);
}

LRESULT CALLBACK windowProcess(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(window_handle);
		return WS_MINIMIZEBOX;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, msg, wParam, lParam);
	}
	return 0;
}

/*FÍN ÁREA: CONFIGURACIÓN DE VENTANA Y SU CONTEXTO*/

/*		Esta función se ejecuta en un hilo asincrónico.
		Se encarga de calcular los frames por segundo, de imprimir en pantalla, 
*///	y de ejecucar la función del programa.
DWORD WINAPI background_render(LPVOID null)
{
	setContext();
	RECT client_screen;
	GetClientRect(canvas.window_handle,&client_screen);
	glViewport(0,0,client_screen.right,client_screen.bottom);
	glClearColor(0,0,0,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	auto start = std::chrono::high_resolution_clock::now(); //Tomo el tiempo de inicio de dibujado
	SwapBuffers(canvas.device_context); //Esta función basicamente me imprime en pantalla lo dibujado
	while(1)
	{
		auto finish = std::chrono::high_resolution_clock::now(); //Tomo el tiempo de finalización de dibujado
		std::chrono::duration<double> elapsed = finish - start;
		start = finish; //Vuelvo a tomar el tiempo de inicio
		double time = elapsed.count();
		if (SetWindowTextA(canvas.window_handle,("UTN FRSF - ALGORITMOS - FPS: " + intToString( (int) (1.0/time)) ).c_str()));
		else break;
		if (!loop(time)) break; //Ejecuto el programa
		SwapBuffers(canvas.device_context); //Imprimo lo ejecutado en el programa
		glClear(GL_COLOR_BUFFER_BIT); //Limpio la pantalla
	};
	/*Retorno esto porque el metodo CreateThread me pide que tenga este tipo de retorno.
	No lo uso para nada*/
	DWORD response;
	return response;
}

/*
AREA DE CODIGO : INSTRUCCIONES DE DIBUJO
Tips:
	#La función glBegin y glEnd me permiten conectar vectores de diferentes maneras,
		según el parametro que le pases a glBegin();
	#glVertex2f te permite definir un vector/punto en el espacio cuyas dimensiones deben estár en ([-1,1],[-1,1]).
		Siendo que x=1 es el extremo derecho de la pantalla, y x=-1 es el extremo izquierdo (De manera analoga con y).
	#glPointSize te permite definir el tamaño del punto en pixeles.
	#glLineWidth te permite definir el ancho del vector trazado en pixeles (No estoy seguro si en pixeles).
*/
void drawPixel(double x, double y, int color)
{
	glPointSize(1);
	glBegin(GL_POINTS);
	glColor3f(((color/16)/16)%16,(color/16)%16,color%16);        
	x = (((x/canvas.width)*2)-1);
	y = (((y/canvas.height)*2)-1);
	glVertex2f(x,y);
	glEnd();
}

void drawLine(double x0, double y0, double x1, double y1, int color = 0xffffff)
{
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glColor3f(red,green,blue);
	glBegin(GL_LINES);
	x0 = (((x0/canvas.width)*2)-1);
	y0 = (((y0/canvas.height)*2)-1);
	x1 = (((x1/canvas.width)*2)-1);
	y1 = (((y1/canvas.height)*2)-1);
	glVertex2f(x0,y0);
	glVertex2f(x1,y1);
	glEnd();
}

void drawLineByAngle(int x0, int y0, double angle, double length, int color = 0xffffff)
{    
	angle = (angle / 180) * PI;
	drawLine(x0,y0,x0 + (int) (cos(angle)*length), y0 + (int) (sin(angle)*length), color);
}

void drawCircle(double x0, double y0, double radius, int color = 0xffffff)
{   
	const float DEG2RAD = 3.14159/180;
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glColor3f(red,green,blue);
	glBegin(GL_LINE_LOOP);
	for (int i=0; i < 360; i++)
	{
		float degInRad = i*DEG2RAD;
		double x = (x0+cos(degInRad)*radius);
		double y = (y0+sin(degInRad)*radius);
		glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	}
	glEnd();
}

void drawSemiCircle(double x0, double y0, double radius, double from, double to, int color = 0xffffff)
{
	const float DEG2RAD = 3.14159/180;
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glColor3f(red,green,blue);
	glBegin(GL_LINE_STRIP);
	int i_add = 1;
	if (from >= to) i_add = -1; 
	for (int i=from; i != to; i += i_add)
	{
		float degInRad = i*DEG2RAD;
		double x = (x0+cos(degInRad)*radius);
		double y = (y0+sin(degInRad)*radius);
		glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	}
	glEnd();
}

void fillSemiCircle(double x0, double y0, double radius, double from, double to, int color = 0xffffff)
{
	const float DEG2RAD = 3.14159/180;
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glColor3f(red,green,blue);
	glBegin(GL_POLYGON);
	int i_add = 1;
	if (from >= to) i_add = -1; 
	for (int i=from; i != to; i += i_add)
	{
		float degInRad = i*DEG2RAD;
		double x = (x0+cos(degInRad)*radius);
		double y = (y0+sin(degInRad)*radius);
		glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	}
	glEnd();
}

void fillCircle(double x0, double y0, double radius, int color = 0xffffff)
{    
	const float DEG2RAD = 3.14159/180;
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glColor3f(red,green,blue);
	glBegin(GL_POLYGON);
	for (int i=0; i < 360; i++)
	{
		float degInRad = i*DEG2RAD;
		double x = (x0+cos(degInRad)*radius);
		double y = (y0+sin(degInRad)*radius);
		glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	}
	glEnd();
}

void drawRect(double x, double y, double width, double height, int color = 0xffffff)
{   
	glBegin(GL_LINE_LOOP);
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glColor3f(red,green,blue);
	glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glVertex2f(((((x+width)/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glVertex2f(((((x+width)/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glVertex2f((((x/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glEnd();
}

void fillRect(double x, double y, double width, double height, int color = 0xffffff)
{    
	glBegin(GL_POLYGON);
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glColor3f(red,green,blue);
	glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glVertex2f(((((x+width)/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glVertex2f(((((x+width)/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glVertex2f((((x/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glEnd();
}

void drawTriangle(double x0, double y0, double x1, double y1, double x2, double y2, int color = 0xffffff)
{
	drawLine(x0,y0,x1,y1,color);
	drawLine(x1,y1,x2,y2,color);
	drawLine(x2,y2,x0,y0,color);
}

/*ÁREA DE CODIGO : HERRAMIENTAS*/
bool isPressed(int key)
{
	if (GetAsyncKeyState(key) && 0x8000) return true;
	return false;
}

std::string intToString(int value) {
	std::string number = "";
	if (value < 10)
	{
		number += (char)(value + 48);
		return number;
	}
	else
	{
		number += intToString(value / 10);
		number += (char)(value % 10 + 48);
		return number;
	}
}

void write(std::string text, float x0, float y0, float size, int color = 0xffffff)
{
	for (int letter = 0; letter < text.size(); letter++)
	{
		switch (text[letter])
		{
			case 'a':
			case 'A':
				drawLine(x0+0*size,y0+0*size,x0+3*size,y0+9*size,color);
				drawLine(x0+3*size,y0+9*size,x0+6*size,y0+0*size,color);
				drawLine(x0+0.4*3*size,y0+0.4*9*size,x0+1.6*3*size,y0+0.4*9*size,color);
				x0 += 7*size;
				break;
			case 'b':
			case 'B':
				drawSemiCircle(x0+1*size,y0+6.5*size,2.5*size,90,-45,color);
				drawSemiCircle(x0+1*size,y0+2.7*size,2.7*size,70,-90,color);
				drawLine(x0+0*size,y0+0*size,x0+0*size,y0+9*size,color);
				drawLine(x0+0*size,y0+9*size,x0+1.1*size,y0+9*size,color);
				drawLine(x0+0*size,y0+0*size,x0+1.1*size,y0+0*size,color);
				x0 += 5*size;
				break;
			case 'c':
			case 'C':
				drawSemiCircle(x0+4.5*size,y0+4.5*size,4.5*size,85,280,color);
				x0 += 7*size;
				break;
			case 'd':
			case 'D':
				drawSemiCircle(x0+0*size,y0+4.5*size,4.5*size,85,-85,color);
				drawLine(x0,y0,x0,y0+9*size,color);
				x0 += 6*size;
				break;
			case 'e':
			case 'E':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0,y0+9*size,x0+4*size,y0+9*size,color);
				drawLine(x0,y0+4.5*size,x0+4*size,y0+4.5*size,color);
				drawLine(x0,y0,x0+4*size,y0,color);
				x0 += 5.3*size;
				break;
			case 'f':
			case 'F':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0,y0+9*size,x0+4*size,y0+9*size,color);
				drawLine(x0,y0+4.5*size,x0+4*size,y0+4.5*size,color);
				x0 += 5.3*size;
				break;
			case 'g':
			case 'G':
				drawSemiCircle(x0+4.5*size,y0+4.5*size,4.5*size,90,290,color);
				drawLine(x0+6*size,y0,x0+6*size,y0+4.5*size,color);
				drawLine(x0+6*size,y0+4.5*size,x0+4*size,y0+4.5*size,color);
				x0 += 8*size;
				break;
			case 'h':
			case 'H':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0+4*size,y0,x0+4*size,y0+9*size,color);
				drawLine(x0,y0+4.5*size,x0+4*size,y0+4.5*size,color);
				x0 += 6*size;
				break;
			case 'i':
			case 'I':
				drawLine(x0,y0,x0,y0+9*size,color);
				x0 += 2*size;
				break;
			case 'j':
			case 'J':
				drawLine(x0+3*size,y0+3*size,x0+3*size,y0+9*size,color);
				drawSemiCircle(x0,y0+3*size,3*size,0,-90,color);
				x0 += 5*size;
				break;
			case 'k':
			case 'K':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0,y0+4.5*size,x0+4*size,y0+9*size,color);
				drawLine(x0,y0+4.5*size,x0+4*size,y0,color);
				x0 += 5*size;
				break;
			case 'l':
			case 'L':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0,y0,x0+3*size,y0,color);
				x0 += 4*size;
				break;
			case 'm':
			case 'M':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0+6*size,y0,x0+6*size,y0+9*size,color);
				drawLine(x0,y0+9*size,x0+3*size,y0+5*size,color);
				drawLine(x0+6*size,y0+9*size,x0+3*size,y0+5*size,color);
				x0 += 7.5*size;
				break;
			case 'n':
			case 'N':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0+4.5*size,y0,x0+4.5*size,y0+9*size,color);
				drawLine(x0,y0+9*size,x0+4.5*size,y0,color);
				x0 += 5.5*size;
				break;
			case 'o':
			case 'O':
				drawSemiCircle(x0+3*size,y0+6*size,3*size,0,180,color);
				drawSemiCircle(x0+3*size,y0+3*size,3*size,180,360,color);
				drawLine(x0,y0+3*size,x0,y0+6*size,color);
				drawLine(x0+6*size,y0+3*size,x0+6*size,y0+6*size,color);
				x0 += 7*size;
				break;
			case 'p':
			case 'P':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0,y0+9*size,x0+1*size,y0+9*size,color);
				drawLine(x0,y0+4*size,x0+1*size,y0+4*size,color);
				drawSemiCircle(x0+1*size,y0+6.5*size,2.5*size,90,-90,color);
				x0 += 4.5*size;
				break;
			case 'q':
			case 'Q':
				drawSemiCircle(x0+3*size,y0+6*size,3*size,0,180,color);
				drawSemiCircle(x0+3*size,y0+3*size,3*size,180,360,color);
				drawLine(x0,y0+3*size,x0,y0+6*size,color);
				drawLine(x0+6*size,y0+3*size,x0+6*size,y0+6*size,color);
				drawLine(x0+3*size,y0+4.5*size,x0+6*size,y0,color);
				x0 += 7*size;
				break;
			case 'r':
			case 'R':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0,y0+9*size,x0+1*size,y0+9*size,color);
				drawLine(x0,y0+4*size,x0+1*size,y0+4*size,color);
				drawSemiCircle(x0+1*size,y0+6.5*size,2.5*size,90,-90,color);
				drawLine(x0+2.5*size,y0+4.5*size,x0+3.5*size,y0,color);
				x0 += 4.5*size;
				break;
			case 's':
			case 'S':
				drawSemiCircle(x0+3*size,y0+6.75*size,2.25*size,55,260,color);
				drawSemiCircle(x0+2.25*size,y0+2.25*size,2.25*size,80,-140,color);
				x0 += 5.5*size;
				break;
			case 't':
			case 'T':
				drawLine(x0+3*size,y0,x0+3*size,y0+9*size,color);
				drawLine(x0,y0+9*size,x0+6*size,y0+9*size,color);
				x0 += 7*size;
				break;
			case 'u':
			case 'U':
				drawSemiCircle(x0+2.5*size,y0+2.5*size,2.5*size,180,360,color);
				drawLine(x0,y0+2.5*size,x0,y0+9*size,color);
				drawLine(x0+5*size,y0+2.5*size,x0+5*size,y0+9*size,color);
				x0 += 6*size;
				break;
			case 'v':
			case 'V':
				drawLine(x0+2*size,y0,x0,y0+9*size,color);
				drawLine(x0+2*size,y0,x0+4*size,y0+9*size,color);
				x0 += 5*size;
				break;
			case 'w':
			case 'W':
				drawLine(x0,y0,x0,y0+9*size,color);
				drawLine(x0+6*size,y0,x0+6*size,y0+9*size,color);
				drawLine(x0,y0,x0+3*size,y0+5*size,color);
				drawLine(x0+6*size,y0,x0+3*size,y0+5*size,color);
				x0 += 7.5*size;
				break;
			case 'x':
			case 'X':
				drawLine(x0,y0+9*size,x0+4*size,y0,color);
				drawLine(x0,y0,x0+4*size,y0+9*size,color);
				x0 += 5*size;
				break;
			case 'y':
			case 'Y':
				drawLine(x0+2*size,y0,x0+2*size,y0+4.5*size,color);
				drawLine(x0,y0+9*size,x0+2*size,y0+4.5*size,color);
				drawLine(x0+4*size,y0+9*size,x0+2*size,y0+4.5*size,color);
				x0 += 5*size;
				break;
			case 'z':
			case 'Z':
				drawLine(x0,y0,x0+5*size,y0,color);
				drawLine(x0,y0+9*size,x0+5*size,y0+9*size,color);
				drawLine(x0,y0,x0+5*size,y0+9*size,color);
				x0 += 6*size;
				break;
			case ' ':
				x0 += 5*size;
				break;
			case '.':
				fillCircle(x0+0.7*size,y0+0.7*size,0.7*size,color);
				x0 += 1*size;
				break;
			case ',':
				drawLine(x0+0.7*size,y0+0.7*size,x0+0.4*size,y0-1.3*size,color);
				x0 += 1*size;
				break;
			case '!':
				fillCircle(x0+1*size,y0+1*size,0.3*size,color);
				drawLine(x0+1*size,y0+2*size,x0+1*size,y0+9*size,color);
				x0 += 2*size;
				break;
			case '?':
				fillCircle(x0+1*size,y0+1*size,0.3*size,color);
				drawSemiCircle(x0+1*size,y0+7*size,2*size,90,-90,color);
				drawLine(x0+1*size,y0+5*size,x0+1*size,y0+2*size,color);
				break;
			case '0':
				drawSemiCircle(x0+3*size,y0+6*size,3*size,0,180,color);
				drawSemiCircle(x0+3*size,y0+3*size,3*size,180,360,color);
				drawLine(x0,y0+3*size,x0,y0+6*size,color);
				drawLine(x0+6*size,y0+3*size,x0+6*size,y0+6*size,color);
				drawLine(x0,y0+6*size,x0+6*size,y0+3*size,color);
				x0 += 7*size;
				break;
			case '1':
				drawLine(x0+3*size,y0,x0+3*size,y0+9*size,color);
				drawLine(x0,y0+5*size,x0+3*size,y0+9*size,color);
				x0 += 5*size;
				break;
			case '2':
				drawSemiCircle(x0+3*size,y0+6*size,3*size,160,0,color);
				drawLine(x0+6*size,y0+6*size,x0,y0,color);
				drawLine(x0,y0,x0+6*size,y0,color);
				x0 += 7*size;
				break;
			case '3':
				drawSemiCircle(x0+2.25*size,y0+6.75*size,2.25*size,120,-90,color);
				drawSemiCircle(x0+2.25*size,y0+2.25*size,2.25*size,90,-140,color);
				x0 += 7*size;
				break;
			case '4':
				drawLine(x0+4*size,y0,x0+4*size,y0+9*size,color);
				drawLine(x0,y0+3.5*size,x0+4*size,y0+9*size,color);
				drawLine(x0,y0+3.5*size,x0+6*size,y0+3.5*size,color);
				x0 += 7*size;
				break;
			case '5':
				drawLine(x0,y0+9*size,x0+5*size,y0+9*size,color);
				drawLine(x0,y0+9*size,x0,y0+5*size,color);
				drawSemiCircle(x0+2.5*size,y0+2.5*size,2.5*size,90,-90,color);
				drawLine(x0,y0+5*size,x0+2.5*size,y0+5*size,color);
				drawLine(x0,y0,x0+2.5*size,y0,color);
				x0 += 6*size;
				break;
			case '6':
				drawCircle(x0+2.5*size,y0+2.5*size,2.5*size,color);
				drawSemiCircle(x0+6*size,y0+2.5*size,6*size,180,100,color);
				x0 += 7*size;
				break;
			case '7':
				drawLine(x0,y0+9*size,x0+5*size,y0+9*size,color);
				drawLine(x0+5*size,y0+9*size,x0,y0,color);
				x0 += 6*size;
				break;
			case '8':
				drawCircle(x0+2.5*size,y0+7*size,2*size,color);
				drawCircle(x0+2.5*size,y0+2.5*size,2.5*size,color);
				x0 += 6*size;
				break;
			case '9':
				drawLine(x0+4*size,y0,x0+4*size,y0+7*size,color);
				drawCircle(x0+2*size,y0+7*size,2*size,color);
				x0 += 6*size;
				break;
		}
	}
}
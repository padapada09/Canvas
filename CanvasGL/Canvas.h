#define _WIN32_WINNT 0x0601
#define PI 3.141592653
#include <windows.h>
#include <chrono>
#include "tools.h"
#include <math.h>
#include <GL/gl.h>
#include <iostream>

/* Para poder compilar con minGW debe agregarse "-l gdi32 -l opengl32"*/

void setContext();
void drawCircle(double x_center, double y_center, double radius, int color);
void fillCircle(double x_center, double y_center, double radius, int color);
void drawLineByAngle(int x0, int y0, double angle, double length, int color);
void drawLine(double x1, double y1, double x2, double y2, int color);
void drawRect(double x, double y, double height, double width, int color);
void fillRect(double x, double y, double height, double width, int color);
void drawPixel(double x, double y, int color);
int loop(double time);
void clear();
void addColor(int hex);
void startWindow();
DWORD WINAPI background_render(LPVOID null);
static LRESULT CALLBACK windowProcess(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam);
void findColor(int hex,int color[2], HANDLE hStdout);
int setUp(int &width, int &height);

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
	HANDLE second_process = CreateThread(
										 NULL,
										 0,
										 background_render,
										 nullptr,
										 0,
										 &thread_id
										 );
	
	while(GetMessage(&canvas.Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&canvas.Msg);
		DispatchMessage(&canvas.Msg);
	}
	
	WaitForSingleObject(second_process, INFINITE);
}

void startWindow()
{
	//Step 1: Registering the Window Class
	canvas.window_class.cbSize = sizeof(WNDCLASSEX);
	canvas.window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	canvas.window_class.lpfnWndProc = windowProcess;
	canvas.window_class.cbClsExtra = 0;
	canvas.window_class.cbWndExtra = 0;
	canvas.window_class.hInstance = GetModuleHandle(nullptr);
	canvas.window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	canvas.window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	canvas.window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	canvas.window_class.lpszMenuName = NULL;
	canvas.window_class.lpszClassName = "CanvasGL";
	canvas.window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	RegisterClassEx(&canvas.window_class);
	
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;
	
	canvas.window_handle = CreateWindowEx(dwExStyle,"CanvasGL","UTN FRSF - Canvas",dwStyle,0,0, canvas.width, canvas.height,NULL, NULL, GetModuleHandle(nullptr), NULL);
	
	
	canvas.pfd = {sizeof(PIXELFORMATDESCRIPTOR),1,PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,32,  0, 0, 0, 0, 0, 0,0,0,0,0, 0, 0, 0,24,8,0,PFD_MAIN_PLANE,0,0, 0, 0};
	
	canvas.device_context = GetDC(canvas.window_handle);
}

LRESULT CALLBACK windowProcess(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam)
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

void setContext()
{
	canvas.pixel_format_number = ChoosePixelFormat(canvas.device_context,&canvas.pfd);
	SetPixelFormat(canvas.device_context,canvas.pixel_format_number,&canvas.pfd);
	canvas.opengl_context = wglCreateContext(canvas.device_context);
	wglMakeCurrent(canvas.device_context,canvas.opengl_context);
}

DWORD WINAPI background_render(LPVOID null)
{
	setContext();
	DWORD response;
	//Aca tengo que empezar el cronometro
	glViewport(0,0,canvas.width,canvas.height);
	glClearColor(0,0,0,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers(canvas.device_context);    
	auto start = std::chrono::high_resolution_clock::now();
	while(1)
	{
		//Aca tengo que tomar el tiempo y ejecutar el loop, según el tiempo que haya pasado
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		double time = elapsed.count();
		if (!loop(time)) break;
		start = finish;
		SwapBuffers(canvas.device_context);
		glClear(GL_COLOR_BUFFER_BIT);
	};
	return response;
}

///////////////////////////////
/*INICIALIZADOR | CONSTRUCTOR*/


///////////////////////////
/*INSTRUCCIONES DE DIBUJO*/
void drawPixel(double x, double y, int color)
{
	glPointSize(1);
	glBegin(GL_POINTS);
	glColor3f(((color/16)/16)%16,(color/16)%16,color%16);        
	glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glEnd();
}

void drawLine(double x0, double y0, double x1, double y1, int color = 0xffffff)
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
			drawPixel(x,y,color);
		}
	}
	else
	{
		for (int x = min(x0,x1); x < max(x0,x1); x++)
		{
			int y = pendiente*x + ordenada;
			drawPixel(x,y,color);
		}
	}
}

void drawLineByAngle(int x0, int y0, double angle, double length, int color = 0xffffff)
{    
	angle = (angle / 180) * PI;    
	drawLine(x0,y0,x0 + (int) (cos(angle)*length), y0 + (int) (sin(angle)*length), color);
}

void drawCircle(double x0, double y0, double radius, int color = 0xffffff)
{   
	glBegin(GL_LINE_LOOP);
	const float DEG2RAD = 3.14159/180;
	glColor3f(((color/16)/16)%16,(color/16)%16,color%16);
	for (int i=0; i < 360; i++)
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
	glBegin(GL_POLYGON);
	const float DEG2RAD = 3.14159/180;
	glColor3f(((color/16)/16)%16,(color/16)%16,color%16);
	for (int i=0; i < 360; i++)
	{
		float degInRad = i*DEG2RAD;
		double x = (x0+cos(degInRad)*radius);
		double y = (y0+sin(degInRad)*radius);
		glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	}
	glEnd();
}

void drawRect(double x, double y, double height, double width, int color = 0xffffff)
{   
	glBegin(GL_LINE_LOOP);
	glColor3f(((color/16)/16)%16,(color/16)%16,color%16);
	glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glVertex2f(((((x+width)/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glVertex2f(((((x+width)/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glVertex2f((((x/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glEnd();
}

void fillRect(double x, double y, double height, double width, int color = 0xffffff)
{    
	glBegin(GL_POLYGON);
	glColor3f(((color/16)/16)%16,(color/16)%16,color%16);
	glVertex2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glVertex2f(((((x+width)/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glVertex2f(((((x+width)/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glVertex2f((((x/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glEnd();
}

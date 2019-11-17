#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0601
#endif
#define PI 3.141592653
#include <windows.h>
#include <winuser.h>
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
/*FÍN ÁREA: ACLARACIONES*/

/*Funciones vacias en caso de que el usuario no quiera especificar un handler*/
void keyIgnore(int){}
void clickIgnore(int,int){}
void reziseIgnore(){}

/*ÁREA: DECLARACIÓN DE PROTOTIPOS Y ESTRUCTURAS*/

/*Estructura para poder almacenar toda la información pertinente a la venta y su contexto*/
struct Canvas
{
	HANDLE output_handler;
	HANDLE input_handler;
	bool key_pressed[256] = {0};
	void (*onKeyDown)(int) = keyIgnore; //Punteros de funciones que el usuario puede asignar a sus propios handlers
	void (*onKeyUp)(int) = keyIgnore;
	void (*onResizing)() = reziseIgnore;
	void (*onLeftClickDown)(int,int) = clickIgnore;
	void (*onRightClickDown)(int,int) = clickIgnore;
	void (*onLeftClickUp)(int,int) = clickIgnore;
	void (*onRightClickUp)(int,int) = clickIgnore;
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

void setContext();
void updateViewPort();
void drawCircle(double x_center, double y_center, double radius, int color);
void drawSemiCircle(double x_center, double y_center, double radius, double from , double to, int color);
void fillCircle(double x_center, double y_center, double radius, int num_segments, int color);
void fillSemiCircle(double x0, double y0, double radius, double from, double to, int color);
void drawLineByAngle(int x0, int y0, double angle, double length, int color);
void drawLine(double x1, double y1, double x2, double y2, int color);
void drawRect(double x, double y, double height, double width, int color);
void fillRect(double x, double y, double height, double width, int color);
void drawTriangle(double x0, double y0, double x1, double y1, double x2, double y2, int color);
std::string intToString(int value);
void startWindow();
DWORD WINAPI background_render(LPVOID null);
static LRESULT CALLBACK windowProcess(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam);
void findColor(int hex,int color[2], HANDLE hStdout);

	/*SUB-ÁREA: DECLARACIÓN DE PROTOTIPOS VIRTUALES*/
	/*	Estas dos funciones no se implementan en la librería. El usuario las debe implementar.
			
			*/int setUp();/*
			Llamada por el hilo asincrónico una unica vez 
			para configurar la pantalla (y posiblemente despues la librería permita hacer
			más configuraciones).

			*/int loop(float time);/*
			El hilo asincrónico llama a loop por cada frame que pone en la pantalla.
			Le manda como parametro un double time, que le avisa al programa, 
	*///	cuanto tiempo paso desde el útlimo frame mostrado al cliente.
	/*FÍN SUB-ÁREA: DECLARACIÓN DE PROTOTIPOS VIRTUALES*/

HINSTANCE gdi32Lib;
HINSTANCE opengl32Lib;

typedef BOOL (__stdcall *SetPixelForm)(HDC,int,const PIXELFORMATDESCRIPTOR*);
typedef BOOL (__stdcall *ChoosePixelForm)(HDC,const PIXELFORMATDESCRIPTOR*);
typedef HGLRC (__stdcall *wglCreateCont)(HDC);
typedef BOOL (__stdcall *wglMakeCurr)(HDC,HGLRC);
typedef void (__stdcall *glViewpor)(GLint,GLint,GLsizei,GLsizei);
typedef void (__stdcall *glClearCol)(GLclampf,GLclampf,GLclampf,GLclampf);
typedef void (__stdcall *glCle)(GLbitfield);
typedef BOOL (__stdcall *SwapBuff)(HDC);
typedef void (__stdcall *glCol3f)(GLfloat,GLfloat,GLfloat);
typedef void (__stdcall *glBeg)(GLenum);
typedef void (__stdcall *glEn)(void);
typedef void (__stdcall *glVert2f)(GLfloat,GLfloat);
typedef void (__stdcall *glLineWid)(GLfloat);
SetPixelForm SetPixelF;
ChoosePixelForm ChoosePixelF;
wglCreateCont wglCreateC;
wglMakeCurr wglMakeC;
glViewpor glViewp;
glClearCol glClearC;
glCle glC;
SwapBuff SwapB;
glCol3f glC3f;
glBeg glB;
glEn glE;
glVert2f glV2f;
glLineWid glLineW;

/*FÍN ÁREA: DECLARACIÓN DE PROTOTIPOS Y ESTRUCTURAS*/

int main()
{
	gdi32Lib = LoadLibrary(TEXT("gdi32.dll"));
	opengl32Lib = LoadLibrary(TEXT("opengl32.dll"));
	ChoosePixelF = (ChoosePixelForm) GetProcAddress(gdi32Lib, "ChoosePixelFormat");
	SetPixelF = (SetPixelForm) GetProcAddress(gdi32Lib, "SetPixelFormat");
	wglCreateC = (wglCreateCont) GetProcAddress(opengl32Lib, "wglCreateContext");
	wglMakeC = (wglMakeCurr) GetProcAddress(opengl32Lib, "wglMakeCurrent");
	glViewp = (glViewpor) GetProcAddress(opengl32Lib, "glViewport");
	glClearC = (glClearCol) GetProcAddress(opengl32Lib, "glClearColor");
	glC = (glCle) GetProcAddress(opengl32Lib, "glClear");
	SwapB = (SwapBuff) GetProcAddress(gdi32Lib, "SwapBuffers");
	glC3f = (glCol3f) GetProcAddress(opengl32Lib, "glColor3f");
	glB = (glBeg) GetProcAddress(opengl32Lib, "glBegin");
	glE = (glEn) GetProcAddress(opengl32Lib, "glEnd");
	glV2f = (glVert2f) GetProcAddress(opengl32Lib, "glVertex2f");
	glLineW = (glLineWid) GetProcAddress(opengl32Lib, "glLineWidth");
	canvas.output_handler = GetStdHandle(STD_OUTPUT_HANDLE);
	canvas.input_handler = GetStdHandle(STD_INPUT_HANDLE);
	if (!ChoosePixelF || !SetPixelF || !wglCreateC || !wglMakeC || !glViewp || !glClearC || !SwapB || !glC3f || !glB || !glE || !glV2f || !glLineW) return -1;
	setUp(); //Tomo las coordenadas y handlers que el usuario quiera especificar
	startWindow(); //Seteo la ventana
	ShowWindow(canvas.window_handle,1);
	LPVOID variable;
	DWORD thread_id;
	HANDLE second_process = CreateThread(NULL,0,background_render,nullptr,0,&thread_id); //Creo el thread encargado de ejecutar la función loop del usuario y actualizar la pantalla
	
	while(GetMessage(&canvas.Msg, NULL, 0, 0) > 0) //Bucle de mensajes de la ventana (manda a ejecutar windowProcess) 
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
	canvas.window_class.lpszClassName = TEXT("CanvasGL");
	RegisterClassEx(&canvas.window_class);
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_BORDER | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	canvas.window_handle = CreateWindowEx(dwExStyle,TEXT("CanvasGL"),TEXT("UTN FRSF - Canvas"),dwStyle,0,0, canvas.width, canvas.height,NULL, NULL, GetModuleHandle(nullptr), NULL);
	canvas.pfd = {sizeof(PIXELFORMATDESCRIPTOR),1,PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,32,  0, 0, 0, 0, 0, 0,0,0,0,0, 0, 0, 0,24,8,0,PFD_MAIN_PLANE,0,0, 0,0};
	canvas.device_context = GetDC(canvas.window_handle);
	canvas.pixel_format_number = ChoosePixelF(canvas.device_context,&canvas.pfd);
	SetPixelF(canvas.device_context,canvas.pixel_format_number,&canvas.pfd);
}

void setContext()
{
	canvas.opengl_context = wglCreateC(canvas.device_context);
	wglMakeC(canvas.device_context,canvas.opengl_context);
}

void updateViewPort()
{	
	RECT client_screen;
	GetClientRect(canvas.window_handle,&client_screen);
	canvas.width = client_screen.right - client_screen.left;
	canvas.height = client_screen.bottom - client_screen.top;
	glViewp(0,0,client_screen.right,client_screen.bottom);	
}

LRESULT CALLBACK windowProcess(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BYTE array[256];
	POINT point;
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(window_handle);
		return WS_MINIMIZEBOX;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		GetKeyboardState(array);
		for (int i = 0; i < 256; i++) if (array[i] > 127 && !canvas.key_pressed[i])
		{
			canvas.key_pressed[i] = 1;
			canvas.onKeyDown(i);
		}
		break;
	case WM_KEYUP:
		GetKeyboardState(array);
		for (int i = 0; i < 256; i++) if (array[i] < 127 && canvas.key_pressed[i])
		{
			canvas.key_pressed[i] = 0;
			canvas.onKeyUp(i);
		}
		break;
	case WM_LBUTTONDOWN:
		GetCursorPos(&point);
		canvas.onLeftClickDown(point.x,point.y);
		break;
	case WM_RBUTTONDOWN:
		GetCursorPos(&point);
		canvas.onRightClickDown(point.x,point.y);
		break;
	case WM_LBUTTONUP:
		GetCursorPos(&point);
		canvas.onLeftClickUp(point.x,point.y);
		break;
	case WM_RBUTTONUP:
		GetCursorPos(&point);
		canvas.onRightClickUp(point.x,point.y);
		break;
	case WM_EXITSIZEMOVE :
		updateViewPort();
		canvas.onResizing();
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

/*background_render -> Función ejecutada por un hilo secundario, encargada de llamar a las instrucciones de dibujo
definidas por el usuario. Además también calcula el tiempo de*/
DWORD WINAPI background_render(LPVOID null)
{
	setContext();
	RECT client_screen;
	GetClientRect(canvas.window_handle,&client_screen);
	glViewp(0,0,client_screen.right,client_screen.bottom);
	glClearC(0,0,0,1.0f);
	glC(GL_COLOR_BUFFER_BIT);	
	auto start = std::chrono::high_resolution_clock::now(); //Tomo el tiempo de inicio de dibujado
	SwapB(canvas.device_context); //Esta función basicamente me imprime en pantalla lo dibujado
	while(1)
	{
		auto finish = std::chrono::high_resolution_clock::now(); //Tomo el tiempo de finalización de dibujado
		std::chrono::duration<double> elapsed = finish - start;
		start = finish; //Vuelvo a tomar el tiempo de inicio
		float time = elapsed.count();					
		if (SetWindowTextA(canvas.window_handle,("UTN FRSF - ALGORITMOS - FPS: " + intToString( (int) (1.0/time)) ).c_str()));
		else break;
		updateViewPort();
		if (!loop(time)) break; //Ejecuto el programa
		SwapB(canvas.device_context); //Imprimo lo ejecutado en el programa
		glC(GL_COLOR_BUFFER_BIT); //Limpio la pantalla
	};
	/*Retorno esto porque el metodo CreateThread me pide que tenga este tipo de retorno.
	No lo uso para nada*/
	DWORD response;
	return response;
}

/*
AREA DE CODIGO : INSTRUCCIONES DE DIBUJO
Tips:
	#La función glB y glE me permiten conectar vectores de diferentes maneras,
		según el parametro que le pases a glB();
	#glV2f te permite definir un vector/punto en el espacio cuyas dimensiones deben estár en ([-1,1],[-1,1]).
		Siendo que x=1 es el extremo derecho de la pantalla, y x=-1 es el extremo izquierdo (De manera analoga con y).
	#glPointSize te permite definir el tamaño del punto en pixeles.
	#glLineW te permite definir el ancho del vector trazado en pixeles (No estoy seguro si en pixeles).
*/
void drawLine(double x0, double y0, double x1, double y1, int color = 0xffffff)
{
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glC3f(red,green,blue);
	glB(GL_LINES);
	x0 = (((x0/canvas.width)*2)-1);
	y0 = (((y0/canvas.height)*2)-1);
	x1 = (((x1/canvas.width)*2)-1);
	y1 = (((y1/canvas.height)*2)-1);(x0,y0);(x1,y1);
	glE();
}

void drawLineByAngle(int x0, int y0, double angle, double length, int color = 0xffffff)
{    
	angle = (angle / 180) * PI;
	drawLine(x0,y0,x0 + (int) (cos(angle)*length), y0 + (int) (sin(angle)*length), color);
}

void drawCircle(double cx, double cy, double r, int num_segments = 0) 
{ 
	if (num_segments == 0) num_segments = (10.0 * sqrtf(r))/2;
	float theta = 2 * 3.1415926 / float(num_segments); 
	float tangetial_factor = tanf(theta);
	float radial_factor = cosf(theta);
	float x = r;
	float y = 0; 
    glC3f(1.0,1.0,1.0);
	glB(GL_LINE_LOOP); 
	for(int ii = 0; ii < num_segments; ii++) 
	{
		glV2f(((((x+cx)/canvas.width)*2)-1),((((y+cy)/canvas.height)*2)-1));
		float tx = -y; 
		float ty = x;
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glE();
}

void drawSemiCircle(double x0, double y0, double radius, double from, double to, int color = 0xffffff)
{
	const float DEG2RAD = 3.14159/180;
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glC3f(red,green,blue);
	glB(GL_LINE_STRIP);
	int i_add = 1;
	if (from >= to) i_add = -1; 
	for (int i=from; i != to; i += i_add)
	{
		float degInRad = i*DEG2RAD;
		double x = (x0+cos(degInRad)*radius);
		double y = (y0+sin(degInRad)*radius);
		glV2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	}
	glE();
}

void fillSemiCircle(double x0, double y0, double radius, double from, double to, int color = 0xffffff)
{
	const float DEG2RAD = 3.14159/180;
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glC3f(red,green,blue);
	glB(GL_POLYGON);
	int i_add = 1;
	if (from >= to) i_add = -1; 
	for (int i=from; i != to; i += i_add)
	{
		float degInRad = i*DEG2RAD;
		double x = (x0+cos(degInRad)*radius);
		double y = (y0+sin(degInRad)*radius);
		glV2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	}
	glE();
}

void fillCircle(double x0, double y0, double radius, int num_segments = 0, int color = 0xffffff)
{    
	if (num_segments == 0) num_segments = (10.0 * sqrtf(radius))/2;
	double theta = 2 * 3.1415926 / double(num_segments);
	double tangetial_factor = tanf(theta);
	double radial_factor = cosf(theta);
	double x = radius;
	double y = 0;
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
    glC3f(red,green,blue);
	glLineW(1.0);
	glB(GL_POLYGON);
	for(int ii = 0; ii < num_segments; ii++)
	{
(((((x+x0)/canvas.width)*2)-1),((((y+y0)/canvas.height)*2)-1));
		float tx = -y;
		float ty = x;
		x += tx * tangetial_factor;
		y += ty * tangetial_factor;
		x *= radial_factor;
		y *= radial_factor;
	}
	glE();
}

void drawRect(double x, double y, double width, double height, int color = 0xffffff)
{   
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glB(GL_LINE_LOOP);
	glC3f(red,green,blue);
	glV2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glV2f(((((x+width)/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glV2f(((((x+width)/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glV2f((((x/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glE();
}

void fillRect(double x, double y, double width, double height, int color = 0xffffff)
{    
	glB(GL_POLYGON);
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glC3f(red,green,blue);
	glV2f((((x/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glV2f(((((x+width)/canvas.width)*2)-1),(((y/canvas.height)*2)-1));
	glV2f(((((x+width)/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glV2f((((x/canvas.width)*2)-1),((((y+height)/canvas.height)*2)-1));
	glE();
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


void write(std::string text, float x0, float y0, float size, bool centered = false , int color = 0xffffff)
{
	double width = 3*size;
	double height = 4*size;
	float x;
	float red = (((color/256)/256)%256)/255.0;
	float green = ((color/256)%256)/255.0;
	float blue = (color%256)/255.0;
	glC3f(red,green,blue);
	if (!centered)
	{
		x0 = (((x0/canvas.width)*2)-1);
		x = x0;
		y0 = (((y0/canvas.height)*2)-1);
	}else{
		x0 -= (width-1*size)*text.size()/2.0;
		x = x0;
		x0 = (((x0/canvas.width)*2)-1);
		y0 = (((y0/canvas.height)*2)-1);
	}
	width = (width/canvas.width);
	height = (height/canvas.height);
	for (int letter = 0; letter < text.size(); letter++)
	{
		glB(GL_LINE_STRIP);
		switch (text[letter])
		{
			case 'a':
			case 'A':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width,y0+height);
				glV2f(x0+width,y0);
				glV2f(x0+width,y0+height*0.6);
				glV2f(x0,y0+height*0.6);
				x0 += width+0.002*size;
				break;
			case 'b':
			case 'B':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width*0.7,y0+height);
				glV2f(x0+width*0.7,y0+height*0.6);
				glV2f(x0,y0+height*0.6);
				glV2f(x0+width,y0+height*0.6);
				glV2f(x0+width,y0);
				glV2f(x0,y0);
				x0 += width+0.002*size;
				break;
			case 'c':
			case 'C':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width,y0+height);
				glV2f(x0,y0+height);
				glV2f(x0,y0);
				glV2f(x0+width,y0);
				x0 += width+0.002*size;
				break;
			case 'd':
			case 'D':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width*0.8,y0+height);
				glV2f(x0+width,y0+height*0.8);
				glV2f(x0+width,y0+height*0.2);
				glV2f(x0+width*0.8,y0);
				glV2f(x0,y0);
				x0 += width+0.002*size;
				break;
			case 'e':
			case 'E':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width,y0+height);
				glV2f(x0,y0+height);
				glV2f(x0,y0+height/2);
				glV2f(x0+width,y0+height/2);
				glV2f(x0,y0+height/2);
				glV2f(x0,y0);
				glV2f(x0+width,y0);
				x0 += width+0.002*size;
				break;
			case 'f':
			case 'F':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width,y0+height);
				glV2f(x0,y0+height);
				glV2f(x0,y0+height/2);
				glV2f(x0+width,y0+height/2);
				x0 += width+0.002*size;
				break;
			case 'g':
			case 'G':
				glV2f(x0+width*0.6,y0+height*0.6);
				glV2f(x0+width,y0+height*0.6);
				glV2f(x0+width,y0+height*0.2);
				glV2f(x0+width*0.8,y0);
				glV2f(x0+width*0.2,y0);
				glV2f(x0,y0+height*0.2);
				glV2f(x0,y0+height*0.8);
				glV2f(x0+width*0.2,y0+height);
				glV2f(x0+width,y0+height);
				x0 += width+0.002*size;
				break;
			case 'h':
			case 'H':
				glV2f(x0,y0+height);
				glV2f(x0,y0+height/2);
				glV2f(x0+width,y0+height/2);
				glV2f(x0+width,y0+height);
				glV2f(x0+width,y0);
				glV2f(x0+width,y0+height/2);
				glV2f(x0,y0+height/2);
				glV2f(x0,y0);
				x0 += width+0.002*size;
				break;
			case 'i':
			case 'I':
				glV2f(x0,y0);
				glV2f(x0+width/4,y0);
				glV2f(x0+width/4,y0+height);
				glV2f(x0,y0+height);
				glV2f(x0+width/2,y0+height);
				glV2f(x0+width/4,y0+height);
				glV2f(x0+width/4,y0);
				glV2f(x0+width/2,y0);
				x0 += width/2+0.002*size;
				break;
			case 'j':
			case 'J':
				glV2f(x0,y0+height);
				glV2f(x0+width/2,y0+height);
				glV2f(x0+width/2,y0+height*0.3);
				glV2f(x0+width/3,y0);
				glV2f(x0,y0);
				glV2f(x0,y0+height*0.3);
				x0 += width/2+0.002*size;
				break;
			case 'k':
			case 'K':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0,y0+height/2);
				glV2f(x0+width,y0+height);
				glV2f(x0,y0+height/2);
				glV2f(x0+width,y0);
				x0 += width+0.002*size;
				break;
			case 'l':
			case 'L':
				glV2f(x0,y0+height);
				glV2f(x0,y0);
				glV2f(x0+width,y0);
				x0 += width+0.002*size;
				break;
			case 'm':
			case 'M':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width/2,y0+height*0.6);
				glV2f(x0+width,y0+height);
				glV2f(x0+width,y0);
				x0 += width+0.002*size;
				break;
			case 'n':
			case 'N':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width,y0);
				glV2f(x0+width,y0+height);
				x0 += width+0.002*size;
				break;
			case 'o':
			case 'O':
				glV2f(x0,y0+height*0.2);
				glV2f(x0,y0+height*0.8);
				glV2f(x0+width*0.2,y0+height);
				glV2f(x0+width*0.8,y0+height);
				glV2f(x0+width,y0+height*0.8);
				glV2f(x0+width,y0+height*0.2);
				glV2f(x0+width*0.8,y0);
				glV2f(x0+width*0.2,y0);
				glV2f(x0,y0+height*0.2);
				x0 += width+0.002*size;
				break;
			case 'p':
			case 'P':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width*0.9,y0+height);
				glV2f(x0+width,y0+height*0.9);
				glV2f(x0+width,y0+height*0.6);
				glV2f(x0+width*0.9,y0+height*0.5);
				glV2f(x0,y0+height*0.5);
				x0 += width+0.002*size;
				break;
			case 'q':
			case 'Q':
				glV2f(x0,y0+height*0.2);
				glV2f(x0,y0+height*0.8);
				glV2f(x0+width*0.2,y0+height);
				glV2f(x0+width*0.8,y0+height);
				glV2f(x0+width,y0+height*0.8);
				glV2f(x0+width,y0+height*0.2);
				glV2f(x0+width*0.9,y0+height*0.1);
				glV2f(x0+width*0.6,y0+height*0.3);
				glV2f(x0+width,y0);
				glV2f(x0+width*0.9,y0+height*0.1);
				glV2f(x0+width*0.8,y0);
				glV2f(x0+width*0.2,y0);
				glV2f(x0,y0+height*0.2);
				x0 += width+0.002*size;
				break;
			case 'r':
			case 'R':
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				glV2f(x0+width*0.9,y0+height);
				glV2f(x0+width,y0+height*0.9);
				glV2f(x0+width,y0+height*0.6);
				glV2f(x0+width*0.9,y0+height*0.5);
				glV2f(x0+width,y0);
				glV2f(x0+width*0.9,y0+height*0.5);
				glV2f(x0,y0+height*0.5);
				x0 += width+0.002*size;
				break;
			case 's':
			case 'S':
				glV2f(x0+width,y0+height);
				glV2f(x0+width*0.1,y0+height);
				glV2f(x0,y0+height*0.9);
				glV2f(x0,y0+height*0.55);
				glV2f(x0+width*0.1,y0+height*0.50);
				glV2f(x0+width*0.9,y0+height*0.50);
				glV2f(x0+width,y0+height*0.45);
				glV2f(x0+width,y0+height*0.1);
				glV2f(x0+width*0.9,y0);
				glV2f(x0,y0);
				x0 += width+0.002*size;
				break;
			case 't':
			case 'T':
				glV2f(x0,y0+height);
				glV2f(x0+width*0.5,y0+height);
				glV2f(x0+width*0.5,y0);
				glV2f(x0+width*0.5,y0+height);
				glV2f(x0+width,y0+height);
				x0 += width+0.002*size;
				break;
			case 'u':
			case 'U':
				glV2f(x0+width,y0+height);
				glV2f(x0+width,y0+height*0.2);
				glV2f(x0+width*0.8,y0);
				glV2f(x0+width*0.2,y0);
				glV2f(x0,y0+height*0.2);
				glV2f(x0,y0+height);
				x0 += width+0.002*size;
				break;
			case 'v':
			case 'V':
				glV2f(x0+width,y0+height);
				glV2f(x0+width/2,y0);
				glV2f(x0,y0+height);
				x0 += width+0.002*size;
				break;
			case 'w':
			case 'W':
				glV2f(x0+width,y0+height);
				glV2f(x0+width,y0);
				glV2f(x0+width/2,y0+height*0.6);
				glV2f(x0,y0);
				glV2f(x0,y0+height);
				x0 += width+0.002*size;
				break;
			case 'x':
			case 'X':
				glV2f(x0+width,y0+height);
				glV2f(x0+width/2,y0+height/2);
				glV2f(x0,y0+height);
				glV2f(x0+width,y0);
				glV2f(x0+width/2,y0+height/2);
				glV2f(x0,y0);
				x0 += width+0.002*size;
				break;
			case 'y':
			case 'Y':
				glV2f(x0+width/2,y0);
				glV2f(x0+width/2,y0+height*0.6);
				glV2f(x0+width,y0+height);
				glV2f(x0+width/2,y0+height*0.6);
				glV2f(x0,y0+height);
				x0 += width+0.002*size;
				break;
			case 'z':
			case 'Z':
				glV2f(x0,y0+height);
				glV2f(x0+width,y0+height);
				glV2f(x0,y0);
				glV2f(x0+width,y0);
				x0 += width+0.002*size;
				break;
			case '0':
				glV2f(x0,y0+height*0.2);
				glV2f(x0,y0+height*0.8);
				glV2f(x0+width*0.2,y0+height);
				glV2f(x0+width*0.8,y0+height);
				glV2f(x0+width,y0+height*0.8);
				glV2f(x0,y0+height*0.2);
				glV2f(x0+width,y0+height*0.8);
				glV2f(x0+width,y0+height*0.2);
				glV2f(x0+width*0.8,y0);
				glV2f(x0+width*0.2,y0);
				glV2f(x0,y0+height*0.2);
				x0 += width+0.002*size;
				break;
			case '1':
				glV2f(x0,y0+height*0.6);
				glV2f(x0+width*0.4,y0+height);
				glV2f(x0+width*0.4,y0);
				x0 += width*0.4+0.002*size;
				break;
			case '2':
				glV2f(x0,y0+height*0.8);
				glV2f(x0+width*0.2,y0+height);
				glV2f(x0+width*0.8,y0+height);
				glV2f(x0+width,y0+height*0.8);
				glV2f(x0+width,y0+height*0.7);
				glV2f(x0,y0);
				glV2f(x0+width,y0);
				x0 += width+0.002*size;
				break;
			case '3':
				glV2f(x0,y0+height*0.8);
				glV2f(x0+width*0.1,y0+height);
				glV2f(x0+width*0.9,y0+height);
				glV2f(x0+width,y0+height*0.9);
				glV2f(x0+width,y0+height*0.5);
				glV2f(x0+width*0.5,y0+height*0.5);
				glV2f(x0+width,y0+height*0.5);
				glV2f(x0+width,y0+height*0.1);
				glV2f(x0+width*0.9,y0);
				glV2f(x0+width*0.2,y0);
				glV2f(x0,y0+height*0.2);
				x0 += width+0.002*size;
				break;
			case '4':
				glV2f(x0+width*0.6,y0);
				glV2f(x0+width*0.6,y0+height);
				glV2f(x0,y0+height*0.3);
				glV2f(x0+width,y0+height*0.3);
				x0 += width+0.002*size;
				break;
			case '5':
				glV2f(x0+width,y0+height);
				glV2f(x0,y0+height);
				glV2f(x0,y0+height*0.5);
				glV2f(x0+width*0.9,y0+height*0.5);
				glV2f(x0+width,y0+height*0.4);
				glV2f(x0+width,y0+height*0.1);
				glV2f(x0+width*0.9,y0);
				glV2f(x0,y0);
				x0 += width+0.002*size;
				break;
			case '6':
				glV2f(x0+width,y0+height);
				glV2f(x0+width*0.2,y0+height);
				glV2f(x0,y0+height*0.8);
				glV2f(x0,y0+height*0.5);
				glV2f(x0+width*0.9,y0+height*0.5);
				glV2f(x0+width,y0+height*0.4);
				glV2f(x0+width,y0+height*0.1);
				glV2f(x0+width*0.9,y0);
				glV2f(x0+width*0.2,y0);
				glV2f(x0,y0+height*0.2);
				glV2f(x0,y0+height*0.5);
				x0 += width+0.002*size;
				break;
			case '7':
				glV2f(x0,y0+height);
				glV2f(x0+width,y0+height);
				glV2f(x0,y0);
				x0 += width+0.002*size;
				break;
			case '8':
				glV2f(x0+width*0.1,y0+height);
				glV2f(x0+width*0.9,y0+height);
				glV2f(x0+width,y0+height*0.9);
				glV2f(x0+width,y0+height*0.6);
				glV2f(x0+width*0.9,y0+height*0.5);
				glV2f(x0+width,y0+height*0.4);
				glV2f(x0+width,y0+height*0.1);
				glV2f(x0+width*0.9,y0);
				glV2f(x0+width*0.1,y0);
				glV2f(x0,y0+height*0.1);
				glV2f(x0,y0+height*0.4);
				glV2f(x0+width*0.1,y0+height*0.5);
				glV2f(x0+width*0.9,y0+height*0.5);
				glV2f(x0+width*0.1,y0+height*0.5);
				glV2f(x0,y0+height*0.6);
				glV2f(x0,y0+height*0.9);
				glV2f(x0+width*0.1,y0+height);
				x0 += width+0.002*size;
				break;
			case '9':
				glV2f(x0+width,y0+height*0.5);
				glV2f(x0+width*0.1,y0+height*0.5);
				glV2f(x0,y0+height*0.6);
				glV2f(x0,y0+height*0.9);
				glV2f(x0+width*0.1,y0+height);
				glV2f(x0+width*0.9,y0+height);
				glV2f(x0+width,y0+height*0.9);
				glV2f(x0+width,y0);
				x0 += width+0.002*size;
				break;
			case '|':
				glV2f(x0,y0);
				glV2f(x0,y0+height);				
				x0 += width*0.2+0.002*size;
				break;
			case ':':
				glV2f(x0,y0);
				glV2f(x0+width*0.1,y0);
				glV2f(x0+width*0.1,y0+height*0.1);
				glV2f(x0,y0+height*0.1);
				glV2f(x0,y0);
				glE();
				glB(GL_LINE_STRIP);
				glV2f(x0,y0+height*0.8);
				glV2f(x0+width*0.1,y0+height*0.8);
				glV2f(x0+width*0.1,y0+height*0.1+height*0.8);
				glV2f(x0,y0+height*0.1+height*0.8);
				glV2f(x0,y0+height*0.8);
				x0 += width*0.2+0.002*size;
				break;
			case ' ':
				x0 += width*0.7+0.002*size;
				break;
			case '.':
				glV2f(x0,y0);
				glV2f(x0+width*0.1,y0);
				glV2f(x0+width*0.1,y0+height*0.1);
				glV2f(x0,y0+height*0.1);
				glV2f(x0,y0);
				x0 += width*0.1+0.002*size;
				break;
			case '!':
				glV2f(x0,y0);
				glV2f(x0+width*0.1,y0);
				glV2f(x0+width*0.1,y0+height*0.1);
				glV2f(x0,y0+height*0.1);
				glV2f(x0,y0);
				glE();
				glB(GL_LINE_STRIP);
				glV2f(x0+width*0.05,y0+height*0.2);
				glV2f(x0+width*0.05,y0+height);
				x0 += width*0.1+0.002*size;
				break;
			case '*':
				glV2f(x0+width*0.4,y0+height*0.4);
				glV2f(x0+width*0.4,y0+height);
				glV2f(x0+width*0.4,y0+height*0.7);
				glV2f(x0,y0+height);
				glV2f(x0+width*0.8,y0+height*0.4);
				glV2f(x0+width*0.4,y0+height*0.7);
				glV2f(x0,y0+height*0.4);
				glV2f(x0+width*0.8,y0+height);
				glV2f(x0+width*0.4,y0+height*0.7);
				glV2f(x0+width*0.8,y0+height*0.7);
				glV2f(x0+width*0,y0+height*0.7);
				x0 += width*0.8+0.002*size;
				break;			
		}
		glE();
	}
}

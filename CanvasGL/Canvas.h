#define _WIN32_WINNT 0x0601
#define PI 3.141592653
#include <windows.h>
#include <chrono>
#include "tools.h"
#include <math.h>
#include <GL/gl.h>
#include <iostream>

/* Para poder compilar con minGW debe agregarse "-l gdi32 -l opengl32"*/

class Canvas {

private:

	HANDLE output_handler = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE input_handler = GetStdHandle(STD_INPUT_HANDLE);
    HGLRC opengl_context;
    MSG Msg;
    WNDCLASSEX window_class;
    PIXELFORMATDESCRIPTOR pfd;
    int pixel_format_number;
    void addColor(int hex);
    void startWindow();
    static LRESULT CALLBACK windowProcess(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam);
    void findColor(int hex,int color[2], HANDLE hStdout);
    int pallet_size = 1;

public:

	int width;
	int height;
    int pixel_x_size;
    int pixel_y_size;
    HWND window_handle;
    HDC device_context;
	CHAR_INFO* buffer;
	SMALL_RECT window;

    void render();
    void setContext();
    void drawCircle(double x_center, double y_center, double radius, int color);
    void fillCircle(double x_center, double y_center, double radius, int color);
    void drawLineByAngle(int x0, int y0, double angle, double length, int color);
    void drawLine(double x1, double y1, double x2, double y2, int color);
    void drawRect(int x, int y, int height, int width, int color);
    void fillRect(int x, int y, int height, int width, int color);
    void drawPixel(double x, double y, int color);
    virtual int loop(double time);
    int start(int width, int height);
    void clear();

};

#include "Canvas.cpp"

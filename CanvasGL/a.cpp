#include <windows.h>
#include <wingdi.h>
#include <iostream>
#include <GL/gl.h>
#include <gdiplus.h>

typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
static wglSwapInterval_t *wglSwapInterval;

/* Para poder compilar con minGW debe agregarse "-l gdi32 -l opengl32"*/

HDC device_context;
HWND window_handle;
HGLRC opengl_context;
MSG Msg;
WNDCLASSEX window_class;
int pixel_format_number;
PIXELFORMATDESCRIPTOR pfd;

const char g_szClassName[] = "CanvasGL";

LRESULT CALLBACK WndProc(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam)
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

void startWindowContext()
{
    //Step 1: Registering the Window Class
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = WndProc;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = GetModuleHandle(nullptr);
    window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    window_class.lpszMenuName = NULL;
    window_class.lpszClassName = g_szClassName;
    window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&window_class);

    window_handle = CreateWindowEx(WS_EX_CLIENTEDGE,g_szClassName,"UTN FRSF - Canvas",WS_OVERLAPPEDWINDOW,0,0, 640, 450,NULL, NULL, GetModuleHandle(nullptr), NULL);

	pfd = {sizeof(PIXELFORMATDESCRIPTOR),1,PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,32,  0, 0, 0, 0, 0, 0,0,0,0,0, 0, 0, 0,24,8,0,PFD_MAIN_PLANE,0,0, 0, 0};

	device_context = GetDC(window_handle);
}

void setContext()
{
	pixel_format_number = ChoosePixelFormat(device_context,&pfd);
	SetPixelFormat(device_context,pixel_format_number,&pfd);
    opengl_context = wglCreateContext(device_context);
    wglMakeCurrent(device_context,opengl_context);
}

int main()
{
    startWindowContext();
    setContext();
    glEnable(GL_TEXTURE_2D);
    ShowWindow(window_handle,5);
    glViewport(0,0,640,480);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        glViewport(0, 0, 640, 480);
        glClearColor(0.0f,0,0,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1,1,0);
        glLineWidth(3);
        glRecti(0,0,0,0);
        SwapBuffers(device_context);
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

/* OLD

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	int pixel_format_number;
	if (!(pixel_format_number = ChoosePixelFormat(hdc,&pfd)))
	{
		MessageBox(NULL, "No se pudo machear el formato de pixel!", "Error!", MB_ICONEXCLAMATION | MB_OK);
	};

	if (!SetPixelFormat(hdc,pixel_format_number,&pfd))
	{
		MessageBox(NULL, "No se pudo setear el formato de pixel!", "Error!", MB_ICONEXCLAMATION | MB_OK);
	}

	HGLRC hglrc;
	if ((hglrc = wglCreateContext(hdc)) == nullptr)
	{
		MessageBox(NULL, "No se pudo crear el contexto!", "Error!", MB_ICONEXCLAMATION | MB_OK);
	}

	ShowWindow(hwnd, nCmdShow);

	std::cout << wglMakeCurrent(hdc,hglrc);

	glViewport(0, 0, 400, 400);
	// glEnable(GL_TEXTURE_2D);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	wglSwapInterval = (wglSwapInterval_t*) wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapInterval) wglSwapInterval(0);
	glClearColor(1.0f,0,0,1.0f);

	while (1)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		// Remove Frame cap
		glViewport(0, 0, 400, 400);

		glBegin(GL_LINE);
			glVertex2f(0.0f,0.0f);
			glVertex2f(0.5f,0.5f);
		glEnd();

		SwapBuffers(hdc);

		UpdateWindow(hwnd);
	}

    // Step 3: The Message Loop
    // while(GetMessage(&Msg, NULL, 0, 0) > 0)
    // {
    //     TranslateMessage(&Msg);
    //     DispatchMessage(&Msg);
    // }
    return Msg.wParam;
}

*/
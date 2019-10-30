#include <windows.h>
#include <wingdi.h>
#include <iostream>
#include <GL/gl.h>
#include <gdiplus.h>

typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
static wglSwapInterval_t *wglSwapInterval;

/* Para poder compilar con minGW debe agregarse "-l gdi32 -l opengl32"*/

const char g_szClassName[] = "myWindowClass";

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(nullptr);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "UTN FRSF - Canvas",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CS_OWNDC, 400, 400,
        NULL, NULL, GetModuleHandle(nullptr), NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	HDC hdc = GetDC(hwnd);

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

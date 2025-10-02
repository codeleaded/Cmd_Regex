#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include "Stroke.h"

long long Time_Nano() {
    LARGE_INTEGER counter, frequency;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&frequency);
    return (long long)((counter.QuadPart * 1000000000ULL) / frequency.QuadPart);
}

typedef struct YanisWindow {
    char Name[32];
    unsigned int* Buffer;
    unsigned long long LastTime;
    double ElapsedTime;
    int Width;
    int Height;
    int MouseX;
    int MouseY;
    int Focus;
    int Running;
    States Strokes[MAX_STROKES];
    void (*setup)(struct YanisWindow*);
    void (*update)(struct YanisWindow*);
    void (*delete)(struct YanisWindow*);
    HWND hwnd;
    HDC hdc;
    BITMAPINFO bmi;
} YanisWindow;


LRESULT CALLBACK YanisWindow_StdWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_NCCREATE:{
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            YanisWindow* w = (YanisWindow*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)w);
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }break;
        
        case WM_MOUSEMOVE:{
            YanisWindow* w = (YanisWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            w->MouseX = LOWORD(lParam);
            w->MouseY = HIWORD(lParam);
        }break;

        case WM_SIZE:{
            YanisWindow* w = (YanisWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            
            int Width = (int)LOWORD(lParam);
            int Height = (int)HIWORD(lParam);

            unsigned int* Buffer = w->Buffer;
            w->Buffer = (int*)malloc(Width * Height * sizeof(unsigned int));
            memset(w->Buffer,0,Width * Height * sizeof(unsigned int));
            free(Buffer);
            
            w->Width  = Width;
            w->Height = Height;

            ZeroMemory(&w->bmi, sizeof(BITMAPINFO));
            w->bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            w->bmi.bmiHeader.biWidth = Width;
            w->bmi.bmiHeader.biHeight = -Height;
            w->bmi.bmiHeader.biPlanes = 1;
            w->bmi.bmiHeader.biBitCount = 32;
            w->bmi.bmiHeader.biCompression = BI_RGB;
        }break;

        case WM_SETFOCUS:{
            YanisWindow* w = (YanisWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            w->Focus = 1;
        }break;

        case WM_KILLFOCUS:{
            YanisWindow* w = (YanisWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            w->Focus = 0;
        }break;

        case WM_CLOSE:{
            YanisWindow* w = (YanisWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            w->Running = 0;
        }break;

        case WM_DESTROY:{
            PostQuitMessage(0);
        }break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void YanisWindow_Init(
    YanisWindow* yw,
    char* Name,
    int Width,
    int Height,
    void (*setup)(YanisWindow*),
    void (*update)(YanisWindow*),
    void (*delete)(YanisWindow*)
){
    memset(yw->Name,0,sizeof(yw->Name));
    strcpy(yw->Name,Name);

    yw->Width = Width;
    yw->Height = Height;

    yw->setup = setup;
    yw->update = update;
    yw->delete = delete;

    yw->LastTime = 0ULL;
    yw->ElapsedTime = 0.0;
    yw->MouseX = 0;
    yw->MouseY = 0;
    yw->Focus = 0;
    yw->Running = 0;

    WNDCLASSEX wc;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = YanisWindow_StdWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = "";
    wc.lpszClassName = yw->Name;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExA(&wc)) {
        DWORD err = GetLastError();
        char buf[256];
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       buf, sizeof(buf), NULL);
        MessageBoxA(NULL, buf, "RegisterClassEx failed", MB_ICONERROR | MB_OK);
        return;
    }

    yw->hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        yw->Name,
        "Buffer Example",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,yw->Width,yw->Height,
        NULL,NULL,
        GetModuleHandle(NULL),
        yw
    );

    if (!yw->hwnd) {
        DWORD err = GetLastError();
        char buf[256];
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       buf, sizeof(buf), NULL);
        MessageBoxA(NULL, buf, "CreateWindowEx failed", MB_ICONERROR | MB_OK);
        return;
    }

    ZeroMemory(&yw->bmi, sizeof(BITMAPINFO));
    yw->bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    yw->bmi.bmiHeader.biWidth = yw->Width;
    yw->bmi.bmiHeader.biHeight = -yw->Height;
    yw->bmi.bmiHeader.biPlanes = 1;
    yw->bmi.bmiHeader.biBitCount = 32;
    yw->bmi.bmiHeader.biCompression = BI_RGB;

    yw->hdc = GetDC(yw->hwnd);
    ShowWindow(yw->hwnd,SW_SHOW);
    UpdateWindow(yw->hwnd);
}
void YanisWindow_Render(YanisWindow* yw) {
    StretchDIBits(
        yw->hdc, 
        0, 0, yw->Width, yw->Height, 
        0, 0, yw->Width, yw->Height, 
        yw->Buffer, 
        &yw->bmi, 
        DIB_RGB_COLORS, 
        SRCCOPY
    );
    UpdateWindow(yw->hwnd);
}
void YanisWindow_Update(YanisWindow* yw){
    // MSG msg;
    // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    //     if (msg.message == WM_QUIT) yw->Running = 0;
    //     TranslateMessage(&msg);
    //     DispatchMessage(&msg);
    // }
    
    // char Buffer[256];
    // GetKeyboardState(Buffer);
    
    // if(yw->Focus){
    //     for (int i = 0; i < MAX_STROKES; i++){
	//     	yw->Strokes[i].PRESSED = 0;
	//     	yw->Strokes[i].RELEASED = 0;
	//     	if (GetAsyncKeyState(i) & 0x8000){
    //             yw->Strokes[i].PRESSED = !yw->Strokes[i].DOWN;
	//     		yw->Strokes[i].DOWN = 1;
	//     	}else{
	//     		yw->Strokes[i].RELEASED = yw->Strokes[i].DOWN;
	//     		yw->Strokes[i].DOWN = 0;
	//     	}
	//     }
    // }

    yw->ElapsedTime = (double)(Time_Nano() - yw->LastTime) / 1.0E9;
    yw->LastTime = Time_Nano();
    double Fps = 1.0 / yw->ElapsedTime;
    
    char TitleBuffer[100];
    sprintf(TitleBuffer,"Yanis - %s - %4.1lf",yw->Name,Fps);
    SetWindowText(yw->hwnd,TitleBuffer);
}
void YanisWindow_Start(YanisWindow* yw){
    yw->Focus = 1;
    if(yw->setup) yw->setup(yw);
    yw->Running = 1;
    
    while(yw->Running){
        YanisWindow_Update(yw);

        yw->update(yw);

        YanisWindow_Render(yw);

        if(!yw->Focus)  Sleep(10);
        else            Sleep(1);
    }
}
void YanisWindow_Free(YanisWindow* yw){
    yw->Running = 0;
    if(yw->delete) yw->delete(yw);
    if(yw->Buffer) free(yw->Buffer);
    yw->Buffer = NULL;
    DestroyWindow(yw->hwnd);
}

void YanisWindow_Clear(YanisWindow* yw,unsigned int color){
    for(int i = 0;i<yw->Width * yw->Height;i++){
        yw->Buffer[i] = color;
    }
}

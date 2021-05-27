#include "Display.h"

display* window;
double speed;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool display::Init(display* disp, satellite* sat, planet* pla, Input* inpt)
{
    display::wnd.cbSize =           sizeof(WNDCLASSEXW);
	display::wnd.style =            CS_HREDRAW | CS_VREDRAW;
    display::wnd.lpfnWndProc =      WinProc;
    display::wnd.cbClsExtra =       0;
    display::wnd.cbWndExtra =       0;
	display::wnd.hInstance =        display::hInstance;
    display::wnd.hIcon =            NULL;
    display::wnd.hCursor =          LoadCursor(nullptr, IDC_ARROW);
    display::wnd.hbrBackground =    CreateSolidBrush(RGB(0, 0, 0));
    display::wnd.lpszMenuName =     display::szTitle;
    display::wnd.lpszClassName =    display::szTitle;
    display::wnd.hIconSm =          NULL;

	RegisterClassExW(&(display::wnd));

    display::hwnd = CreateWindowExW(
        WS_EX_LEFT | WS_EX_COMPOSITED,              // dwExStyle
        szTitle,                                    // lpClassName
        szTitle,                                    // lpWindowName
        WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,   // dwStyle
        100,                                        // x
        100,                                        // y
        display::width,                             // nWidth
        display::height,                            // nHeight
        NULL,                                       // hWndParent
        NULL,                                       // hMenu
        hInstance,                                  // hInstance
        NULL);                                      // lpParam

    if (display::hwnd == NULL)
        return false;

    window = disp;
    input = inpt;
    display::_satellite = sat;
    display::_planet = pla;
    display::input = inpt;

    ShowWindow(display::hwnd, SW_SHOWNORMAL);

    return true;
}

bool display::WinMain()
{
    while (PeekMessageA(&(display::msg), nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&(display::msg));
        DispatchMessage(&(display::msg));
    }
    RECT rect;
    rect.left = 0; rect.right = 100; rect.top = 0; rect.bottom = 800;
    //RedrawWindow(display::hwnd, NULL, NULL, RDW_INTERNALPAINT);
    InvalidateRect(display::hwnd, NULL, false);
    UpdateWindow(display::hwnd);
    return true;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC dc;

    switch (message)
    {
    case WM_CHAR:
        if (wParam == 8)
            window->input->backspace = true;
        window->input->lastchar = (char)wParam;
        break;
    case WM_LBUTTONDOWN:
        window->input->leftmouse = true;
        window->input->mousepos = MAKEPOINTS(lParam);
        window->input->mouseclick = MAKEPOINTS(lParam);
        break;
    case WM_LBUTTONUP:
        window->input->leftmouse = false;
        break;
    case WM_MOUSEMOVE:
        window->input->mousepos = MAKEPOINTS(lParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        PAINTSTRUCT paint;
        dc = BeginPaint(window->hwnd, &paint);
        if (window->inputmode)
        {
            window->input->Begin(dc);
            window->input->InputMain(&window->inputmode);
            window->input->End();
        }
        if (!window->inputmode)
        {
            window->render(dc);
        }
        EndPaint(window->hwnd, &paint);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
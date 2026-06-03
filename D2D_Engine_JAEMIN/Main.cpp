// Main.cpp
// Win32 프로그램의 진입점과 윈도우 메시지 처리를 담당합니다.
// 실제 게임 로직은 Core/GameApp으로 넘겨서 Main.cpp가 비대해지지 않게 유지합니다.

#include "framework.h"
#include "D2D_Engine_JAEMIN.h"
#include "Content/StoneGachaGame.h"
#include "Engine/Core/GameApp.h"

#include <chrono>
#include <windowsx.h>

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

namespace
{
    GameApp g_gameApp;
    StoneGachaGame g_stoneGachaGame;
    HWND g_mainWindow = nullptr;
}

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Direct2D, DirectWrite, WIC 같은 Windows 그래픽 API는 COM 초기화가 필요합니다.
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_D2DENGINEJAEMIN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        CoUninitialize();
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D2DENGINEJAEMIN));

    MSG msg{};
    auto previousTime = std::chrono::steady_clock::now();

    // PeekMessage를 사용하면 메시지가 없을 때도 게임 Update/Render를 계속 실행할 수 있습니다.
    // 그래서 타이틀 로고의 부유 애니메이션처럼 시간 기반 연출이 자연스럽게 움직입니다.
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            const auto currentTime = std::chrono::steady_clock::now();
            const std::chrono::duration<float> elapsed = currentTime - previousTime;
            previousTime = currentTime;

            g_gameApp.Update(elapsed.count());
            InvalidateRect(g_mainWindow, nullptr, FALSE);
            Sleep(1);
        }
    }

    CoUninitialize();
    return static_cast<int>(msg.wParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2DENGINEJAEMIN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_D2DENGINEJAEMIN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   RECT windowRect{ 0, 0, 960, 640 };
   AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, TRUE);

   HWND hWnd = CreateWindowW(
       szWindowClass,
       L"돌 파밍으로 돈을 모아 가챠로 성공",
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT,
       0,
       windowRect.right - windowRect.left,
       windowRect.bottom - windowRect.top,
       nullptr,
       nullptr,
       hInstance,
       nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_mainWindow = hWnd;

   if (FAILED(g_gameApp.Initialize(hWnd)))
   {
       return FALSE;
   }

   // Main.cpp는 이번 실행 파일이 어떤 컨텐츠를 올릴지만 결정합니다.
   // 이후 게임 규칙과 씬 구성은 Content/StoneGachaGame이 담당합니다.
   g_stoneGachaGame.Initialize(g_gameApp);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONDOWN:
        g_gameApp.OnMouseClick(
            static_cast<float>(GET_X_LPARAM(lParam)),
            static_cast<float>(GET_Y_LPARAM(lParam)));
        break;
    case WM_SIZE:
        g_gameApp.OnResize(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_ERASEBKGND:
        // Direct2D가 매 프레임 화면 전체를 다시 그리므로 GDI 배경 지우기를 생략해 깜빡임을 줄입니다.
        return 1;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            g_gameApp.Render();
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return static_cast<INT_PTR>(TRUE);

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return static_cast<INT_PTR>(TRUE);
        }
        break;
    }

    return static_cast<INT_PTR>(FALSE);
}

// Pong.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Pong.h"

#include <time.h>       /* time */

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HINSTANCE hInstCHD;
HINSTANCE hInstCHDP;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcCHD(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcCHDB(HWND ChWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam);

void GetTextInfoForKeyMsg(WPARAM wParam, const TCHAR* msgName, TCHAR* buf, int bufSize)
{
    static int counter = 0;
    counter++;
    _stprintf_s(buf, bufSize, _T("%s key: %d ( counter : %d)"), msgName, wParam, counter);
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PONG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PONG));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    HBRUSH brushYellow = CreateSolidBrush(RGB(255, 255, 0));

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PONG));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = brushYellow;
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PONG);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }


    RECT rc;  /// center window 

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);

    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    GetWindowRect(hWnd, &rc);

    SetWindowPos(hWnd, 0, (screenWidth - rc.right) / 2, (screenHeight - rc.bottom) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    ShowWindow(hWnd, nCmdShow);
    SetWindowPos(hWnd, 0, (screenWidth - rc.right) / 2, (screenHeight - rc.bottom) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);


    SetWindowLong(hWnd, GWL_EXSTYLE,
        GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    // Make this window 50% alpha
    SetLayeredWindowAttributes(hWnd, 0, (255 * 50) / 100, LWA_ALPHA);
    // Show this window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
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
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CREATE: {


        WNDCLASSEXW wcexCHD;

        wcexCHD.cbSize = sizeof(WNDCLASSEX);

        wcexCHD.style = CS_HREDRAW | CS_VREDRAW;
        wcexCHD.lpfnWndProc = WndProcCHD;
        wcexCHD.cbClsExtra = 0;
        wcexCHD.cbWndExtra = 0;
        wcexCHD.hInstance = hInstCHD;/// put  HINSTANCE hInstCHD;    in LINE 11
        wcexCHD.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcexCHD.hIcon = LoadIcon(hInstCHD, MAKEINTRESOURCE(IDI_PONG));
        wcexCHD.hbrBackground = (HBRUSH)COLOR_BACKGROUND + 1;
        wcexCHD.lpszMenuName = MAKEINTRESOURCEW(IDC_PONG);
        wcexCHD.lpszClassName = L"REDSQ";
        wcexCHD.hIconSm = LoadIcon(wcexCHD.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        //RegisterClassExW(&wcexCHD);

        if (!RegisterClassExW(&wcexCHD))
        {
            SetWindowText(hWnd, L"Not working classs");
        }

        HWND ChWnd = CreateWindow(L"REDSQ", szTitle,
            WS_CHILD | WS_VISIBLE,
            10, 10, 50, 20,
            hWnd, NULL, hInst, NULL);


        if (!ChWnd)
        {
            SetWindowText(hWnd, L"Not working");
        }


        RECT rc;  /// center window 

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);

        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        GetWindowRect(hWnd, &rc);

        SetWindowPos(ChWnd, 0, 10, 220, 0, 0, SWP_NOZORDER | SWP_NOSIZE);




        HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));

        WNDCLASSEXW wcexCHDP;

        wcexCHDP.cbSize = sizeof(WNDCLASSEX);

        wcexCHDP.style = CS_HREDRAW | CS_VREDRAW;
        wcexCHDP.lpfnWndProc = WndProcCHDB;
        wcexCHDP.cbClsExtra = 0;
        wcexCHDP.cbWndExtra = 0;
        wcexCHDP.hInstance = hInstCHDP;/// put  HINSTANCE hInstCHDP;    in LINE 11
        wcexCHDP.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcexCHDP.hIcon = LoadIcon(hInstCHD, MAKEINTRESOURCE(IDI_PONG));
        wcexCHDP.hbrBackground = brush;
        wcexCHDP.lpszMenuName = MAKEINTRESOURCEW(IDC_PONG);
        wcexCHDP.lpszClassName = L"REDball";
        wcexCHDP.hIconSm = LoadIcon(wcexCHD.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        //RegisterClassExW(&wcexCHD);

        if (!RegisterClassExW(&wcexCHDP))
        {
            SetWindowText(hWnd, L"Not working classs");
        }

        HWND ChWndB = CreateWindow(L"REDball", szTitle,
            WS_CHILD | WS_VISIBLE,
            10, 10, 10, 10,
            hWnd, NULL, hInst, NULL);



        HRGN ball = CreateEllipticRgn(10, 10, 1, 1);



        SetWindowRgn(ChWndB, ball, true);

        if (!ChWndB)
        {
            SetWindowText(hWnd, L"Not working");
        }




        //SetTimer(hWnd, 7, 50, NULL);


    } break;
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
        minMaxInfo->ptMaxSize.x = minMaxInfo->ptMaxTrackSize.x = 200;
        minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = 300;
        minMaxInfo->ptMinTrackSize.x = 200;  /// will not allow window to hcange size width
        minMaxInfo->ptMinTrackSize.y = 300;/// will not allow window to hcange size height

    }




    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}




LRESULT CALLBACK WndProcCHD(HWND ChWnd, UINT message, WPARAM wParam, LPARAM lParam)
{



    switch (message)
    {



    default:
        return DefWindowProc(ChWnd, message, wParam, lParam);
    }
    return 0;
}


LRESULT CALLBACK WndProcCHDB(HWND ChWnd, UINT message, WPARAM wParam, LPARAM lParam)
{



    switch (message)
    {



    default:
        return DefWindowProc(ChWnd, message, wParam, lParam);
    }
    return 0;
}
/*

BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
{
    LPRECT rcParent;
    int  idChild;

    // Retrieve the child-window identifier. Use it to set the
    // position of the child window.

    idChild = GetWindowLong(hwndChild, GWL_ID);


    // Size and position the child window.

    rcParent = (LPRECT)lParam;

    RECT rc;  /// center window

    GetWindowRect(hwndChild, &rc);



    srand(time(NULL));

    /* generate secret number between 1 and 5: */
    // int iSecret = rand() % 10 + 1;
    // int iSecret = rand() % 10 + 1;
     /*

     MoveWindow(hwndChild,
         rc.right-rc.left+ iSecret,
         10,
         10,
         10,
         TRUE);



     // Make sure the child window is visible.

     ShowWindow(hwndChild, SW_SHOW);

     return TRUE;
 }

     */



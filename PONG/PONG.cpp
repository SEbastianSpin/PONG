// Pong.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Pong.h"
#include "windowsx.h"
#include <time.h>       /* time */

#define ID_BallCHILD  100 
#define ID_PaddleCHILD  101


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HINSTANCE hInstCHD;
HINSTANCE hInstCHDP;
BOOL AreWeplaying=true;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcCHD(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcCHDB(HWND ChWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam);
BOOL CALLBACK EnumChildProcPaddle(HWND hwndChild, LPARAM lParam);



void GetTextInfoForMouseMsg(WPARAM wParam, LPARAM lParam,
     const TCHAR * msgName, TCHAR* buf, int bufSize)
     {
     short x = (short)LOWORD(lParam);
     short y = (short)HIWORD(lParam);
     _stprintf_s(buf, bufSize, _T("%s x: %d, y: %d, vk:"),
         msgName, x, y);
     
         _tcscat_s(buf, bufSize, _T(" LEFT "));
        
    
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
    HWND ChWndB;
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
        case ID_FILE_NEWGAMEF2:

            POINT mouseh;
            mouseh.x = 10;
            mouseh.y = 10;
            SetTimer(hWnd, 7, 50, NULL);
            AreWeplaying = true;
            EnumChildWindows(hWnd, EnumChildProc, (LPARAM)&mouseh);
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

        //RegisterClassExW(&wcexCHD);   ID_PaddleCHILD

        if (!RegisterClassExW(&wcexCHD))
        {
            SetWindowText(hWnd, L"Not working classs");
        }

        HWND ChWnd = CreateWindow(L"REDSQ", szTitle,
            WS_CHILD | WS_VISIBLE,
            10, 10, 50, 20,
            hWnd, (HMENU)(int)(ID_PaddleCHILD), hInst, NULL);


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

         ChWndB = CreateWindow(L"REDball", szTitle,
            WS_CHILD | WS_VISIBLE,
            10, 10, 10, 10,
            hWnd, (HMENU)(int)(ID_BallCHILD), hInst, NULL);



        HRGN ball = CreateEllipticRgn(10, 10, 1, 1);



        SetWindowRgn(ChWndB, ball, true);

        if (!ChWndB)
        {
            SetWindowText(hWnd, L"Not working");
        }



        TRACKMOUSEEVENT paddle;
        paddle.cbSize = sizeof(TRACKMOUSEEVENT);
        paddle.dwFlags = TME_HOVER;
        paddle.hwndTrack = hWnd;
        paddle.dwHoverTime = HOVER_DEFAULT;


        
       
      if(!TrackMouseEvent(&paddle)) SetWindowText(hWnd, L"Not working TrackMouseEvent");

       




        SetTimer(hWnd, 7, 50, NULL);



    } break;

    case WM_MOUSEHOVER:
    {
    case WM_MOUSEMOVE:
    {
        
        POINT mouseh;
        mouseh.x= GET_X_LPARAM(lParam);
        mouseh.y = GET_Y_LPARAM(lParam);

        EnumChildWindows(hWnd, EnumChildProcPaddle, (LPARAM)&mouseh);
        
    }
    break;

    }
    break;
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
        minMaxInfo->ptMaxSize.x = minMaxInfo->ptMaxTrackSize.x = 200;  //200
        minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = 300; // 300
        minMaxInfo->ptMinTrackSize.x = 200;  /// will not allow window to hcange size width
        minMaxInfo->ptMinTrackSize.y = 300;/// will not allow window to hcange size height

    }
    break;

    case WM_TIMER:
    {
        if (wParam == 7) {
            RECT rcClient;
            GetClientRect(hWnd, &rcClient);
            EnumChildWindows(hWnd, EnumChildProc, NULL);
        
        }
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


BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
{
    //LPRECT rcParent;
    int i, idChild;

    // Retrieve the child-window identifier. Use it to set the 
    // position of the child window. 

    idChild = GetWindowLong(hwndChild, GWL_ID);
    POINT paddlepos;
    GetCursorPos(&paddlepos);
     

    HWND daddy= GetParent(hwndChild);
   
    static int x = 10;
    static int y = 10;
    static int lx = 0;
    static int rx = 20;
    static int ty = 0;
    static int by = 20;


    if (lParam!=NULL) {
        LPPOINT ball = (LPPOINT)lParam;
        x = ball->x;
        y = ball->y;
        lx = 0;
        rx = 20;
        ty = 0;
         by = 20;
    }
    
    if (rx >= 190)
        x = -abs(x);
    if (lx <= 0)
        x = abs(x);
    if (by >= 235)
        y = -abs(y);
    if (ty <= 0)
        y = abs(y);



    //SetCursorPos(100, 10);
   
   



    ScreenToClient(daddy, &paddlepos);

    TCHAR s[256];

    _stprintf_s(s, 256, _T(" Paddle posx: %d mouse posx %d y %d "), paddlepos.x, lx ,ty);
    //SetWindowText(daddy, s);

   
    if (paddlepos.x +25 >= lx && paddlepos.x - 25 <= lx && ty>=200) {
       // SetWindowText(daddy, L"COLLI");
        y = -abs(y);

    }
    

    if (ty >= 210) { /// losgin
        // SetWindowText(daddy, L"COLLI");
        KillTimer(daddy, 7);
        AreWeplaying = false;

    }

    lx = lx + x;
    ty = ty + y;
    rx = rx + x;
    by = by + y;

    if (idChild == ID_BallCHILD) {
    
        

        MoveWindow(hwndChild, lx, ty, rx - lx, by - ty, TRUE);
    }

   
    

    // Make sure the child window is visible. 

    ShowWindow(hwndChild, SW_SHOW);

    return TRUE;
 }





BOOL CALLBACK EnumChildProcPaddle(HWND hwndChild, LPARAM lParam)
{
    //LPRECT rcParent;
    int i, idChild;

    // Retrieve the child-window identifier. Use it to set the 
    // position of the child window. 

    idChild = GetWindowLong(hwndChild, GWL_ID);



    LPPOINT padde = (LPPOINT)lParam;
    if (idChild == ID_PaddleCHILD && AreWeplaying) {

        MoveWindow(hwndChild, (LONG)padde->x-25, 220, 50, 20, TRUE);

    }


    // Make sure the child window is visible. 

    ShowWindow(hwndChild, SW_SHOW);

    return TRUE;
}




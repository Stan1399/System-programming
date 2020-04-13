// L7.cpp : Defines the entry point for the application.
//

#include "header.h"
#include "L7.h"

#define MAX_LOADSTRING 100

#define IDB_ADD 1
#define IDB_CLEAR 2
#define IDB_TORIGHT 3
#define IDB_DELETE 4
#define IDLB_1 5
#define IDLB_2 6
#define IDE_EDIT 7

// Global Variables:
HINSTANCE hInst;										// current instance
WCHAR szTitle[MAX_LOADSTRING];							// The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];					// the main window class name
HWND hBtn_ADD, hBtn_CLEAR, hBtn_TORIGHT, hBtn_DELETE;	// buttons descriptors
HWND hLstb_1, hLstb_2;									// listbox descriptor
HWND hEd;												// edit descriptor

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_L7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_L7));

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

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_L7));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_L7);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   hBtn_ADD = CreateWindow("BUTTON", "Add", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER | BS_DEFPUSHBUTTON, 10, 10, 100, 40, hWnd, (HMENU)IDB_ADD, hInstance, nullptr);
   hBtn_CLEAR = CreateWindow("BUTTON", "Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER | BS_DEFPUSHBUTTON, 10, 70, 100, 40, hWnd, (HMENU)IDB_CLEAR, hInstance, nullptr);
   hBtn_TORIGHT = CreateWindow("BUTTON", "To right", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER | BS_DEFPUSHBUTTON, 10, 130, 100, 40, hWnd, (HMENU)IDB_TORIGHT, hInstance, nullptr);
   hBtn_DELETE = CreateWindow("BUTTON", "Delete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER | BS_DEFPUSHBUTTON, 10, 190, 100, 40, hWnd, (HMENU)IDB_DELETE, hInstance, nullptr);
   hLstb_1 = CreateWindow("LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 150, 10, 150, 230, hWnd, (HMENU)IDLB_1, hInstance, nullptr);
   hLstb_2 = CreateWindow("LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 550, 10, 150, 230, hWnd, (HMENU)IDLB_2, hInstance, nullptr);
   hEd = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 350, 10, 150, 40, hWnd, (HMENU)IDE_EDIT, hInstance, nullptr);
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
			case IDB_ADD:
				BYTE chBuff[80];
				WORD cbText;
				*(WORD *)chBuff = sizeof(chBuff) - 1;	// Записываем в первое слово буфера значение размера буфера в байтах
				// Получаем от редактора текста содержимое первой строки. Функция возвращает количество байт, скопированных в буфер
				cbText = SendMessage(hEd, EM_GETLINE, 0, (LPARAM)(LPSTR)chBuff);
				chBuff[cbText] = '\0';
				//*(WORD *)chBuff = sizeof(BYTE) * cbText;
				SendMessage(hLstb_1, WM_SETREDRAW, FALSE, 0L);					// Window painting on pause
				SendMessage(hLstb_1, LB_ADDSTRING, 0, (LPARAM)(LPSTR)chBuff);	// Add string to list
				SendMessage(hLstb_1, WM_SETREDRAW, TRUE, 0L);					// Window painting on resume
				InvalidateRect(hLstb_1, NULL, TRUE);
				break;
			case IDB_CLEAR:
				SendMessage(hLstb_1, LB_RESETCONTENT, NULL, NULL);
				SendMessage(hLstb_2, LB_RESETCONTENT, NULL, NULL);
				break;
			case IDB_TORIGHT:
				// SendMessage(hLstb_1, LB_GETCURSEL, NULL, NULL)				// Detect number of selected string
				cbText = SendMessage(hLstb_1, LB_GETTEXT, SendMessage(hLstb_1, LB_GETCURSEL, NULL, NULL), (LPARAM)(LPSTR)chBuff);
				chBuff[cbText] = '\0';
				SendMessage(hLstb_2, WM_SETREDRAW, FALSE, 0L);					// Window painting on pause
				SendMessage(hLstb_2, LB_ADDSTRING, 0, (LPARAM)(LPSTR)chBuff);	// Add string to list
				SendMessage(hLstb_2, WM_SETREDRAW, TRUE, 0L);					// Window painting on resume
				break;
			case IDB_DELETE:
				SendMessage(hLstb_1, LB_DELETESTRING, SendMessage(hLstb_1, LB_GETCURSEL, NULL, NULL), NULL);
				SendMessage(hLstb_2, LB_DELETESTRING, SendMessage(hLstb_2, LB_GETCURSEL, NULL, NULL), NULL);
				break;
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

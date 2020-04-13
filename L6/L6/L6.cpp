// L6.cpp : Defines the entry point for the application.
//

#include "header.h"
#include "L6.h"

#define MAX_LOADSTRING 100
#define IDM_START 1001
#define IDM_STOP 1002
#define IDM_MAIN 0

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				CreateMenuItem(HMENU, char*, UINT, UINT, HMENU);

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
    LoadStringW(hInstance, IDC_L6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_L6));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_L6));
    wcex.hCursor        = LoadCursor(nullptr, IDC_UPARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //  MAKEINTRESOURCEW(IDC_L6);
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
	static HMENU hMainMenu;
	static HMENU hOptionsMenu;
	static char text[] = "Hello!";
	static int sec = 0;
	static bool moving = false;
    switch (message)
    {
	case WM_CREATE:
		hMainMenu = CreateMenu();
		hOptionsMenu = CreatePopupMenu();
		CreateMenuItem(hOptionsMenu, (char*)"Start", 0, IDM_START, NULL);
		CreateMenuItem(hOptionsMenu, (char*)"Stop", 1, IDM_STOP, NULL);
		CreateMenuItem(hMainMenu, (char*)"Main", 0, IDM_MAIN, hOptionsMenu);
		CreateMenuItem(hMainMenu, (char*)"Exit", 1, IDM_EXIT, NULL);
		SetMenu(hWnd, hMainMenu);
		DrawMenuBar(hWnd);
		// Timer creation:
		SetTimer(hWnd, 1, 1000, NULL);
		//...
		SetForegroundWindow(hWnd);
		break;
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDM_START:
				moving = true;
				break;
			case IDM_STOP:
				moving = false;
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_TIMER:
		sec++;
		InvalidateRect(hWnd, NULL, TRUE);
    case WM_PAINT:
        {
			if (moving)
			{
				PAINTSTRUCT ps;
				RECT rectPlace, set;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				GetClientRect(hWnd, &rectPlace);
				GetClientRect(hWnd, &set);
				rectPlace.left += sec*100;
				rectPlace.left %= (set.right - set.left);
				SetTextColor(hdc, NULL);
				HFONT hFont = CreateFont(60, 0, 0, 0, 0, 0, 0, 0,
					DEFAULT_CHARSET,
					0, 0, 0, 0,
					"Arial Bold"
				);
				SelectObject(hdc, hFont);
				DrawText(hdc, text, 6, &rectPlace, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				EndPaint(hWnd, &ps);
			}
			else
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(hWnd, &ps);
			}
			
        }
        break;
    case WM_DESTROY:
		KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Helper for creating menu items
BOOL CreateMenuItem(HMENU hMenu, char* str,
	UINT uID, UINT wID, HMENU hSubMenu)
{
	// Заполняем структуру mi.
	MENUITEMINFO mi;
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mi.fType = MFT_STRING;
	mi.fState = MFS_ENABLED;
	mi.dwTypeData = str;
	mi.cch = sizeof(str);
	mi.wID = wID;
	mi.hSubMenu = hSubMenu; // Устновка подменю.
	// Вставляем меню.
	return InsertMenuItem(hMenu, uID, FALSE, &mi);
}
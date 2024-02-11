// trafficLight.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "trafficLight.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
                        //declare methods here or in ".h" files
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
    LoadStringW(hInstance, IDC_TRAFFICLIGHT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRAFFICLIGHT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAFFICLIGHT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TRAFFICLIGHT);
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


            //put actions (things the program has to listen to) in here
            // just add a new case for every new "message" to be handled
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Static variables to keep track of the states of lights
    static bool redLightOn1 = true;
    static bool yellowLightOn1 = false;
    static bool greenLightOn1 = false;

    static bool redLightOn2 = false;
    static bool yellowLightOn2 = false;
    static bool greenLightOn2 = true;

    switch (message)
    {
    case WM_LBUTTONDOWN: //switch traffic lights on left button press
        // traffic light drive sequence: red, red+yellow, green
        // traffic light stop sequence: green, yellow, red
        {
            // Toggle the state of the lights
            if (redLightOn1 && !yellowLightOn1) {
                // Red light is on, and yellow light is off
                // Turn off red light and turn on yellow light
                redLightOn1 = true;
                yellowLightOn1 = true;
                yellowLightOn2 = true;
                greenLightOn2 = false;
            }
            else if (redLightOn1 && yellowLightOn1) {
                // Red light is on, and yellow light is on
                // Turn off red and yellow lights, and turn on green light
                redLightOn1 = false;
                yellowLightOn1 = false;
                greenLightOn1 = true;
                yellowLightOn2 = false;
                redLightOn2 = true;

            }
            else if (greenLightOn1) {
                // Green light is on
                // Turn off green light and turn on red light
                greenLightOn1 = false;
                yellowLightOn1 = true;
                redLightOn2 = true;
                yellowLightOn2 = true;
            }
            else if (yellowLightOn1 && !redLightOn1) {
                // Yellow light is on
                // Turn off yellow light and turn on green light
                yellowLightOn1 = false;
                redLightOn1 = true;
                redLightOn2 = true;
                yellowLightOn2 = true;
                greenLightOn2 = true;
            }

            // Invalidate the window to trigger a repaint
            InvalidateRect(hWnd, NULL, TRUE);
        }
    break;

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
            // create all brushes to be used during WM_PAINT (Delete all at end of WM_PAINT)
            HBRUSH hGreyBrush = CreateSolidBrush(RGB(100, 100, 100)); // create brush, grey
            HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0)); // black
            HBRUSH hRedOn = CreateSolidBrush(RGB(255, 0, 0));    //red, on
            HBRUSH hRedOff = CreateSolidBrush(RGB(70, 0, 0));    //red, off
            HBRUSH hYellowOn = CreateSolidBrush(RGB(255, 255, 0));
            HBRUSH hYellowOff = CreateSolidBrush(RGB(70, 70, 0));
            HBRUSH hGreenOn = CreateSolidBrush(RGB(0, 255, 0));
            HBRUSH hGreenOff = CreateSolidBrush(RGB(0, 70, 0));
            HGDIOBJ hOrg = SelectObject(hdc, hGreyBrush); //create object that WM_PAINT can default to at the end.

            //Road
           // Get the dimensions of the window
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            // Calculate the left and right positions of the horizontal road (left-right)
            int leftPosition = 0; // Leftmost side of the window
            int rightPosition = clientRect.right; // Rightmost side of the window

            // Calculate the positions of the horizontal road (top-bottom)
            int roadWidth = 100; // Adjust as needed
            int roadLeft = (clientRect.right - roadWidth) / 2; // Center the road horizontally
            int roadRight = roadLeft + roadWidth; // Calculate the right side of the road
            int roadTop = 0; // Topmost side of the window
            int roadBottom = clientRect.bottom; // Bottommost side of the window

            // Draw the road using the calculated positions
            SelectObject(hdc, hGreyBrush);
            Rectangle(hdc, leftPosition, 400, rightPosition, 500); // left-right
            Rectangle(hdc, roadLeft, roadTop, roadRight, roadBottom); //top-bottom


            //traffic lights background
            SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, roadLeft-110, roadTop+120, roadLeft, roadTop+400); // top-bottom light
            Rectangle(hdc, roadLeft-310, roadTop+500, roadLeft, roadTop+610); // left-right light


            //red light on/off
            if (redLightOn1) {
                SelectObject(hdc, hRedOn);
                Ellipse(hdc, roadLeft - 100, roadTop + 130, roadLeft - 10, roadTop + 210);
                //implement logic for trafficLight2
                SelectObject(hdc, hGreenOn);
                Ellipse(hdc, roadLeft-300, roadTop + 510, roadLeft-210, roadTop + 600);
            }
            else {
                SelectObject(hdc, hRedOff);
                Ellipse(hdc, roadLeft - 100, roadTop + 130, roadLeft - 10, roadTop + 210);
                //implement logic for trafficLight2
                SelectObject(hdc, hGreenOff);
                Ellipse(hdc, roadLeft - 300, roadTop + 510, roadLeft - 210, roadTop + 600);
            }



            //yellow light on/off
            if (yellowLightOn1) {
                SelectObject(hdc, hYellowOn);
                Ellipse(hdc, roadLeft - 100, roadTop + 220, roadLeft - 10, roadTop + 300); // 125, 230, 225, 330
                //implement logic for trafficLight2
                SelectObject(hdc, hYellowOn);
                Ellipse(hdc, roadLeft - 200, roadTop + 510, roadLeft - 110, roadTop + 600);
                SelectObject(hdc, hGreenOff);
                Ellipse(hdc, roadLeft - 300, roadTop + 510, roadLeft - 210, roadTop + 600);
            }
            else {
                SelectObject(hdc, hYellowOff);
                Ellipse(hdc, roadLeft - 100, roadTop + 220, roadLeft - 10, roadTop + 300); // 125, 230, 225, 330
                //implement logic for trafficLight2
                SelectObject(hdc, hYellowOff);
                Ellipse(hdc, roadLeft - 200, roadTop + 510, roadLeft - 110, roadTop + 600);
            }
            
            //green light on/off
            if (greenLightOn1) {
                SelectObject(hdc, hGreenOn);
                Ellipse(hdc, roadLeft - 100, roadTop + 310, roadLeft - 10, roadTop + 390); // 125, 340, 225, 440
                //implement logic for trafficLight2
                SelectObject(hdc, hRedOn);
                Ellipse(hdc, roadLeft - 100, roadTop + 510, roadLeft -10, roadTop + 600);
            }
            else if (!greenLightOn1 && !redLightOn1) {
                SelectObject(hdc, hGreenOff);
                Ellipse(hdc, roadLeft - 100, roadTop + 310, roadLeft - 10, roadTop + 390); // 125, 340, 225, 440
                SelectObject(hdc, hYellowOn);
                Ellipse(hdc, roadLeft - 100, roadTop + 220, roadLeft - 10, roadTop + 300);
                //implement logic for trafficLight2
                SelectObject(hdc, hRedOn);
                Ellipse(hdc, roadLeft - 100, roadTop + 510, roadLeft - 10, roadTop + 600);
                SelectObject(hdc, hYellowOn);
                Ellipse(hdc, roadLeft - 200, roadTop + 510, roadLeft - 110, roadTop + 600);
            }
            else{
                SelectObject(hdc, hGreenOff);
                Ellipse(hdc, roadLeft - 100, roadTop + 310, roadLeft - 10, roadTop + 390); // 125, 340, 225, 440
                //implement logic for trafficLight2
                SelectObject(hdc, hRedOff);
                Ellipse(hdc, roadLeft - 100, roadTop + 510, roadLeft-10, roadTop + 600);
            }
            
            //select saved object to not get errors (best practice)
            SelectObject(hdc, hOrg);
            //delete all objects to not get memmory leak
            DeleteObject(hBlackBrush);
            DeleteObject(hGreyBrush);
            DeleteObject(hRedOn);
            DeleteObject(hRedOff);
            DeleteObject(hYellowOn);
            DeleteObject(hYellowOff);
            DeleteObject(hGreenOn);
            DeleteObject(hGreenOff);

            //(WM_LBUTTONDOWN)
            

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

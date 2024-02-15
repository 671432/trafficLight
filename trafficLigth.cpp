// trafficLight.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "trafficLigth.h"
#include <Windows.h>
#include <memory>
#include <Math.h>


#define MAX_LOADSTRING 100
#define _USE_MATH_DEFINES
#define TIMER_LYS 1
#define TIMER_BIL 3
#define TIMER_ADD_BILER 4
#define TIMER_DRIVE 6
/*
#define bilLengde 25
#define bilBredde 50
#define roadWidth 100
*/

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
std::unique_ptr<TrafikkLys> trafikkLys[2];
/*
std::deque<COLORREF> northCarsColor;
std::deque<int> northCarsPosition;
std::deque<COLORREF> westCarsColor;
std::deque<int> westCarsPosition;
*/
int lightstate = 0;
int pW = 100;
int pN = 100;

// Forward declarations of functions included in this code module:
                        //declare methods here or in ".h" files
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void TrafikkLys::oppdaterPosisjon(const RECT& nyPosisjon) {
    clientRect = nyPosisjon;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TRAFFICLIGTH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRAFFICLIGTH));

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

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAFFICLIGTH));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TRAFFICLIGTH);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


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


void DrawRoads(HDC hdc, RECT clientRect) {
    HBRUSH hBrush = CreateSolidBrush(RGB(210, 173, 128));
    HGDIOBJ hOrg = SelectObject(hdc, hBrush);
    int roadWidth = 50; // Anta at dette er bredden på veien.
    int spacing = 100; // Mellomrommet mellom veiene i krysset.

    // Beregner sentrumspunktet i vinduet
    int centerX = (clientRect.right) / 2;
    int centerY = (clientRect.bottom) / 2;

    // Tegner veiene i hvert hjørne
    // Øvre venstre vei
    Rectangle(hdc, 0, 0, centerX - spacing / 2, centerY - spacing / 2);

    // Øvre høyre vei
    Rectangle(hdc, centerX + spacing / 2, 0, clientRect.right, centerY - spacing / 2);

    // Nedre venstre vei
    Rectangle(hdc, 0, centerY + spacing / 2, centerX - spacing / 2, clientRect.bottom);

    // Nedre høyre vei
    Rectangle(hdc, centerX + spacing / 2, centerY + spacing / 2, clientRect.right, clientRect.bottom);

    SelectObject(hdc, hOrg);
    DeleteObject(hBrush);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    static int rot = 0;
    static int circleX = 0;
    
    switch (message)
    {
    case WM_CREATE:
    {
        RECT clientRect;
        GetClientRect(hWnd, &clientRect); // Hent størrelsen på vinduets klientområde


        // Definerer størrelser og plassering basert på klientområdet
        int roadWidth = 50; // Bredden på veien.
        int spacing = 100; // Mellomrommet mellom veiene i krysset.
        int centerX = (clientRect.right) / 2;
        int centerY = (clientRect.bottom) / 2;

        trafikkLys[0] = std::make_unique<TrafikkLys>(RECT{ (centerX - spacing / 2 - roadWidth),(centerY - 100 / 2),(centerX - spacing / 2),(centerY + roadWidth / 2) }, TrafikkLys::Tilstand::RED);
        trafikkLys[1] = std::make_unique<TrafikkLys>(RECT{ (centerX + spacing / 2), (centerY - roadWidth / 2), (centerX + spacing / 2 + roadWidth), (centerY + roadWidth / 2) }, TrafikkLys::Tilstand::GREEN);

        SetTimer(hWnd, TIMER_LYS, 1000, NULL);
        SetTimer(hWnd, TIMER_BIL, 1000, NULL);
        SetTimer(hWnd, TIMER_DRIVE, 10, NULL);
        SetTimer(hWnd, TIMER_ADD_BILER, 1000, NULL);
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
        HDC phdc = BeginPaint(hWnd, &ps);
        HDC hdc = CreateCompatibleDC(phdc);
        //henter vindue stoerelse
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        //lager bitmap foer alt anna for at den ikke tegner bakgrunn over alt sammen
        HBITMAP bmp = CreateCompatibleBitmap(phdc, clientRect.right, clientRect.bottom);
        SelectObject(hdc, bmp);
        //ny bitmap farge (for bil veien)
        HBRUSH bg = CreateSolidBrush(RGB(200, 200, 200));
        HGDIOBJ hOrg = SelectObject(hdc, bg);
        Rectangle(hdc, 0, 0, clientRect.right, clientRect.bottom);
        //int circleX = 0;
        int circleY = (clientRect.bottom) / 2;
        //skole buss broom broom
        HBRUSH hYellowBrush = CreateSolidBrush(RGB(255, 255, 0));
        HBRUSH hWheelBrush = CreateSolidBrush(RGB(128, 128, 128));
        SelectObject(hdc, hWheelBrush);
        Ellipse(hdc, circleX - 115, circleY, circleX-90, circleY+35);
        Ellipse(hdc, circleX - 40, circleY, circleX - 15, circleY + 35);
        SelectObject(hdc, hYellowBrush);
        Rectangle(hdc, circleX - 125, circleY + 25, circleX, circleY - 25);
        HBRUSH hBlueBrush = CreateSolidBrush(RGB(135, 206, 235));
        SelectObject(hdc, hBlueBrush);
        Rectangle(hdc, circleX - 30, circleY + 22, circleX - 5, circleY - 22);
        RECT rectangle = { circleX - 125, circleY + 25, circleX - 34, circleY - 25 };
        SelectObject(hdc, hYellowBrush);
        LPCWSTR text = L"School Buss";
        Rectangle(hdc, rectangle.left, rectangle.top, rectangle.right, rectangle.bottom);
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, text, -1, &rectangle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        SelectObject(hdc, hOrg);
        //bygger bitmap
        BitBlt(phdc, 0, 0, clientRect.right, clientRect.bottom, hdc, 0, 0, SRCCOPY);
        //fortau
        DrawRoads(phdc, clientRect);
        //trafikkLys lys
        if (trafikkLys[0]) trafikkLys[0]->tegn(phdc);
        if (trafikkLys[1]) trafikkLys[1]->tegn(phdc);
        //cleanup
        DeleteObject(hYellowBrush);
        DeleteObject(hWheelBrush);
        DeleteObject(hBlueBrush);
        DeleteObject(bmp);
        DeleteDC(hdc);
        DeleteObject(bg);
        //end paint
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_TIMER:
    {
        static int timerCount = 0;
        switch (wParam)
        {
        case TIMER_LYS:
            //lightState = ++lightState % 4;
            trafikkLys[0]->SkiftTilstand();
            trafikkLys[1]->SkiftTilstand();

            if (timerCount < 1)
            {
                SetTimer(hWnd, TIMER_LYS, 1000, NULL);
                timerCount++;
            }
            else
            {
                SetTimer(hWnd, TIMER_LYS, 3000, NULL);
                timerCount = 0;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case TIMER_BIL:

            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            int centerX = (clientRect.right) / 2;
            int centerY = (clientRect.bottom) / 2;

            int prePos = 99999; //for at ingen biler er synlig paa starten
            int minDistanse = 10; //minimum distanse mellom biler
            int speed = 10; //hastighet paa biler
            int bilLengde = 25;
            int roadWidth = 50;

            /* not worki
            auto pos = northCarsPosition.begin();
            while (pos != northCarsPosition.end()) {
                if (*pos + minDistanse + speed + bilLengde < prePos) {
                    if (lightstate != 2 && *pos + bilLengde <= roadWidth/-2 && (*pos + bilLengde + speed >= roadWidth/-2)) {
                        (*pos) = roadWidth/-2-bilLengde;
                    }else {
                    (*pos) += speed;
                }
            }
            prePos = *pos;
            pos++;
            }

            pos = westCarsPosition.begin();
            prePos = 99999;
            while (pos != westCarsPosition.end()) {
                if (*pos + minDistanse + speed + bilLengde < prePos) {
                    if (lightstate != 0 && *pos + bilLengde <= roadWidth/-2 && (*pos + bilLengde + speed >= roadWidth/-2)) {
                        (*pos) = roadWidth/-2-bilLengde;
                    }else {
                    (*pos) += speed;
                }
            }
            prePos = *pos;
            pos++;
            }
            */
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
    //case TIMER_DRIVE:
            //rot = (rot+1) % 360;
            circleX += 5; //speed of cars
            InvalidateRect(hWnd, 0, false);
            break;
        
        
        /*
        case TIMER_ADD_BILER:
        HDC screenDC = GetDC(hWnd);
        int XRes = GetDeviceCaps(screenDC, HORZRES);
        int YRes = GetDeviceCaps(screenDC, VERTRES);
        DeleteDC(screenDC);
        if(northCarsColor.empty() || *(northCarsPosition.rbegin()) > -(YRes/2+50-bilLengde))........{
            if(rand()%100<pN){
                northCarsColor.push_back(RGB(rand()%255,rand()%255,radn()%255));
                northCarsPosition.push_back(-(YRes/2+50));
            }
        }
        //remove cars outside screen
        if(!northCarsColor.empty() && *(northCarsPosition.begin())YRes/2+50){
        northCarsPosition.pop_front();
        northCarsColor.pop_front();
        }
        if (westCarsColor.empty() && *(westCarsPosition.rbegin()) > -(XRes/2+50-bilLengde)){
            if (rand()%100<pW){
                westCarsColor.push_back(RGB(rand()%255,rand()%255,radn()%255));
                westCarsPosition.push_back(-(XRes/2+50));
            }
        }
        if(!westCarsColor.empty() && *(westCarsPosition.begin())>XRes/2+50){
            westCarsPosition.pop_front();
            westCarsColor.pop_front();
        }
        break;
    }
        */
        break;
    }
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            pW = pW >= 5 ? pW - 5 : 0;
            break;
        case VK_RIGHT:
            pW = pW <= 95 ? pW + 5 : 100;
            break;
        case VK_DOWN:
            pN = pN >= 5 ? pN - 5 : 0;
            break;
        case VK_UP:
            pN = pN <= 95 ? pN + 5 : 100;
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        lightstate = ++lightstate % 4;
        InvalidateRect(hWnd, 0, 0);

    case WM_SIZE:
    {
        int width = LOWORD(lParam);  // Ny bredde på vinduet
        int height = HIWORD(lParam); // Ny høyde på vinduet

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int centerX = clientRect.right / 2;
        int centerY = clientRect.bottom / 2;
        int spacing = 100;
        int roadWidth = 50;

        // Oppdaterer posisjonene til trafikklysenes rektangler
        trafikkLys[0]->oppdaterPosisjon(RECT{ centerX - 100, centerY + 50, centerX - 50, centerY + 150 });

        //trafikkLys[0]->oppdaterPosisjon(RECT{ centerX - spacing / 2 - roadWidth, centerY - roadWidth / 2, centerX - spacing / 2, centerY + roadWidth / 2 });
        trafikkLys[1]->oppdaterPosisjon(RECT{ centerX + spacing / 2, centerY - 150, centerX + spacing / 2 + roadWidth, centerY - 50 });

        InvalidateRect(hWnd, NULL, TRUE); // Be om at vinduet tegnes på nytt
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
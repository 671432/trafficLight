// trafficLight.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "trafficLigth.h"
#include <deque>
#include <strsafe.h>
#include <Windows.h>
#include <memory>


#define MAX_LOADSTRING 100
#define TIMER_LYS 1
#define TIMER_BIL 2
#define TIMER_ADD_BILER 3
#define bilLengde 60
#define bilBredde 30
#define roadWidth 100


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
std::unique_ptr<TrafikkLys> trafikkLys[2];
std::deque<COLORREF> northCarsColor;
std::deque<int> northCarsPosition;
std::deque<COLORREF> westCarsColor;
std::deque<int> westCarsPosition;
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

    // Beregner sentrumspunktet i vinduet
    int centerX = (clientRect.right) / 2;
    int centerY = (clientRect.bottom) / 2;

    // Tegner veiene i hvert hjørne
    // Øvre venstre vei
    Rectangle(hdc, 0, 0, centerX - roadWidth / 2, centerY - roadWidth / 2);

    // Øvre høyre vei
    Rectangle(hdc, centerX + roadWidth / 2, 0, clientRect.right, centerY - roadWidth / 2);

    // Nedre venstre vei
    Rectangle(hdc, 0, centerY + roadWidth / 2, centerX - roadWidth / 2, clientRect.bottom);

    // Nedre høyre vei
    Rectangle(hdc, centerX + roadWidth / 2, centerY + roadWidth / 2, clientRect.right, clientRect.bottom);

    SelectObject(hdc, hOrg);
    DeleteObject(hBrush);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT clientRect;
    GetClientRect(hWnd, &clientRect); // Hent størrelsen på vinduets klientområde
    POINT center = { clientRect.right / 2, clientRect.bottom / 2 };

    switch (message)
    {
    case WM_CREATE:
    {

        // Definerer størrelser og plassering basert på klientområdet
        trafikkLys[0] = std::make_unique<TrafikkLys>(RECT{ (center.x - roadWidth / 2),(center.y - roadWidth / 2),(center.x - roadWidth / 2),(center.y + roadWidth / 2) }, TrafikkLys::Tilstand::RED);
        trafikkLys[1] = std::make_unique<TrafikkLys>(RECT{ (center.x + roadWidth / 2), (center.y - roadWidth / 2), (center.x + roadWidth / 2), (center.y + roadWidth / 2) }, TrafikkLys::Tilstand::GREEN);

        SetTimer(hWnd, TIMER_LYS, 2000, NULL);
        SetTimer(hWnd, TIMER_BIL, 100, NULL);
        SetTimer(hWnd, TIMER_ADD_BILER, 500, NULL);

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
        SelectObject(phdc, bmp);
        //ny bitmap farge (for bil veien)
        HBRUSH bg = CreateSolidBrush(RGB(200, 200, 200));
        HGDIOBJ hOrg = SelectObject(phdc, bg);
        Rectangle(phdc, 0, 0, clientRect.right, clientRect.bottom);
        //auto generer biler
        auto itp = northCarsPosition.begin();
        auto itc = northCarsColor.begin();
        while (itp != northCarsPosition.end() && itc != northCarsColor.end()) {
            HBRUSH carBrush = CreateSolidBrush(*itc);
            SelectObject(phdc, carBrush);
            Rectangle(phdc, center.x - bilBredde/2, *itp - bilLengde / 2, center.x + bilBredde/2, *itp + bilLengde / 2);
            SelectObject(phdc, hOrg);
            DeleteObject(carBrush);
            itp++;
            itc++;
        }
        itp = westCarsPosition.begin();
        itc = westCarsColor.begin();
        while (itp != westCarsPosition.end() && itc != westCarsColor.end()) {
            HBRUSH carBrush = CreateSolidBrush(*itc);
            SelectObject(phdc, carBrush);
            Rectangle(phdc, *itp - bilLengde/2, center.y - bilBredde / 2, *itp+bilLengde/2, center.y + bilBredde / 2);
            SelectObject(phdc, hOrg);
            DeleteObject(carBrush);
            itp++;
            itc++;
        }
        //bygger bitmap
        BitBlt(phdc, 0, 0, clientRect.right, clientRect.bottom, hdc, 0, 0, SRCCOPY);
        //fortau
        DrawRoads(phdc, clientRect);
        //trafikkLys lys
        if (trafikkLys[0]) trafikkLys[0]->tegn(phdc);
        if (trafikkLys[1]) trafikkLys[1]->tegn(phdc);
        //Prosenter
        WCHAR num[10];
        SetBkMode(phdc, TRANSPARENT);
        StringCbPrintf(num, 10, L"%d%%", pW);
        TextOut(phdc, center.x - roadWidth+7,center.y + roadWidth+50, num, wcsnlen_s(num, 10));
        StringCbPrintf(num, 10, L"%d%%", pN);
        TextOut(phdc, center.x + roadWidth / 2 + 7, center.y - 165, num, wcsnlen_s(num, 10));
        //cleanup
        DeleteObject(bmp);
        DeleteDC(hdc);
        DeleteObject(bg);
        //end paint
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_TIMER:
    {
        switch (wParam)
        {
        case TIMER_LYS:
        {
            trafikkLys[0]->SkiftTilstand();
            trafikkLys[1]->SkiftTilstand();

            if (trafikkLys[0]->tilstand == TrafikkLys::Tilstand::GREEN || trafikkLys[1]->tilstand == TrafikkLys::Tilstand::GREEN)
            {
                SetTimer(hWnd, TIMER_LYS, 3000, NULL);
            }
            else
            {
                SetTimer(hWnd, TIMER_LYS, 1000, NULL);
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        case TIMER_BIL:
        {
            int prePos = 99999; //for at ingen biler er synlig paa starten
            int minDistanse = 5; //minimum distanse mellom biler
            int speed = 20; //hastighet paa biler

            auto it = northCarsPosition.begin();
            while (it != northCarsPosition.end()) {
                if (*it + minDistanse + speed + bilLengde < prePos) {
                    if (trafikkLys[1]->tilstand != TrafikkLys::Tilstand::GREEN
                       && *it + bilLengde <= center.y - 25) {
                        (*it) += 0;
                    }
                    else {
                        (*it) += speed;
                    }
                }
                prePos = *it;
                it++;
            }

            prePos = 99999;
            it = westCarsPosition.begin();
            while (it != westCarsPosition.end()) {
                if (*it + minDistanse + speed + bilLengde < prePos) {
                    if (trafikkLys[0]->tilstand != TrafikkLys::Tilstand::GREEN
                        && *it + bilLengde <= center.x-25) {
                        (*it) += 0;
                    }
                    else {
                        (*it) += speed;
                    }
                }
                prePos = *it;
                it++;
            }

            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        case TIMER_ADD_BILER:
        {
            HDC screenDC = GetDC(hWnd);
            int XRes = GetDeviceCaps(screenDC, HORZRES);
            int YRes = GetDeviceCaps(screenDC, VERTRES);
            DeleteDC(screenDC);
            //spawn top biler
            if (northCarsColor.empty() || *(northCarsPosition.rbegin()) > -(YRes / 2 + 50 - bilLengde)) {
                if (rand() % 100 < pN) {
                    northCarsColor.push_back(RGB(rand() % 255, rand() % 255, rand() % 255));
                    northCarsPosition.push_back(-(YRes / 2 + 50));
                }
            }
            //slett top biler
            if (!northCarsColor.empty() && *(northCarsPosition.begin()) > clientRect.bottom - bilLengde / 2) {
                northCarsPosition.pop_front();
                northCarsColor.pop_front();
            }
            //spawn venstre biler
            if (westCarsColor.empty() || *(westCarsPosition.rbegin()) > -(XRes/2+50-bilLengde)) {
                if (rand() % 100 < pW) {
                    westCarsColor.push_back(RGB(rand() % 255, rand() % 255, rand() % 255));
                    westCarsPosition.push_back(-(XRes/2+50));
                }
            }
            //slett venstre biler
            if (!westCarsColor.empty() && *(westCarsPosition.begin()) > clientRect.right - bilLengde/2) {
                westCarsPosition.pop_front();
                westCarsColor.pop_front();
            }
        }
        break;
        }
        break;
    }
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            pW = pW >= 10 ? pW - 10 : 0;
            break;
        case VK_RIGHT:
            pW = pW <= 90 ? pW + 10 : 100;
            break;
        case VK_DOWN:
            pN = pN >= 10 ? pN - 10 : 0;
            break;
        case VK_UP:
            pN = pN <= 90 ? pN + 10 : 100;
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
    {
        int width = LOWORD(lParam);  // Ny bredde på vinduet
        int height = HIWORD(lParam); // Ny høyde på vinduet

        // Oppdaterer posisjonene til trafikklysenes rektangler
        trafikkLys[0]->oppdaterPosisjon(RECT{ center.x - roadWidth, center.y + roadWidth/2, center.x - roadWidth/2, center.y + 150 });
        trafikkLys[1]->oppdaterPosisjon(RECT{ center.x + roadWidth / 2, center.y - 150, center.x + roadWidth, center.y - 50 });

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
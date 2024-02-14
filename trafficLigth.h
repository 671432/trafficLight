#pragma once

#include "resource.h"
#include <windows.h>



class TrafikkLys {
public:
    enum class Tilstand {
        RED,
        RED_YELLOW,
        YELLOW,
        GREEN
    };
    Tilstand tilstand;
private:

    RECT clientRect;  // Posisjonen og størrelsen til trafikklyset
    COLORREF roedFarge;
    COLORREF gulFarge;
    COLORREF gronnFarge;

    //standardfarge når lys ikke er på
    COLORREF svakRoed;
    COLORREF svakGul;
    COLORREF svakGronn;

public:
    TrafikkLys(const RECT& pos, Tilstand x) : clientRect(pos), tilstand(x),
        roedFarge(RGB(255, 0, 0)), gulFarge(RGB(255, 255, 0)), gronnFarge(RGB(0, 255, 0)),
        svakRoed(RGB(70, 0, 0)), svakGul(RGB(70, 70, 0)), svakGronn(RGB(0, 70, 0)) {}



    void tegn(HDC hdc) {
        // Her vil du implementere logikken for å tegne trafikklyset basert på tilstanden

        HBRUSH hBlackBrush = CreateSolidBrush(RGB(100, 100, 100));
        HGDIOBJ hOrg = SelectObject(hdc, hBlackBrush);

        FillRect(hdc, &clientRect, hBlackBrush); //Tegner trafikklys-rektangelet basert på clientRect som blir sendt med fra hovedprogrammet

        //Rydder opp
        SelectObject(hdc, hOrg);
        DeleteObject(hBlackBrush);

        tegnLys(hdc, clientRect, tilstand);
    }



    void tegnLys(HDC hdc, RECT rect, Tilstand tilstand) {
        int lysHoyde = (rect.bottom - rect.top) / 3;
        int lysBredde = rect.right - rect.left;

        int lysStartX = rect.left + (lysBredde - 30) / 2; // Juster størrelse på lysene
        int lysBreddeEnd = lysStartX + 30; // 30 piksler bredde for hvert lys


        RECT redRect = { lysStartX, rect.top, lysBreddeEnd, rect.top + lysHoyde };
        RECT yellowRect = { lysStartX, rect.top + lysHoyde, lysBreddeEnd, rect.top + 2 * lysHoyde };
        RECT greenRect = { lysStartX, rect.top + 2 * lysHoyde, lysBreddeEnd, rect.bottom };


        HBRUSH hRedBrush = CreateSolidBrush(tilstand == Tilstand::RED || tilstand == Tilstand::RED_YELLOW ? roedFarge : svakRoed);
        HBRUSH hYellowBrush = CreateSolidBrush(tilstand == Tilstand::YELLOW || tilstand == Tilstand::RED_YELLOW ? gulFarge : svakGul);
        HBRUSH hGreenBrush = CreateSolidBrush(tilstand == Tilstand::GREEN ? gronnFarge : svakGronn);
        HGDIOBJ hOrg = SelectObject(hdc, hRedBrush);
        //Rød lys
        FillEllipse(hdc, &redRect, hRedBrush);

        //Gult lys
        FillEllipse(hdc, &yellowRect, hYellowBrush);

        //Grønt lys
        FillEllipse(hdc, &greenRect, hGreenBrush);

        //Rydder opp

        DeleteObject(hRedBrush);
        DeleteObject(hYellowBrush);
        DeleteObject(hGreenBrush);
    }


    void oppdaterPosisjon(const RECT& nyPosisjon);

    void FillEllipse(HDC hdc, RECT* rect, HBRUSH hBrush) {
        HGDIOBJ hOrg = SelectObject(hdc, hBrush);
        Ellipse(hdc, rect->left, rect->top, rect->right, rect->bottom);
        SelectObject(hdc, hOrg);
    }



    void SkiftTilstand() {
        // Logikk for å skifte tilstand fra rød til rød+gul til grønn til gul og tilbake
        switch (tilstand) {

        case Tilstand::RED:
            tilstand = Tilstand::RED_YELLOW;
            break;
        case Tilstand::RED_YELLOW:
            tilstand = Tilstand::GREEN;
            break;
        case Tilstand::GREEN:
            tilstand = Tilstand::YELLOW;
            break;
        case Tilstand::YELLOW:
            tilstand = Tilstand::RED;
            break;

        }

    }
};
#include "SystFuncsAltWin.h"

#include <cstdint>

#include "ColorTypes.h"

namespace SystFuncs::SystFuncsAltWin {
    uint8_t GetColorR(TColor colorValue) {
        TColorRec colorRec = TColorRec();
        colorRec.color = colorValue;
        return colorRec.r;
    }

    uint8_t GetColorG(TColor colorValue) {
        TColorRec colorRec = TColorRec();
        colorRec.color = colorValue;
        return colorRec.g;
    }

    uint8_t GetColorB(TColor colorValue) {
        TColorRec colorRec = TColorRec();
        colorRec.color = colorValue;
        return colorRec.b;
    }

    TColor GetColorFromRGB(uint8_t r, uint8_t g, uint8_t b) {
        TColorRec colorRec = TColorRec();
        colorRec.a = 0;
        colorRec.r = r;
        colorRec.g = g;
        colorRec.b = b;
        return colorRec.color;
    }

    TColor GetColorFromARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
        TColorRec colorRec = TColorRec();
        colorRec.a = a;
        colorRec.r = r;
        colorRec.g = g;
        colorRec.b = b;
        return colorRec.color;
    }
}

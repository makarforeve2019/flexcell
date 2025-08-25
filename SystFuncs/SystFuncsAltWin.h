#ifndef F1FLEXCEL_CPPPROJECT_SYSTFUNCSALTWIN_H
#define F1FLEXCEL_CPPPROJECT_SYSTFUNCSALTWIN_H

#include <cstdint>

#include "ColorTypes.h"

namespace SystFuncs::SystFuncsAltWin {
    using namespace DelphiAdapt;

    uint8_t GetColorR(TColor colorValue);

    uint8_t GetColorG(TColor colorValue);

    uint8_t GetColorB(TColor colorValue);

    TColor GetColorFromRGB(uint8_t r, uint8_t g, uint8_t b);

    TColor GetColorFromARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

    // TODO: Доделать эти методы
}

#endif //F1FLEXCEL_CPPPROJECT_SYSTFUNCSALTWIN_H

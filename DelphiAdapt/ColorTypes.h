#ifndef F1FLEXCEL_CPPPROJECT_COLORTYPES_H
#define F1FLEXCEL_CPPPROJECT_COLORTYPES_H

#include <cstdint>
#include "Property.h"

namespace DelphiAdapt {
    using TColor = int;

    struct TColorRec {
        uint8_t a = 0;
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;

        TColor GetColor() {
            return (a << 24) | (r << 16) | (g << 8) | b;
        }

        void SetColor(TColor colorValue) {
            a = (colorValue & 0xff000000) >> 24;
            r = (colorValue & 0x00ff0000) >> 16;
            g = (colorValue & 0x0000ff00) >> 8;
            b = (colorValue & 0x000000ff);
        }

        Property<TColor, TColorRec> color = Property<TColor, TColorRec>(this, &TColorRec::GetColor, &TColorRec::SetColor);
    };
}

#endif //F1FLEXCEL_CPPPROJECT_COLORTYPES_H

#ifndef MWSHorizontalAlignEnumH
#define MWSHorizontalAlignEnumH


#include <string>
#include <vector>

enum class TMWSHorizontalAlignEnum {
    haGeneral, haLeft, haCenter, haRight, haFill, haJustify, haCenterAcrossCells
};

enum class TMWSVerticalAlignEnum {
    vaTop, vaCenter, vaBottom
};

enum class TMWSBorderStyleEnum {
    bsNone, bsThin, bsMedium, bsDashed, bsDotted, bsThick, bsDouble, bsHair, bsMediumDashed, bsDashDot, bsMediumDashDot, bsDashDotDot, bsMediumDashDotDot, bsSlantedDashDot
};

enum class TMWSFixupEnum {
    fNormal, FAppend, fPrepend
};

#endif
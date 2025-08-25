#ifndef F1FLEXCEL_CPPPROJECT_TF1RANGE_H
#define F1FLEXCEL_CPPPROJECT_TF1RANGE_H

#include "Property.h"

namespace F1DevExpress::unF1FlexCel {
    using namespace DelphiAdapt;

    struct TF1Range {
        int StartRow = 0;
        int EndRow = 0;
        int StartCol = 0;
        int EndCol = 0;

        TF1Range() = default;

        PropertyRO<int, TF1Range> Rows = PropertyRO<int, TF1Range>(this, &TF1Range::Get_Rows);
        PropertyRO<int, TF1Range> Cols = PropertyRO<int, TF1Range>(this, &TF1Range::Get_Cols);

        int Get_Rows() {
            return EndRow - StartRow;
        }

        int Get_Cols() {
            return EndCol - StartCol;
        }
    };
}

#endif //F1FLEXCEL_CPPPROJECT_TF1RANGE_H

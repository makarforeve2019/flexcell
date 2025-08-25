#ifndef MWSRESTAPI_UPDBUDGPRO2_H
#define MWSRESTAPI_UPDBUDGPRO2_H


#include "unFormLogicF1.h"
#include "json.hpp"

class TUpdBudgPro2 {
public:
    static nlohmann::json F1Book1DblClickEvent(TFormLogicF1* formLogicF1, int nSheet, int nRow, int nCol);
};


#endif //MWSRESTAPI_UPDBUDGPRO2_H

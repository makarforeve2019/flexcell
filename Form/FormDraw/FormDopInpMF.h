#ifndef FormDopInpMFH
#define FormDopInpMFH

#include "../FormTypes.h"
#include "../DB/DB_Common.h"
#include "../unDopAtrCol.h"
#include "../../nlohmann/json.hpp"

class TFormDopInpMF {
public:
    static std::string GetNextFormula(std::string aSS, int& aNCol, std::string& aForm);
    static std::string GetSubFormulaByN(std::string aSS, int aN);
    static int GetField_id(int class_id, std::string note2);
    static int getIdFromNote2(std::string note2);
    static std::string GetFormulaForCol(std::string aSS, int aNCol);
    static int GetPokazIdForCol(std::string aSS, int aNCol);
    static int GetPokazIdForCol2(std::string aSS, int aNCol, bool& isSimvR);
    static std::string ConvertColToSimvs(int aCol);
};

#endif
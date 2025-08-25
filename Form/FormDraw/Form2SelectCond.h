#ifndef Form2SelectCondH
#define Form2SelectCondH

#include "../FormTypes.h"
#include "Form2.h"
#include "../DB/DB_Common.h"
#include "../Form/unDopAtrCol.h"
#include "../DB/unDBType.h"
#include "../nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class TForm2SelectCond : public TForm2 {
public:
    TForm2SelectCond(TForm2* form) : TForm2(*form) {}
    bool doForm2SelectCond(int aPlan_id, string& aAndWhere);
    void doEvalAtrSelectCond(PDopAtrCol DopAtrCol, int god, int CurrVidOtch, int CurrQuarter, int CurrMonth, int condLayerNumber);
    void PrepareAtrSelectCondShablon(PDopAtrCol DopAtrCol, int god, int CurrVidOtch, int CondLayerNumber);
    static void ParseShablonChislo(int condLayerNumber, PAtrStmnt AtrStmnt);
};

#endif


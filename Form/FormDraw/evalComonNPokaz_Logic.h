#ifndef unevalComonNPokaz_LogicH
#define unevalComonNPokaz_LogicH

#include "unFormLogicF1.h"
#include "FormDopInp.h"
#include "../FormTypes.h"
#include "../../DB/DB_Common.h"

class TevalComonNPokaz_Logic {
public:
    // initial display of the value in the form
    static void showZnacenijCNP(TwwStringList* p, TFormLogicF1* FormLogicF1, PCellValueAtr vd, int nRow, int nCol);
    // initial display of the final value for all subjects in the form
    static void showZnacenijCNPallPlans(TwwStringList* p, TFormLogicF1* FormLogicF1, PCellValueAtr vd, int nRow, int nCol);
    // automatic calculation of other columns after the user selects an element from the list of budget recipients
    static void avtoEvalOtherByBudgPoluc(TwwStringList* p, TFormLogicF1* FormLogicF1, std::string toTblName, int budget_poluc_id, int nRow);
    // automatic calculation of other columns after the user selects an element from the product nomenclature
    static void avtoEvalOtherByVidProdukcii(TwwStringList* p, TFormLogicF1* FormLogicF1, std::string toTblName, int vidprod_id, int nRow);
    // automatic calculation of other columns after the user selects an element from OKDP
    static void avtoEvalOtherByOKDP(TwwStringList* p, TFormLogicF1* FormLogicF1, std::string toTblName, int okdpId, int nRow);
    // automatic calculation of other columns after the user selects an element from OKVED
    static void avtoEvalOtherByOKVED(TwwStringList* p, TFormLogicF1* FormLogicF1, std::string toTblName, int okved_id, int nRow);
    // common part of automatic calculation
    static void avtoEvalOther(TQueryUni* q, TwwStringList* p, TFormLogicF1* FormLogicF1, std::string toTblName, int nRow);
};
#endif

#ifndef Form2UslovieH
#define Form2UslovieH

#include "../FormTypes.h"
#include "Form2.h"
#include "../DB/DB_Common.h"
#include "../Form/unDopAtrCol.h"

class TForm2Uslovie : public TForm2
{
public:
    TForm2Uslovie(TForm2* form) : TForm2(*form) {}
    void doForm2Uslovie(const int aOwnerNomRow, int newCurrPlanId, std::string strName, std::string note2,
                        int level_sign, int readonly, int is_hidden, int is_filter2, int ColOnFilter2, int nStolbec,
                        std::string font, int h, int is_bold, TQueryUni *q);
};

#endif


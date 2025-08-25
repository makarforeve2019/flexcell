#ifndef Form2AgregatH
#define Form2AgregatH

#include "../../DB/DB_Common.h"
#include "Form2.h"

class TForm2Agregat : public TForm2
{
public:
    TForm2Agregat(TForm2* form) : TForm2(*form) {}
    void doForm2Agregat(const int aOwnerNomRow, const int newCurrPlanId, const std::string strName, const std::string note2,
                        const int level_sign, const int readonly, const int is_hidden, const int is_filter2,
                        const int ColOnFilter2, const int nStolbec, const std::string font, const unsigned char h,
                        const unsigned char is_bold, TQueryUni* q);
};

#endif

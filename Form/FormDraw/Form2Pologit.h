#ifndef FORM2POLOGIT_H
#define FORM2POLOGIT_H

#include "Form2.h"
#include "../DB/DB_Common.h"
#include "../unDopAtrCol.h"
#include <string> // Include the standard string header

class TForm2Pologit : public TForm2
{
public:
    TForm2Pologit(TForm2* form) : TForm2(*form) {}
    void doForm2Pologit(const int aOwnerNomRow, int newCurrPlanId, std::string strName, std::string note2,
        short level_sign, short Is_open, short readonly, short is_hidden, short is_filter2,
        short ColOnFilter2, short nStolbec, std::string font, unsigned char h, unsigned char is_bold,
        unsigned char Align_vert, unsigned char Align_gor, TQueryUni *q);
};

#endif // FORM2POLOGIT_H

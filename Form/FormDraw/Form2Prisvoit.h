#ifndef Form2PrisvoitH
#define Form2PrisvoitH

#include "../FormTypes.h"
#include "Form2.h"
#include "../../DB/DB_Common.h"
#include "../unDopAtrCol.h"

class TForm2Prisvoit : public TForm2 {
public:
    TForm2Prisvoit(TForm2* form) : TForm2(*form) {}
    void doForm2Prisvoit(int aOwnerNomRow, int newCurrPlanId, std::string strName, std::string note2,
                                int level_sign, bool Is_open, bool readonly, bool is_hidden, bool is_filter2,
                                int ColOnFilter2, int nStolbec, std::string font, unsigned char h, bool is_bold,
                                unsigned char Align_vert, unsigned char Align_gor, TQueryUni* q, unsigned char isRow);
};

#endif
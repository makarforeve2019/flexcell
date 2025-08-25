#ifndef Form2PokazKortegH
#define Form2PokazKortegH

#include "../FormTypes.h"
#include "Form2.h"
#include "../../DB/DB_Common.h"
#include "../unDopAtrCol.h"

class TForm2PokazKorteg : public TForm2 {
public:
    TForm2PokazKorteg(TForm2* form) : TForm2(*form) {}
    void doForm2PokazKorteg(bool aIs_sort, int aOwnerNomRow,
                                   int newCurrPlanId, int saveCurrPlanId,
                                   std::string strName, std::string note2, std::string shablon, std::string oldshablon,
                                   int level_sign, int Is_open, int readonly, int is_hidden, int is_filter2, int ColOnFilter2, int noPrint, int is_filter, int nStolbec,
                                   std::string font, unsigned char h, unsigned char is_bold,
                                   unsigned char Align_vert, unsigned char Align_gor, PDopAtrCol aDopAtrCol,
                                   TQueryUni* q);
};

#endif
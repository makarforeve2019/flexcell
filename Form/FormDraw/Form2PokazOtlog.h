#ifndef Form2PokazOtlogH
#define Form2PokazOtlogH

#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/DB_Pokaztl.h"
#include "../DB/unDBType.h"
#include "Form2PokazKorteg.h"
#include "../../calc/unIspolGrupu_AlbumLogic.h"
#include "../Form/unDopAtrCol.h"

class TForm2PokazOtlog : public TForm2 {
public:
    TForm2PokazOtlog(TForm2* form) : TForm2(*form) {}
    void doForm2PokazOtlog(bool aIs_sort, int aOwnerNomRow, int newCurrPlanId, int saveCurrPlanId,
                           std::string strName, std::string note2, std::string shablon, std::string oldshablon,
                           int level_sign, bool Is_open, bool readonly, bool is_hidden, bool is_filter2, int ColOnFilter2, bool noPrint, bool is_filter, int nStolbec,
                           std::string font, unsigned char h, unsigned char is_bold,
                           unsigned char Align_vert, unsigned char Align_gor, PDopAtrCol aDopAtrCol,
                           TQueryUni *q);
};
#endif
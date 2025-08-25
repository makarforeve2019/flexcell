#ifndef Form2PlusPatternKBKH
#define Form2PlusPatternKBKH

#include "unFormLogicF1.h"
#include "FormDopInp.h"
#include "FormTypes.h"
#include "DB_Common.h"
#include "Form1.h"
#include "unDopAtrCol.h"

class TForm2PlusPatternKBK {
public:

    static std::string createLikePattern(int CurrYear, std::string shablon);
    static void ExecPokazPatternKBK(PDopAtrCol aDopAtrCol, TFormLogicF1* FormLogicF1, int aRow, TwwStringList* p, TQueryUni* q);
    static void SummaPatternKBK(TFormLogicF1* FormLogicF1, int aRow, TwwStringList* p, int pokaz_idRoot, const std::string& LikePattern);
};
#endif
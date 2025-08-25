#ifndef Form2F1H
#define Form2F1H

#include "../FormTypes.h"
#include "Form2.h"
#include "../DB/DB_Common.h"
#include "../unDopAtrCol.h"

class TForm2F1 : public TForm2
{
public:
    TForm2F1(TForm2* form) : TForm2(*form) {}
    void SelectF1(int newPokazCol, std::string note2, PFormatStyle aFormatStyle, int tocnost,
                  int is_filter2, int ColOnFilter2, PDopAtrCol aDopAtrCol);
};

#endif

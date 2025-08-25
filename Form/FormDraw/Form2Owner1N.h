#ifndef Form2Owner1NH
#define Form2Owner1NH

#include "../FormTypes.h"
#include "Form2.h"
#include "../DB/DB_Common.h"
#include "../unDopAtrCol.h"

class TForm2Owner1N : public TForm2
{
public:
    TForm2Owner1N(TForm2* form) : TForm2(*form) {}
    void doForm2Owner1N(int aPlan_id, PDopAtrCol aDopAtrCol, int aOwnerNomRow);
};

#endif

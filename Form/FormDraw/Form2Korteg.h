#ifndef Form2KortegH
#define Form2KortegH

#include "../Form/FormTypes.h"
#include "Form2.h"
#include "../DB/DB_Common.h"
#include "../Form/unDopAtrCol.h"

class TForm2Korteg : public TForm2 {
public:
    TForm2Korteg(TForm2* form) : TForm2(*form) {}
    void doForm2Korteg(TQueryUni* aQOwnerPlanStr, int aPlan_id, PDopAtrCol aDopAtrCol, std::string aStmntRow);
};

#endif
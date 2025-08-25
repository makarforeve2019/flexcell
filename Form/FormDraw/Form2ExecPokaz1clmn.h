#ifndef Form2ExecPokaz1clmnH
#define Form2ExecPokaz1clmnH

#include "../unDopAtrCol.h"
#include "Form2.h"

class TForm2ExecPokaz1clmn : public TForm2
{
public:
    TForm2ExecPokaz1clmn();

    TForm2ExecPokaz1clmn(TForm2* form) : TForm2(*form) {}
    void ExecPokaz1clmn(int nRow, int nClmn, PAtrStmnt AtrStmnt, PAtrRow AtrRow,
                               TQueryUni* aQOwnerPlanStr, int aPlan_id, PDopAtrCol aDopAtrCol, std::string aStmntRow);
};
#endif
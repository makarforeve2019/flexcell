#ifndef Form2ObjectH
#define Form2ObjectH

#include "FormTypes.h"
#include "DB_Common.h"
#include "unDopAtrCol.h"
#include "Form2ExecPokaz1clmn.h"

class TForm2Object : public TForm2ExecPokaz1clmn
{
public:
    TForm2Object(TForm2ExecPokaz1clmn* form) : TForm2ExecPokaz1clmn(*form) {}

    TForm2Object(TForm2 *pForm2);

    void SelectSpField(int aOwnerNomRow, PDopAtrCol aDopAtrCol);
};

#endif

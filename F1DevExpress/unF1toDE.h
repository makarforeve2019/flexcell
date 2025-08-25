#ifndef unF1toDEH
#define unF1toDEH

#include "../Form/FormTypes.h"
#include "TF1FlexCel.h"
#include "../Form/FormDraw/unFormLogicF1.h"

class TF1toDE {
public:
    static void CreateClmnCells(TwwStringList* p, TFormLogicF1* FormLogicF1, int nCol);
    static void ColWidth(TFormLogicF1* F1Book1, int nCol, int WidthTwips, int WidthPixels);
    static void CreateNewRowsCells(TwwStringList* p, TFormLogicF1* FormLogicF1, int nRow1, int nRow2);
    static int CreateNewRowCells(TwwStringList* p, TFormLogicF1* FormLogicF1);
    static void InsertRowsCells(TwwStringList* p, TFormLogicF1* FormLogicF1, int nRow1, int nRow2);
    static void DeleteRows(TFormLogicF1* F1Book1, int nRow1, int nRow2);
    static void WorkSetBorder(TwwStringList* p, TFormLogicF1* FormLogicF1, int aRow, int aCol);
};
#endif
#ifndef Form2loadValueH
#define Form2loadValueH

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <sstream>
#include "unSqlPackets.h"
#include "unFormLogicF1.h"
#include "FormTypes.h"

using namespace std;

class TForm2loadValue {
public:
    TSelCells sc;
    TSelCellsUslPost scUP;
    TSelCellsSum scSum;
    TAgregat scAgr;
    TAgregatSum scAgrSum;
    TComNP scComNP;
    TSelCellsSumObAlb scSumObAlb;
    TComNpSumObAlb scComNpSumObAlb;
    TSelCellsHistory scHistory;

    static void SetInputValue(TFormLogicF1* FormLogicF1, TwwStringList* p, bool isRowMode, int aRow);
    void SetInputValueDalee(TFormLogicF1* FormLogicF1, TwwStringList* p, bool isRowMode, int aRow);
    void doSC(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd);
    void doSCUP(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd);
    void doSCSum(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd);
    void doSCAgr(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd);
    void doSCAgrSum(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd);
    void doSCComNP(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd);
    void doSCSumObAlb(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd);
    void doSCComNPSumObAlb(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd);
    void doSCHistory(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd);
    void doNevicisl(TFormLogicF1* FormLogicF1, TwwStringList* p, PCellValueAtr vd);
    void doVicisl(TFormLogicF1* FormLogicF1, TwwStringList* p, PCellValueAtr vd);
};

#endif
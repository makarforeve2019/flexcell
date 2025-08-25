#ifndef FormDataH
#define FormDataH

#include "../DB/DB_Common.h"
#include "../DB/unDBType.h"

class TFormDM
{
public:
    TQueryUni *quFormStr;
    TQueryUni *quValue;
    TQueryUni *quPokaztl;
    TQueryUni *quIstFin;
    TQueryUni *quPokaz;
    TQueryUni *quDinIzm;
    TQueryUni *quValuePeriod;
    TQueryUni *quSumPotok;
    TQueryUni *quFormColmn;
    TQueryUni *quOstatok;

    TFormDM();
    ~TFormDM();
};

#endif

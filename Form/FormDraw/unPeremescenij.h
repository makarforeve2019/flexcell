#ifndef UNPEREMESCENIJ_H
#define UNPEREMESCENIJ_H

#include "DB_Common.h" // Assuming this header contains necessary declarations
#include "unPeremescenijColDef.h" // Assuming this header contains necessary declarations

class TPeremescenij : public TPeremescenijColDef {
public:
    static double doEval(int plan_id, int pokaz_id, int VidOtc, int god, int Nvalue);
    static double doEval1(int plan_id, int pokaz_id, int VidOtc, int god);
    static double doEvalSnjto(int plan_id, int pokaz_id, int VidOtc, int god);
    static double doEvalPologeno(int plan_id, int pokaz_id, int VidOtc, int god);
    static double doEvalSnjtoRaspred(int plan_id, int pokaz_id, int VidOtc, int god);
    static double doEvalPologenoRaspred(int plan_id, int pokaz_id, int VidOtc, int god);
};

#endif // UNPEREMESCENIJ_H

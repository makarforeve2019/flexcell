#ifndef FormGFLogicH
#define FormGFLogicH

#include "FormTypes.h"
#include "DB_Common.h"

class TFormGFLogic {
public:
    // Update the financing schedule plan selectively
    static void UpdPlGrFin(TFormLogicF1* FormLogicF1);
    // Update the financing schedule fact selectively
    static void UpdFactGrFin(TFormLogicF1* FormLogicF1);
    // Update the quarterly financing fact selectively
    static void UpdFactKv(TFormLogicF1* FormLogicF1);
    // Set IsUpd to False
    static void SetIsUpdToFalse(TFormLogicF1* FormLogicF1);
};
#endif


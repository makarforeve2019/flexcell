#ifndef Form2IerarxH
#define Form2IerarxH

#include "unFormLogicF1.h"
#include "../FormTypes.h"
#include "Form2.h"
#include "../DB/DB_Common.h"
#include "../unDopAtrCol.h"

class TForm2Ierarx : public TForm2 {
public:
    TForm2Ierarx(TForm2* form) : TForm2(*form) {}
    void SelectObjectsIerarx(bool is_sort, int aOwnerNomRow, std::string OwnerLevelText, 
        std::string VertStr, TQueryUni* prOwnerPlanStr, std::string classIdlist, TExecuteSxemaIerarx& ExSxIerarx, 
        int NOwner, int NSubIerarx1, PDopAtrCol aDopAtrCol);
    void SelectObjectsIerarxDalee(bool is_sort, int aOwnerNomRow, std::string OwnerLevelText, 
        std::string VertStr, TQueryUni* prOwnerPlanStr, std::string classIdlist, TExecuteSxemaIerarx& ExSxIerarx, 
        int NOwner, int NSubIerarx1, PDopAtrCol aDopAtrCol, int aNewCurrPlanId);
    void strokiForClassObject(bool is_sort, int aOwnerNomRow, TQueryUni* quPlanStr, TExecuteSxemaIerarx& ExSxIerarx, 
        int NSelClass, PDopAtrCol aDopAtrCol);
    void strokiPeredIerarxii(int aOwnerNomRow, TExecuteSxemaIerarx& ExSxIerarx, PDopAtrCol aDopAtrCol);
    void strokiPosleIerarxii(int aOwnerNomRow, TExecuteSxemaIerarx& ExSxIerarx, PDopAtrCol aDopAtrCol);
    void outRowToF1(int aOwnerNomRow, PRowStmnt rowStmnt, PDopAtrCol aDopAtrCol);
    static void PrepareData(TForm2* Form2, TFormLogicF1* FormLogicF1, unsigned char IsParamForm, int ParamObjectId, TExecuteSxemaIerarx& ExSxIerarx,
       std::string& classIdlist, int& NSubIerarx1, int nBlockIerarxii);
    static void LoadExecuteSxemaIerarx(TExecuteSxemaIerarx& ExSxIerarx, int class_id, TFormLogicF1* FormLogicF1, int FormId, int nBlockIerarxii);
    static std::string createClassIdlist(TExecuteSxemaIerarx& ExSxIerarx, int aOwner, int& NSubIerarx1);
    static int evalNOwnerNew(TExecuteSxemaIerarx& ExSxIerarx, int NOwner, int objekt_class_id);
};

#endif
#ifndef FormUpdF1LogicH
#define FormUpdF1LogicH

#include "unF1toDE.h"
#include "unFormLogicF1.h"
#include "FormTypes.h"

class TFormUpdF1Logic {
public:
    static void updNameinF1(TFormLogicF1* FormLogicF1, int nRow, string newName);
    static void setInCellStr(TFormLogicF1* FormLogicF1, int nRow, int objectid, int field_id, string str_value);
    static void setInCellNmbr(TFormLogicF1* FormLogicF1, int nRow, int objectid, int field_id, int ist_fin_id, int VidOtch, double value);
    static void SetCellValueStr(TFormLogicF1* FormLogicF1, int nRow, int nCol, string str_value);
    static void setInRowNmbr(TFormLogicF1* FormLogicF1, int nRow, int objectid);
    static void doReDraw(TFormLogicF1* formLogicF1, TwwStringList* p, int nRow);
    static bool insertObjectToF1(TFormLogicF1* FormLogicF1, int OwnerId, int object_id, const std::string object_name,
                                 int objekt_class_id, std::string KBK, int level_sign, int& aInsertRow);
    static void insertEmptyRow(TFormLogicF1* FormLogicF1, int insertRow, TwwStringList* p, int OwnerNomRow, int Ownerid, int objekt_class_id, int ObjectId, int level_sign, int L, int CurRowVid);
    static void DostupnCells(TFormLogicF1* FormLogicF1, int insertRow, int etlnRow);
    static string prefixObjectName(TFormLogicF1* FormLogicF1, int insertRow, TwwStringList* p, int ColName);
    static void toVisible(TFormLogicF1* FormLogicF1, int insertRow, int colName);
    static bool insertPokaztlToF1(TFormLogicF1* FormLogicF1, int OwnerId, int pokaz_id, string pokaz_name, string KBK,
                                  int pokaz_type, int enum_id, int edizm_id, int level_sign, int & aInsertRow);
    static void delPrefixFromNames(TFormLogicF1* FormLogicF1);
    static void deleteRowFromF1(TFormLogicF1* FormLogicF1, int nRow);
    static void deleteRowFromF1_0(TFormLogicF1* FormLogicF1, int nRow);
    static void delRowByPokaz(TFormLogicF1* FormLogicF1, int nRow, int nCol);
    static bool delDefMrpr(TFormLogicF1* FormLogicF1, int nRow, int pokaz_Id, std::string& msgError);
    static bool UpdPokazMrprObj(int pokaz_id,std::string pokaz_name, std::string primecanie);;
};

#endif
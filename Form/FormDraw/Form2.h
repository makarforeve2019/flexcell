
#ifndef FORM2_H
#define FORM2_H

#include "../Form/FormDraw/unFormLogicF1.h"
#include "unF1toDE.h"
#include "../Form/FormTypes.h"
#include "../DB/DB_Common.h"
#include "Form2Plus.h"
#include "../Form/unDopAtrCol.h"

struct TCurrObject {
    int pokaz_id;
    int pokaz_is_razd;
    bool isSimvR=false; // используется только в случае кортежа
};

class TForm2 {
public:
    TInfoFormLogic * InfoFormLogic;
    TFormLogicF1 * FormLogicF1;
    TwwStringList * p;
    int work_with;
    int IsParamForm;   // если IsParamForm = 1, то для параметрической формы, если 0, то нет
                         // если IsParamForm = 2, то для одного объекта ParamObjectId: сгенерировать описания ячеек
                         // и ввести данные в строку nRowParamObjectId   
    int ParamObjectId;      // ParamObjectId содержит ид объекта конкретизации
    int nRowParamObjectId; // номер строки в форме, в которой расположен объект ParamObjectId
    std::string note2;  // для передачи информации к ExecPokaz для кортежа показателей
    std::string Value;
    double ValueD, ValueD2;
    int CurrPokazId, PokazId, PokazType, DataType, PlanId;
    TDateTime PokazDate;
    int period;
    int PeriodType, ValuePeriod, PeriodT, PT, IsIstFin, IstFinGradId, IsGroup, DinIzmId, DinIzmGradId, VidOtch;
    int Kv = 0, Mes = 0, Cnt = 0;
    int LevelType;
    std::string LevelText;
    std::string DinIzmCode;
    bool bBold, bItalic;
    int coef_Pokaz=0;
    int coef_Forma=0;
    int EnumId;
    int BeginRow;
    int RootPokazId;
    TCurrObject CurrObject;
    bool isSelectSubItem;
    bool isOutPutIdRow, isSortedForm;
    TDopAtrCol wDopAtrCol;
    TQueryUni * qSvjzClass;
    int class_idXarakterSvjzObjecta;
    TStringList qDefPokazList; // список запросов столбцов для кортежа
    TStringList slClassIds; // список пар (class_id, nRow)

    // если IsParamForm = 1, то для параметрической формы, если 0, то нет
    // ParamObjectId содержит ид объекта конкретизации
    static void InitFormRows(TInfoFormLogic * InfoFormLogic, TFormLogicF1 * FormLogicF1, int IsParamForm, int ParamObjectId, int nRowParamObjectId = 0);
    // Добавляет новую строку
    static int AddNewRow(TFormLogicF1 * FormLogicF1, int aOwnerNomRow, int OwnerId, int ObjectId);
    // заполняет колонку наименования показателя 
    static int SetTextNameCol(TFormLogicF1 * FormLogicF1, int aRow, std::string Value, int edIzm_id);
    static int SetTextNameColVertStr(TFormLogicF1 * FormLogicF1, int aRow, std::string Value);
    void doInitFormRows();
    void GetCoef_Pokaz(TQueryUni * aQPokaztl);
    bool GetNaturVid();
    // готовит элемент в списке TCellValueAtr для значения показателя или вычмсления
    // параметр содержит описание показателя или характеристики   
    void SetValueData(int nRow, int nClmn, PAtrStmnt AtrStmnt, PAtrRow AtrRow, std::string aStmnt, TQueryUni * aQDefPokaz, PDopAtrCol aDopAtrCol);
    void SetValueDataForComNP(int nClmn, std::string aStmnt, PDopAtrCol aDopAtrCol);
    void SetValueDataForPriznak(int nClmn, PDopAtrCol aDopAtrCol);
    // параметр содержит описание показателя или характеристики
    void InputWithDinIzm(int nRow, int nClmn, PAtrStmnt AtrStmnt, PAtrRow AtrRow, std::string aStmnt, TQueryUni *aQDefPokaz, PDopAtrCol aDopAtrCol);
    // обрабатывая оператор столбец создает ячейку в списке TValueData,
    // оператор не исполняется,
    // в CurrPokazId лежит текущий pokaz_id
    void ExecPokaz(int aOwnerNomRow, TQueryUni * aQOwnerPlanStr, int aPlan_id, PDopAtrCol aDopAtrCol, std::string aStmntRow);
    void doStmnt(int nClmn, int nRow, TQueryUni* aQOwnerPlanStr, std::string stmnt, int wCurrCol);
    void ExecCelPrgrm(int kbk_id);
    // выбор подчиненных показателей/объектов, заданных в описании формы через владельца
    void SelectOwnerPokaz(int aOwnerNomRow, std::string OwnerLevelText, std::string VertStr, TQueryUni * aOwnerPlanStr, int bSkipRow, int isFilter, int isFilter2, int ColOnFilter2, PDopAtrCol aDopAtrCol, std::string aStmntRow, std::string shablon, int aLevel_sign, int aNomUrovRazdela = 0);
    // выбор подчиненных показателей/объектов, заданных в описании формы через владельца
    // в ПОРЯКЕ, определяемой целевыми программами
    void SelectPokazOrderCP(int aRootPokazId, int aOwnerNomRow, std::string OwnerLevelText, std::string VertStr, bool bSkipRow, int isFilter, int isFilter2, int ColOnFilter2, PDopAtrCol aDopAtrCol, std::string aStmntRow, std::string shablon, int isOpen);
    // выбор показателей только КОСГУ
    void SelectOnlyKOSGU(int aRootPokazId, int aOwnerNomRow, std::string OwnerLevelText, std::string VertStr, bool bSkipRow, int isFilter, int isFilter2, int ColOnFilter2, PDopAtrCol aDopAtrCol, std::string aStmntRow);
    // выбор подчиненных показателей/объектов, заданных в описании формы через владельца
    // только указанного вида
    void SelectPokazOnlyVid(int aOwnerNomRow, std::string OwnerLevelText, std::string VertStr, TQueryUni * aOwnerPlanStr, bool bSkipRow, int isFilter, int isFilter2, int ColOnFilter2, PDopAtrCol aDopAtrCol, std::string aStmntRow, int aVid);
    // выбор сортированных подчиненных показателей, заданных в описании формы через владельца
    void SelectSortsPokaz(int aOwnerNomRow, TQueryUni * prOwnerPlanStr, PDopAtrCol aDopAtrCol, int isFilter, int isFilter2, int ColOnFilter2);
    void DoReadonlyHidden(int nRowFrom, int nRowTo, int aReadonly, int aIs_hidden);
    // выбор показателей, заданных в описании формы списком
    void SelectSpStrok(int blockLevelUr, int aForm_id, int aWork_with, bool aIs_sort, int aOwnerNomRow, int isFilter2Block = 0, int ColOnFilter2Block = 0, int plan_idBlock = 0);
    // выбор подчиненных объектов, заданных в описании формы посредством иерархии
    void SelectObjectsIerarx(bool is_sort, int aOwnerNomRow, std::string OwnerLevelText,
                             std::string VertStr, TQueryUni* prOwnerPlanStr,
                             std::string classIdlist, TExecuteSxemaIerarx& ExSxIerarx,
                             int NOwner, int NSubIerarx1, PDopAtrCol aDopAtrCol);
    // готовит и выполняет quFormColmn по всем полям-столбцам
    void SelectAllField();
    //вставить кнопку
    void InputButton(TFormLogicF1 * FormLogicF1, int nRow, int nCol, int Height, std::string aCaption);
    // в конец выборки по фильтру по КБК вставить формулу суммирования;
    void doGenerirFormula1(int aOwnerNomRow, int nRowFrom, int nRowTo, int level_sign, std::string aStrName, PDopAtrCol aDopAtrCol);
    // по aClass_id получить nRow в списке slClassIds. Если нет добавить, вернуть 0
    int findRowByClassId(int aClass_id, int aRow);
    // вычислиь Вид показателя/объекта - для системной характеристики
    static std::string evalVidPokazObj(TwwStringList * p, int aRow, TQueryUni * q);
};

#endif // FORM2_H

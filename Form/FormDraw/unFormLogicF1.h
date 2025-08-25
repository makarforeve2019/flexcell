#ifndef FormLogicF1H
#define FormLogicF1H

#include "json.hpp"
#include "../F1DevExpress/TF1FlexCel.h"
#include "FormTypes.h"
#include "../F1DevExpress/unActionView.h"
#include "Cls_MyBaseArray.h"
#include "DB_Common.h"
#include "FormData.h"
#include "TTquCell.h"

using json = nlohmann::json;

class TFormLogicF1 {
private:
    void ClearListsForForm(int aNSheetDelete);
    void InfoAlbumLogicGet(TInfoAlbumLogic& InfoAlbumLogic);
    void doItogoStyleOforml(TwwStringList* p, int aRow, int aCol);
    void doEvalHRow2(int r, int c1, int c2);
    void doSetColWidthAuto();
    int GetLastVisibleCol();
    void CreateGlobalVars();
    void DeleteGlobalVars();

public:
    std::string UserSessionId;
    std::string TblSessionId;
    std::string TEstTekst;
    std::string CurrElementStr;
    std::string CurrAlbumName;
    int CurrAlbumId;
    int accessalbum_idSelBlockSubj;
    int CurrPlanId;
    std::string CurrPlanName;
    bool CanEdit;
    int CurrVidOtch;
    int CurrYear;
    int CurrQuarter;
    int CurrMonth;
    TDateTime CurrDate;
    std::string aShablonKBK;
    int aId_Owner_link1n;
    int user_id;
    TDateTime dataLastInstanceUsed;

    TActionView* ActionView;
    TMyBaseArray* FormsValuesArray;

    TStringList* ParamFormObozns;
    TStringList* ParamFormObjNames;

    F1DevExpress::unF1FlexCel::TF1FlexCel* f1Forms;

    TQueryUni* quForms;
    TQueryUni* quFormStr;
    TFormDM* FormDM;
    TquCell* quCell;

    int CurrSheet;
    int CurrInputRow;

    bool isDoSelChangeVspomogat;

    TFormLogicF1() {
        ActionView = new TActionView();
        FormDM = new TFormDM();
        quCell = new TquCell();
        FormsValuesArray = new TMyBaseArray();
        f1Forms = new TF1FlexCel();
        CreateGlobalVars();
        ParamFormObozns = new TStringList();
        //ParamFormObozns->sorted = true;
        ParamFormObjNames = new TStringList();
        //ParamFormObjNames->sorted = true;

    };
    ~TFormLogicF1() {
        // ������������ �������� ������ FormsValuesArray, ���� ��� ����������
        for (int i = FormsValuesArray->Size() - 1; i >= 0; i--) {
            ClearListsForForm(i + 1);
        }
        FormsValuesArray->Clear();

        // ������������ ������ ��� ������ ������
        delete FormsValuesArray;
        FormsValuesArray = nullptr;

        delete FormDM;
        FormDM = nullptr;

        delete quCell;
        quCell = nullptr;

        if (quForms != nullptr) {
            delete quForms;
            quForms = nullptr;
        }

        delete ParamFormObozns;
        ParamFormObozns = nullptr;

        delete ParamFormObjNames;
        ParamFormObjNames = nullptr;

        delete f1Forms;
        f1Forms = nullptr;

        delete ActionView;
        ActionView = nullptr;

        DeleteGlobalVars();
    }

    bool NewForm1(TInfoFormLogic* InfoFormLogic, unsigned char IsParamForm, int ParamObjectId, std::string ParamObjectName, int FormId, int FormOrder, TwwStringList* p);
    bool InitFormChart(int IsParamForm, int ParamObjectId, std::string ParamObjectName, int FormId, TwwStringList* p);
    bool InitFormObject(TInfoFormLogic *InfoFormLogic, unsigned char IsParamForm, int ParamObjectId, string ParamObjectName, int FormId, TwwStringList* p);

    void OpenForm(bool isCurForm, int CurrAlbumId, TInfoAlbumLogic InfoAlbumLogic, TInfoFormLogic InfoFormLogic, unsigned char IsParamForm, int ParamObjectId, std::string ParamObjectName, int FormId, int FormOrder, int ownerFormPlanId = 0, bool loadContent = true);
    void CloseForm(int NSheet);
    void CloseAlbum();
    void UpdExecuteCellsAnotherSheet(int nSheet, bool noSetActiveCell, nlohmann::json& JSONresult);
    bool UpdExecuteCellsEndInput(int nSheet, int aNRow, int aNCol, bool noSetActiveCell, nlohmann::json& JSONresult);

    void updNameinF1(int nSheet, int nRow, std::string newName);
    void setInCellStr(int nSheet, int nRow, int objectid, int field_id, std::string str_value);
    void setInCellNmbr(int nSheet, int nRow, int objectid, int field_id, int ist_fin_id, int VidOtch, double value);
    void SetCellValueStr(int nSheet, int nRow, int nCol, std::string str_value);
    void setInRowNmbr(int nSheet, int nRow, int objectid);

    void reDraw(int nSheet, int nRow);

    bool insertObjectToF1(int nSheet, int OwnerId, int object_id, std::string object_name, int objekt_class_id, std::string KBK, int level_sign, int& aInsertRow);
    bool insertPokaztlToF1(int nSheet, int OwnerId, int pokaz_id, std::string pokaz_name, std::string KBK, int pokaz_type, int enum_id, int edizm_id, int level_sign, int& aInsertRow);

    bool doStmntAssignByVvodPokaztl(int nSheet, int nRow, int nCol, int nColVvod);

    void delRowByPokaz(int nSheet, int nRow, int nCol);
    bool XlsToStrJSON(int nSheet, nlohmann::json& wsXlsToStrJSON);

    TwwStringList* GetTww();
    TwwStringList* GetTwwByIndex(int index);
    bool CellValueAtr(int nSheet, int nRow, int nCol, json& JSONresult);
    bool CellEndingEdit(int nSheet, int nRow, int nCol, const std::string editValue, int valueID, nlohmann::json& JSONresult);
    int insertPokaztlToF1Back(int nSheet, int nRowSel, bool isSub, int plan_id, bool isItogo,
                               int pokaz_is_razd, int pokaz_owner_id, int in_cast, const std::string& pokaz_name,
                               const std::string& Primecanie, int is_list, int level_sign, const std::string& level_text,
                               int pokaz_vert, const std::string& vert_str, int pokaz_type, const std::string& pokaz_code,
                               const std::string& pokaz_bk_code, const TDateTime& pokaz_beg_date, const TDateTime& pokaz_end_date,
                               int enum_id, int edizm_id, int pokaz_is_const, int pokaz_is_god, int pokaz_is_kv,
                               int pokaz_is_mes, int pokaz_is_ist_fin, int pokaz_is_din_izm);
    bool UpdPokazMrprObj(int pokaz_id,std::string pokaz_name, std::string primecanie);
    bool delDefRowByPokaz(int nSheet, int nRow, int nCol, std::string& msgError);

private:
    bool doOpenForm(TInfoFormLogic* InfoFormLogic, unsigned char IsParamForm, int ParamObjectId, string ParamObjectName, int FormId, int FormOrder, int ownerFormPlanId = 0, bool loadContent = true);
    void doCloseForm(int NSheet);

    void UpdFormatStyle(std::string aRzd1font, int aRzd1h, int aRzd1is_bold, int aRzd1fontColor, int aRzd1FonColor);
};
#endif

#ifndef MWSRESTAPI_DMGLOBALVARS_H
#define MWSRESTAPI_DMGLOBALVARS_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "unDefDBconnect.h"
#include "unDBType.h"
#include "ClassesUtils.h"
#include "DB_Common.h"
#include "unDB_RP_067eval.h"
#include "unEkonomijBPSeval.h"
#include "unCO_RP_015eval.h"
#include "TT_PostuplForPeriod.h"
#include "TT_SummaBySubjects.h"
#include "TT_ValForPeriod.h"
#include "TT_ValForMesjc.h"

extern std::string endpointNg;
extern std::string ServiceName;
extern std::string authHeaderGlob;
extern std::string FromService;

using namespace std;

// Main
const int PostgreKolElem = 1;
const int KolElem = 41;

struct TVidInfo {
    int vid;
    int beg_God;
    int end_God;
    int nacalo_vKBK;
    int dlina_vKBK;
};

enum TFinalActionType { Nothing, postMsgCol, postMsgForm, Update, SaveEditString, stmntAssignVvodZnacenij, stmntAssignVvodPokaztl };

struct TFinalAction {
    TFinalActionType ActionType;
    int ActionTag;
    int nRow, nCol;
    string EditString;
    double oldValue;

    TFinalAction(TFinalActionType FinalActionType) : ActionType(FinalActionType), ActionTag(0) {}
    TFinalAction(TFinalActionType FinalActionType, int ActionTag) : ActionType(FinalActionType), ActionTag(ActionTag) {}
    TFinalAction(TFinalActionType FinalActionType, int aRow, int aCol, string EditString, double oldValue) : ActionType(FinalActionType), ActionTag(0), nRow(aRow), nCol(aCol), EditString(EditString), oldValue(oldValue) {}
};

class TFinalActions : public vector<TFinalAction*> {
public:
    void doClear() {
        for (auto action : *this) {
            delete action;
        }
        clear();
    }
};

///////////////////////////////////////////////
// символические номера пиктограмм для IconList16 - 16 на 16
const int iconIndex16_Expand = 0; // раскрыть данные
const int iconIndex16_ECP_key16px = 1; // электронно-цифровая подпись
const int iconIndex16_SelectCol = 2; // Выбор столбца
const int iconIndex16_UnVisibleCol = 3; // Невидимый столбец
// символические номера пиктограмм для IconList16 - 24 на 24
const int iconIndex24_Expand = 0; // Раскрытие детальных данных
// символические номера пиктограмм для IconList16 - 64 на 64
const int iconIndex64_ECP_1key = 0; // электронно-цифровая подпись
const int iconIndex64_ECP_2keys = 1; // электронно-цифровая подпись
const int iconIndex64_ECP_3keys = 2; // электронно-цифровая подпись
///////////////////////////////////////////////

class TGlobalVars {
public:
//    std::unique_ptr<TImageList> IconList16;
//    std::unique_ptr<TImageList> IconList24;
//    std::unique_ptr<TImageList> IconList64;
//    std::unique_ptr<TImageList> IconListExcel;

    std::string UserSessionId;
    TFinalActions FinalActions;

    std::string PasswordSysadm;
    std::string NameSysadm;

    //mak ??? TList ListWnd;
    std::unique_ptr<TObject> curForm;
    const wchar_t* MsgVnim;
    std::wstring wMsgVnim;
    int ToolBar_Height;
    int ToolBar_y;
    int UserId;
    int UserAdm;
    std::string UserName;
    int UserctgrId;
    std::string UserctgrObozn;
    std::string Dostupn_funcii;
    int isNoNasledMenu;
    int StrMenuFormUslovie;
    uint8_t MenuInLine;
    int UserctgrGod;
    int MenuVariant;
    std::string CurNameMenu1;
    std::string CurNameMenu2;
    std::string PlanShortName;
    std::string nameExcel;
    std::string key_value;
    TDBType DBType;
    std::string ServerName;
    std::string ServerLogin;
    std::string ServerPassword;
    std::string ServerPort;

    std::string MWSUserName;
    std::string MWSPassword;
    std::string MWSserverName;
    std::string MWSPort;
    std::string MWSdatabaseName;
    bool isMWSDBpostgre;
    bool isDbOpenBySysadm__;
    int count;
    std::string AliasName; // Это алиас оперативной БД
    std::string UserName2, UserPassword;
    bool isDocumConected;
    int Year, Month, Quart;
    std::string name_plan;
    int idSelPlan;
    int accessalbum_idSelBlockSubj;
    int PermSelPlan_forUser;
    int idOwnerPlan;
    int idSelAlbum;
    std::string album_obozn;
    int plan_id_fiks;
    int plan_id_Pref;

    std::unique_ptr<TQueryUni> quFormStr_ColW;
    std::unique_ptr<TQueryUni> quFormStrWidth;
    std::unique_ptr<TQueryUni> quFormStrText;
    std::unique_ptr<TQueryUni> quFormStrColDest;

    int PredEdizmIdField;
    std::string PredEdizmNameField;

    int class_Dog_id;
    int link_pokaz_id;
    int field_Finansir_id;
    int field_Finansir_cellidTB_KV;
    int ist_fin_grad_id;
    int N_BO_id;
    int N_BO_cellid;
    int N_dgvr_id;
    int N_dgvr_cellid;
    int PostUslug_id;
    int PostUslug_cellid;
    int Data_zakluc_id;
    int Data_zakluc_cellid;
    int Srok_deistv_id;
    int Srok_deistv_cellid;
    int soderg_dgvr_id;
    int soderg_dgvr_cellid;
    int INN_id;
    int INN_cellid;
    int UslOplati_id;
    int UslOplati_cellid;
    int EAIST_id;
    int EAIST_cellid;

    bool isArLoad = false;
    std::vector<uint8_t> arUserPlanPerm;

    std::unique_ptr<TStringList> stmntList;

    int pokaz_id_;
    std::string pokaz_bk_code_;

    bool isCreateTmpOrderCP;
    std::unique_ptr<TStringList> slCreateTmpOrderCPNames;

    std::string wwEnum_name;
    int wwEnum_id;

    std::string PasswordSysadm_;
    std::string NameSysadm_;

    std::vector<TVidInfo> arVidInfo; // Пример структуры TVidInfo должен быть определен
    bool isLoadVidInfo;
    int NkolVidInfo;

    int nMWSModulsOpens;

    // Дополнительные поля
    int PredEdizmIdPokaz;
    std::string PredEdizmNamePokaz;
    std::string PrededCodePokaz;
    std::string PrededBKPokaz;

    int PredEdizmIdSepPokaz;
    std::string PredEdizmNameSepPokaz;
    std::string PrededCodeSepPokaz;
    std::string PrededBKSepPokaz;

    // fmPokazBK
    int PredEdizmIdPokazBK;
    std::string PredEdizmNamePokazBK;

    // fmMrpr
    int PredEdizmIdMrpr;
    std::string PredEdizmNameMrpr;
    std::string PrededBKMrpr;

    // BPolucList
    int wBP_id;

    // uFmHelp
//vernut      HANDLE HHCtrlHandle;

    // DB_EcoSUFDswap
    // поля, предназначенные для подсчета открытых окон или файлов, будут добавлены позже

    // FormWord
    bool PrintAlbumName;
    bool NoPrintEmptyStr;
    bool isObjdinit;
    bool isPageOrientation;

    // FormSsilkaNaStr
    std::unique_ptr<TStringList> slSsilkaNaStr;

    // TT_ValForMesjc
      TValForMesjc ValForMesjc;

    // TT_PostuplForPeriod
      TPostuplForPeriod PostuplForPeriod;

    // TT_ValForPeriod
    TValForPeriod ValForPeriod;

    // TT_ValForPeriodAgregat
    std::unique_ptr<TStringList> slAgregat;
    //mak ??? std::unique_ptr<TList> planIDlist;

    // TT_Korteg
    std::unique_ptr<TStringList> slPlcKorteg;
    std::unique_ptr<TStringList> slRspKorteg;

    // TT_SummaBySubjects
    TSummaBySubjects SummaBySubjects;

    // Остальные поля
    int glbGod;
    std::string glbStrPlans;

    // unDB_RP_067eval
    TDB_RP_067eval vDB_RP_067eval;

    // unCO_RP_015eval
    TCO_RP_015eval vCO_RP_015eval;

    // unEkonomijBPSeval
    TEkonomijBPSeval vEkonomijBPSeval;

    // unIDsClassFieldCell;
    std::unique_ptr<TStringList> slIDsClassFieldCell;
    std::unique_ptr<TDictionary<std::string, std::string>> slNamesClassFieldCell;

    // Фильтр КБК
    std::string KBKfilter;

    // CopyFormImportExcel
//vernut      TCopyFormImportExcel CopyFormImportExcel;

    // Form2loadValue
    TStringList* slStmntOboznac1;
    TStringList* slStmntOboznac2;

    TGlobalVars()
            : isDbOpenBySysadm__(false),
              count(0),
              AliasName(""),
              UserName2(""),
              UserPassword(""),
              isDocumConected(false),
              Year(0),
              Month(0),
              Quart(0),
              name_plan(""),
              idSelPlan(0),
              accessalbum_idSelBlockSubj(0),
              PermSelPlan_forUser(0),
              idOwnerPlan(0),
              idSelAlbum(0),
              album_obozn(""),
              plan_id_fiks(0),
              plan_id_Pref(0),
              PredEdizmIdField(0),
              PredEdizmNameField(""),
              class_Dog_id(0),
              link_pokaz_id(0),
              field_Finansir_id(0),
              field_Finansir_cellidTB_KV(0),
              ist_fin_grad_id(0),
              N_BO_id(0),
              N_BO_cellid(0),
              N_dgvr_id(0),
              N_dgvr_cellid(0),
              PostUslug_id(0),
              PostUslug_cellid(0),
              Data_zakluc_id(0),
              Data_zakluc_cellid(0),
              Srok_deistv_id(0),
              Srok_deistv_cellid(0),
              soderg_dgvr_id(0),
              soderg_dgvr_cellid(0),
              INN_id(0),
              INN_cellid(0),
              UslOplati_id(0),
              UslOplati_cellid(0),
              EAIST_id(0),
              EAIST_cellid(0),
              isArLoad(false),
              arUserPlanPerm(5000, 0),  // Создание вектора размером 5000, инициализированного нулями
              pokaz_id_(0),
              pokaz_bk_code_(""),
              isCreateTmpOrderCP(false),
              wwEnum_id(0),
              isLoadVidInfo(false),
              NkolVidInfo(0),
              nMWSModulsOpens(0),
              glbGod(0),
              PrintAlbumName(false),
              NoPrintEmptyStr(false),
              isObjdinit(false),
              isPageOrientation(false),
              KBKfilter(""),
              glbStrPlans("")
    {
        // Инициализация умных указателей
        //mak ??? ListWnd = std::make_unique<TList>();
        curForm = nullptr; // Или другая подходящая инициализация
        quFormStr_ColW = std::make_unique<TQueryUni>(DefDBconnect.Conn);
        quFormStrWidth = std::make_unique<TQueryUni>(DefDBconnect.Conn);
        quFormStrText = std::make_unique<TQueryUni>(DefDBconnect.Conn);
        quFormStrColDest = std::make_unique<TQueryUni>(DefDBconnect.Conn);
        stmntList = std::make_unique<TStringList>();
        slCreateTmpOrderCPNames = std::make_unique<TStringList>();
        slSsilkaNaStr = std::make_unique<TStringList>();
        slAgregat = std::make_unique<TStringList>();
        //mak ??? planIDlist = std::make_unique<TList>();
        slPlcKorteg = std::make_unique<TStringList>();
        slRspKorteg = std::make_unique<TStringList>();
        slIDsClassFieldCell = std::make_unique<TStringList>();
        slNamesClassFieldCell = std::make_unique<TDictionary<std::string, std::string>>();
        slStmntOboznac1 = new TStringList();
        slStmntOboznac2 = new TStringList();


    }

    ~TGlobalVars() {
        // Удаление slStmntOboznac1
        isArLoad = false;
        if (slStmntOboznac1) {
            // Очистка списка
            slStmntOboznac1->Clear();

            // Удаление объекта
            delete slStmntOboznac1;
            slStmntOboznac1 = nullptr;
        }

        // Удаление slStmntOboznac2
        if (slStmntOboznac2) {
            if (slStmntOboznac2->ownsObjects) {
                // Очистка списка
                slStmntOboznac2->Clear();
            }

            // Удаление объекта
            delete slStmntOboznac2;
            slStmntOboznac2 = nullptr;
        }
    }
};

extern TGlobalVars* GlobalVars;





#endif //MWSRESTAPI_DMGLOBALVARS_H

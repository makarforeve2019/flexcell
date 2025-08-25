#include "DB_Dgvr.h"
#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "SystFuncsLogic.h"
#include "DB_PlanStr.h"
#include "DB_Cell.h"
#include "DB_IstFin.h"
#include "DB_NMrpr.h"
#include "DB_UnikNmbrDgvr.h"
#include "DB_StrPodrsd.h"
#include "DB_CellValue.h"
#include "dmGlobalVars.h"


void TDB_Dgvr::SetCodeBK(int object_id, std::string pokaz_bk_code) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_bk_code = :pokaz_bk_code");
    q->SQL->Add("WHERE pokaz_id = :object_id");
    
    q->ParamByName("object_id")->AsInteger() = object_id;
    q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;
    
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

int TDB_Dgvr::GetIdClassDgvr() {
    if (GlobalVars->class_Dog_id == 0) 
        GlobalVars->class_Dog_id = TDB_Pokaztl::GetIdClass("Договор", true);
    
    return GlobalVars->class_Dog_id;
}

int TDB_Dgvr::GetIdClassDgvr1() {
      if (GlobalVars->class_Dog_id == 0) 
        GlobalVars->class_Dog_id = TDB_Pokaztl::GetIdClass("Договор", false);
    
    return GlobalVars->class_Dog_id;
}

int TDB_Dgvr::GetIdClass(std::string& class_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_name = :pokaz_name");
    q->SQL->Add("AND pokaz_type = 1");
    q->SQL->Add("AND pokaz_owner_id = 0");
    q->ParamByName("pokaz_name")->AsString() = class_name;
    q->Open();
    
    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    } else {
        //--SystFuncs.MessageBoxExclamation(PChar('В метабазе отсутствует описание класса "'+class_name+'". Следует обратиться к Разработчику'),
        //--    MsgVnim);
        result = 0;
    }
    
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Dgvr::GetIdFieldMeropr() {
    TQueryUni *qFld = new TQueryUni(DefDBconnect.Conn);

    auto GetField_id = [&](const std::string& field_name) -> int {
        qFld->First();
        int result = 0;
        while (!qFld->EoF()) {
            if (qFld->FieldByName("pokaz_name").AsString() == field_name) {
                result = qFld->FieldByName("pokaz_id").AsInteger();
                break;
            }
            qFld->Next();
        }
        if (result == 0) {
            throw std::runtime_error("В классе Договор отсутствует поле " + field_name);
        }
        return result;
    };

    int result = 0;
    if (GlobalVars->link_pokaz_id > 0) {
        result = GlobalVars->link_pokaz_id;
    } else {
        GetIdClassDgvr();
        if (GlobalVars->class_Dog_id == 0) {
            return result;
        }
        TDB_PlanStr::GetAllSubClassItem(qFld, GlobalVars->class_Dog_id, Date());
        GlobalVars->link_pokaz_id = GetField_id("Мероприятие");
        GlobalVars->field_Finansir_id = GetField_id("Объем финансирования");
        result = GlobalVars->link_pokaz_id;
    }
    
    qFld->Free();
    return result;
}

int TDB_Dgvr::GetIdFieldObjemFin() {
    TDB_Dgvr::GetIdFieldMeropr();
    return GlobalVars->field_Finansir_id;
}

int TDB_Dgvr::GetField_id(TQueryUni* qFld, std::string field_name) {
    qFld->First();
    int result = 0;
    while (!qFld->EoF()) {
        if (qFld->FieldByName("pokaz_name")->AsString() == field_name) {
            result = qFld->FieldByName("pokaz_id")->AsInteger();
            break;
        }
        qFld->Next();
    }
    if (result == 0) {
        throw std::runtime_error("В классе Договор отсутствует поле " + field_name);
    }
    return result;
}

void TDB_Dgvr::GetIdsFieldsDGVR(int& aN_BO_id, int& aN_BO_cellid,
                                 int& aN_dgvr_id, int& aN_dgvr_cellid,
                                 int& aPostUslug_id, int& aPostUslug_cellid,
                                 int& afield_Finansir_id, int& afield_Finansir_cellidTB_KV,
                                 int& aData_zakluc_id, int& aData_zakluc_cellid,
                                 int& aSrok_deistv_id, int& aSrok_deistv_cellid,
                                 int& asoderg_dgvr_id, int& asoderg_dgvr_cellid,
                                 int& aINN_id, int& aINN_cellid) {
    if (GlobalVars->N_BO_id > 0) {
        aN_BO_id = GlobalVars->N_BO_id;
        aN_BO_cellid = GlobalVars->N_BO_cellid;
        aN_dgvr_id = GlobalVars->N_dgvr_id;
        aN_dgvr_cellid = GlobalVars->N_dgvr_cellid;
        aPostUslug_id = GlobalVars->PostUslug_id;
        aPostUslug_cellid = GlobalVars->PostUslug_cellid;
        afield_Finansir_id = GlobalVars->field_Finansir_id;
        afield_Finansir_cellidTB_KV = GlobalVars->field_Finansir_cellidTB_KV;
        aData_zakluc_id = GlobalVars->Data_zakluc_id;
        aData_zakluc_cellid = GlobalVars->Data_zakluc_cellid;
        aSrok_deistv_id = GlobalVars->Srok_deistv_id;
        aSrok_deistv_cellid = GlobalVars->Srok_deistv_cellid;
        asoderg_dgvr_id = GlobalVars->soderg_dgvr_id;
        asoderg_dgvr_cellid = GlobalVars->soderg_dgvr_cellid;
        aINN_id = GlobalVars->INN_id;
        aINN_cellid = GlobalVars->INN_cellid;
        return;
    }

    TDB_Dgvr::GetIdClassDgvr();
    if (GlobalVars->class_Dog_id == 0) return;
    GlobalVars = new TGlobalVars();
    TQueryUni *qFld = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr::GetAllSubClassItem(qFld, GlobalVars->class_Dog_id, Date());

    GlobalVars->link_pokaz_id = TDB_Dgvr::GetField_id(qFld, "Мероприятие");
    GlobalVars->N_BO_id = TDB_Dgvr::GetField_id(qFld, "Номер Бюджетного обязательства");
    aN_BO_id = GlobalVars->N_BO_id;
    GlobalVars->N_BO_cellid = TDB_Cell::GetCellId(GlobalVars->N_BO_id, 0, 0);
    aN_BO_cellid = GlobalVars->N_BO_cellid;
    GlobalVars->N_dgvr_id = TDB_Dgvr::GetField_id(qFld, "Номер договора");
    aN_dgvr_id = GlobalVars->N_dgvr_id;
    GlobalVars->N_dgvr_cellid = TDB_Cell::GetCellId(GlobalVars->N_dgvr_id, 0, 0);
    aN_dgvr_cellid = GlobalVars->N_dgvr_cellid;
    GlobalVars->PostUslug_id = TDB_Dgvr::GetField_id(qFld, "Поставщик услуг");
    aPostUslug_id = GlobalVars->PostUslug_id;
    GlobalVars->PostUslug_cellid = TDB_Cell::GetCellId(GlobalVars->PostUslug_id, 0, 0);
    aPostUslug_cellid = GlobalVars->PostUslug_cellid;
    GlobalVars->field_Finansir_id = TDB_Dgvr::GetField_id(qFld, "Объем финансирования");
    afield_Finansir_id = GlobalVars->field_Finansir_id;
    GlobalVars->field_Finansir_cellidTB_KV = TDB_Cell::GetCellId(GlobalVars->field_Finansir_id, 2, ist_fin_id_TrBudg);
    afield_Finansir_cellidTB_KV = GlobalVars->field_Finansir_cellidTB_KV;
    GlobalVars->Data_zakluc_id = TDB_Dgvr::GetField_id(qFld, "Дата заключения");
    aData_zakluc_id = GlobalVars->Data_zakluc_id;
    GlobalVars->Data_zakluc_cellid = TDB_Cell::GetCellId(GlobalVars->Data_zakluc_id, 0, 0);
    aData_zakluc_cellid = GlobalVars->Data_zakluc_cellid;
    GlobalVars->Srok_deistv_id = TDB_Dgvr::GetField_id(qFld, "Срок действия");
    aSrok_deistv_id = GlobalVars->Srok_deistv_id;
    GlobalVars->Srok_deistv_cellid = TDB_Cell::GetCellId(GlobalVars->Srok_deistv_id, 0, 0);
    aSrok_deistv_cellid = GlobalVars->Srok_deistv_cellid;
    GlobalVars->soderg_dgvr_id = TDB_Dgvr::GetField_id(qFld, "Содержание договора");
    asoderg_dgvr_id = GlobalVars->soderg_dgvr_id;
    GlobalVars->soderg_dgvr_cellid = TDB_Cell::GetCellId(GlobalVars->soderg_dgvr_id, 0, 0);
    asoderg_dgvr_cellid = GlobalVars->soderg_dgvr_cellid;
    GlobalVars->INN_id = TDB_Dgvr::GetField_id(qFld, "ИНН исполнителя");
    aINN_id = GlobalVars->INN_id;
    GlobalVars->INN_cellid = TDB_Cell::GetCellId(GlobalVars->INN_id, 0, 0);
    aINN_cellid = GlobalVars->INN_cellid;

    // ПРИ ДОБАВЛЕНИИ НОВЫХ ПОЛЕЙ обязательно изменить присвоениев начале этой процедуры

    //:=TDB_Dgvr::GetField_id(qFld, "Вид оплаты");
    //:=TDB_Dgvr::GetField_id(qFld, "График оплаты");

    qFld->Free();
}

void TDB_Dgvr::GetIdFieldUslOplati(int& aUslOplati_id, int& aUslOplati_cellid) {
    if (GlobalVars->UslOplati_id > 0) {
        aUslOplati_id = GlobalVars->UslOplati_id;
        aUslOplati_cellid = GlobalVars->UslOplati_cellid;
        return;
    }

    TDB_Dgvr::GetIdClassDgvr();
    if (GlobalVars->class_Dog_id == 0) return;

    TQueryUni *qFld = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr::GetAllSubClassItem(qFld, GlobalVars->class_Dog_id, Date());

    GlobalVars->UslOplati_id = TDB_Dgvr::GetField_id(qFld, "Условия оплаты");
    aUslOplati_id = GlobalVars->UslOplati_id;
    GlobalVars->UslOplati_cellid = TDB_Cell::GetCellId(GlobalVars->UslOplati_id, 0, 0);
    aUslOplati_cellid = GlobalVars->UslOplati_cellid;

    qFld->Free();
}

void TDB_Dgvr::GetIdFieldEAIST(int& aEAIST_id, int& aEAIST_cellid) {
    if (GlobalVars->EAIST_id > 0) {
        aEAIST_id = GlobalVars->EAIST_id;
        aEAIST_cellid = GlobalVars->EAIST_cellid;
        return;
    }

    TDB_Dgvr::GetIdClassDgvr();
    if (GlobalVars->class_Dog_id == 0) return;

    TQueryUni *qFld = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr::GetAllSubClassItem(qFld, GlobalVars->class_Dog_id, Date());

    GlobalVars->EAIST_id = TDB_Dgvr::GetField_id(qFld, "ЕАИСТ");
    aEAIST_id = GlobalVars->EAIST_id;
    GlobalVars->EAIST_cellid = TDB_Cell::GetCellId(GlobalVars->EAIST_id, 0, 0);
    aEAIST_cellid = GlobalVars->EAIST_cellid;

    qFld->Free();
}

int TDB_Dgvr::GetFieldBP(int ObjectId, int& cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT Cell.pokaz_id, Cell.cell_id");
    q->SQL->Add("FROM Pokaztl as A, Pokaztl as B, Cell");
    q->SQL->Add("WHERE A.pokaz_id = :ObjectId");
    q->SQL->Add("AND B.pokaz_owner_id = A.objekt_class_id");
    q->SQL->Add("AND B.pokaz_type = 4");
    q->SQL->Add("AND B.enum_id = 1");
    q->SQL->Add("AND Cell.pokaz_id = B.pokaz_id");
    q->SQL->Add("AND cell_period = 0");
    q->SQL->Add("AND ist_fin_id = 0");
    q->ParamByName("ObjectId")->AsInteger() = ObjectId;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
        cell_id = q->FieldByName("cell_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_Dgvr::GetDgvrByKBK(const std::string& KBK, int plan_id, int god, TStringList* aSl) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT A.pokaz_id, pokaz_name");
    q->SQL->Add("FROM Pokaztl as A, Plan_str as B");
    q->SQL->Add("WHERE B.plan_id = :plan_id");
    q->SQL->Add("AND B.pokaz_id = A.pokaz_id");
    q->SQL->Add("AND A.pokaz_is_razd = 4");
    q->SQL->Add("AND A.objekt_class_id = :class_Dog_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :god BETWEEN Year(A.pokaz_beg_date) AND Year(A.pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :god BETWEEN date_part('year', A.pokaz_beg_date) AND date_part('year', A.pokaz_end_date)");
            break;
    }
    q->SQL->Add("AND A.pokaz_bk_code = :KBK");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("class_Dog_id")->AsInteger() = TDB_Dgvr::GetIdClassDgvr();
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("KBK")->AsString() = KBK;
    q->Open();
    while (!q->EoF()) {
        aSl->AddObject(q->FieldByName("pokaz_name")->AsString(), q->FieldByName("pokaz_id")->AsInteger());
        q->Next();
    }
   //MAK q->Free();
delete q;
}

int TDB_Dgvr::GetKolDgvrByKBK(const std::string& KBK, int plan_id, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT COUNT(*) as count");
    q->SQL->Add("FROM Pokaztl as A, Plan_str as B");
    q->SQL->Add("WHERE B.plan_id = :plan_id");
    q->SQL->Add("AND B.pokaz_id = A.pokaz_id");
    q->SQL->Add("AND A.pokaz_is_razd = 4");
    q->SQL->Add("AND A.objekt_class_id = :class_Dog_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :god BETWEEN Year(A.pokaz_beg_date) AND Year(A.pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :god BETWEEN date_part('year', A.pokaz_beg_date) AND date_part('year', A.pokaz_end_date)");
            break;
    }
    q->SQL->Add("AND A.pokaz_bk_code = :KBK");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("class_Dog_id")->AsInteger() = TDB_Dgvr::GetIdClassDgvr();
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("KBK")->AsString() = KBK;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("count")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Dgvr::GetKolDgvrByKBK_NBO(const std::string& KBK, int plan_id, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT COUNT(*) as count");
    q->SQL->Add("FROM Pokaztl as A, Plan_str as B, KeyOfObjects as C");
    q->SQL->Add("WHERE B.plan_id = :plan_id");
    q->SQL->Add("AND B.pokaz_id = A.pokaz_id");
    q->SQL->Add("AND A.pokaz_is_razd = 4");
    q->SQL->Add("AND A.objekt_class_id = :class_Dog_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :god BETWEEN Year(A.pokaz_beg_date) AND Year(A.pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :god BETWEEN date_part('year', A.pokaz_beg_date) AND date_part('year', A.pokaz_end_date)");
            break;
    }
    q->SQL->Add("AND A.pokaz_bk_code = :KBK");
    q->SQL->Add("AND A.pokaz_id = C.objectid");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("class_Dog_id")->AsInteger() = TDB_Dgvr::GetIdClassDgvr();
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("KBK")->AsString() = KBK;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("count")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

double TDB_Dgvr::GetSumBOByMrpr(int pokaz_id, int plan_id, int god, int vid_otc) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0.0;
    TDateTime DateGod;
    int wFinansir_id;
    int wFinansir_cellidTB_KV;
    int wFinansir_cellidTB_God;
    int wlink_pokaz_id;
    TDB_Dgvr::GetField_id_forOFinansir(wFinansir_id, wFinansir_cellidTB_KV);
    wFinansir_cellidTB_God = wFinansir_cellidTB_KV - 1;
    wlink_pokaz_id = TDB_Dgvr::GetIdFieldMeropr(); // поле Договора - мероприятие
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM LinksObjects, Cell_value");
    q->SQL->Add("WHERE LinksObjects.plan_id = :plan_id");
    q->SQL->Add("AND LinksObjects.pokaz_id = :pokaz_id");
    q->SQL->Add("AND LinksObjects.link_pokaz_id = :link_pokaz_id");
    q->SQL->Add("AND Cell_value.objectid = LinksObjects.objectid");
    q->SQL->Add("AND Cell_value.cell_id = :cell_id");
    q->SQL->Add("AND Cell_value.plan_id = :plan_id2");
    q->SQL->Add("AND Cell_value.vid_otc = :vid_otc");
    q->SQL->Add("AND Cell_value.value_date  = :value_date");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = wlink_pokaz_id;
    q->ParamByName("plan_id2")->AsInteger() = plan_id;
    q->ParamByName("cell_id")->AsInteger() = wFinansir_cellidTB_God;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    DateGod = EncodeDate(god, 12, 31);
    q->ParamByName("value_date")->AsDateTime() = DateGod;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("value")->AsFloat();
    }
   //MAK q->Free();
delete q;
    return result;
}

double TDB_Dgvr::GetSumNumBOByMrpr(int pokaz_id, int plan_id, int god, int vid_otc) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0.0;
    TDateTime DateGod;
    int wFinansir_id;
    int wFinansir_cellidTB_KV;
    int wFinansir_cellidTB_God;
    int wlink_pokaz_id;
    TDB_Dgvr::GetField_id_forOFinansir(wFinansir_id, wFinansir_cellidTB_KV);
    wFinansir_cellidTB_God = wFinansir_cellidTB_KV - 1;
    wlink_pokaz_id = TDB_Dgvr::GetIdFieldMeropr(); // поле Договора - мероприятие
    result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM LinksObjects, Cell_value, KeyOfObjects");
    q->SQL->Add("WHERE LinksObjects.plan_id = :plan_id");
    q->SQL->Add("AND LinksObjects.pokaz_id = :pokaz_id");
    q->SQL->Add("AND LinksObjects.link_pokaz_id = :link_pokaz_id");
    q->SQL->Add("AND KeyOfObjects.objectid = LinksObjects.objectid");
    q->SQL->Add("AND Cell_value.objectid = LinksObjects.objectid");
    q->SQL->Add("AND Cell_value.cell_id = :cell_id");
    q->SQL->Add("AND Cell_value.plan_id = :plan_id2");
    q->SQL->Add("AND Cell_value.vid_otc = :vid_otc");
    q->SQL->Add("AND Cell_value.value_date  = :value_date");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = wlink_pokaz_id;
    q->ParamByName("plan_id2")->AsInteger() = plan_id;
    q->ParamByName("cell_id")->AsInteger() = wFinansir_cellidTB_God;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    DateGod = EncodeDate(god, 12, 31);
    q->ParamByName("value_date")->AsDateTime() = DateGod;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("value")->AsFloat();
    }
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_Dgvr::GetField_id_forNDgvr(int& aN_dgvr_id, int& aN_dgvr_cellid) {
    int N_BO_id, N_BO_cellid;
    int N_dgvr_id, N_dgvr_cellid;
    int PostUslug_id, PostUslug_cellid;
    int field_Finansir_id, field_Finansir_cellidTB_KV;
    int Data_zakluc_id, Data_zakluc_cellid;
    int Srok_deistv_id, Srok_deistv_cellid;
    int soderg_dgvr_id, soderg_dgvr_cellid;
    int INN_id, INN_cellid;

    TDB_Dgvr::GetIdsFieldsDGVR(N_BO_id, N_BO_cellid, N_dgvr_id, N_dgvr_cellid,
                                    PostUslug_id, PostUslug_cellid, field_Finansir_id, field_Finansir_cellidTB_KV,
                                    Data_zakluc_id, Data_zakluc_cellid,
                                    Srok_deistv_id, Srok_deistv_cellid,
                                    soderg_dgvr_id, soderg_dgvr_cellid,
                                    INN_id, INN_cellid);

    aN_dgvr_id = N_dgvr_id;
    aN_dgvr_cellid = N_dgvr_cellid;

    return true;
}

bool TDB_Dgvr::GetField_id_forOFinansir(int& aFinansir_id, int& aFinansir_cellidTB_KV) {
    int N_BO_id, N_BO_cellid;
    int N_dgvr_id, N_dgvr_cellid;
    int PostUslug_id, PostUslug_cellid;
    int field_Finansir_id, field_Finansir_cellidTB_KV;
    int Data_zakluc_id, Data_zakluc_cellid;
    int Srok_deistv_id, Srok_deistv_cellid;
    int soderg_dgvr_id, soderg_dgvr_cellid;
    int INN_id, INN_cellid;

    TDB_Dgvr::GetIdsFieldsDGVR(N_BO_id, N_BO_cellid, N_dgvr_id, N_dgvr_cellid,
                                    PostUslug_id, PostUslug_cellid, field_Finansir_id, field_Finansir_cellidTB_KV,
                                    Data_zakluc_id, Data_zakluc_cellid,
                                    Srok_deistv_id, Srok_deistv_cellid,
                                    soderg_dgvr_id, soderg_dgvr_cellid,
                                    INN_id, INN_cellid);

    aFinansir_id = field_Finansir_id;
    aFinansir_cellidTB_KV = field_Finansir_cellidTB_KV;

    return true;
}

std::string TDB_Dgvr::getNmbrDgvr(int aCurPokazId, int aobjekt_class_id, int aCurYear, int aPlan_id) {
    int UnikNmbrDgvr = TDB_UnikNmbrDgvr::GetNextUnikNmbr(aCurYear, aPlan_id);

    std::string strpodrName, strpodrCode;
    TDB_StrPodrsd::GetStrPodrsdForUser(GetUserId(), strpodrName, strpodrCode);

    std::string pokaz_bk_code = TDB_Pokaztl::GetCodeBK(aCurPokazId);
    std::string rzdl;
    if (pokaz_bk_code.length() >= 4) {
        rzdl = pokaz_bk_code.substr(0, 4);
    }

    std::string w = std::to_string(aCurYear);
    w = w.substr(2, 2);

    std::string fullNmbrDgvr = std::to_string(UnikNmbrDgvr) + "/" + w + "-" + strpodrCode;
    if (!rzdl.empty()) {
        fullNmbrDgvr += "-" + rzdl;
    }

    return fullNmbrDgvr;
}

bool TDB_Dgvr::setNmbrDgvr(int aCurPokazId, int aobjekt_class_id, int aCurYear, int aPlan_id, unsigned char aCurrVidOtch, std::string &nmbrDgvr) {
    std::string fullNmbrDgvr;

    if (aCurYear < 2010) return false;
    if (aobjekt_class_id != TDB_Dgvr::GetIdClassDgvr()) return false;
    if (TDB_NMrpr::GetCurDBName().find("uvao") == std::string::npos) return false;

    fullNmbrDgvr = TDB_Dgvr::getNmbrDgvr(aCurPokazId, aobjekt_class_id, aCurYear, aPlan_id);
    TDB_Dgvr::saveNmbrDgvr(fullNmbrDgvr, aCurPokazId, aobjekt_class_id, aCurYear, aPlan_id, aCurrVidOtch);

    nmbrDgvr = fullNmbrDgvr;
    return true;
}

void TDB_Dgvr::saveNmbrDgvr(const std::string& fullNmbrDgvr, int aCurPokazId, int aobjekt_class_id, int aCurYear, int aPlan_id, unsigned char aCurrVidOtch) {
    int N_dgvr_id, N_dgvr_cellid;
    TCellValueAtr vd;
    TDateTime ValueDate;

    TDB_Dgvr::GetField_id_forNDgvr(N_dgvr_id, N_dgvr_cellid);

    vd.PokazId = N_dgvr_id;
    vd.CellId = N_dgvr_cellid;
    vd.ObjectId = aCurPokazId;
    ValueDate = EncodeDate(aCurYear, 1, 2); // Start of the year - January 2nd - this is conditional and done specifically
                                            // to update existing data without adding new data
    vd.ValueDate = ValueDate;
    vd.DataType = 2;
    vd.pokaz_type = 2;
    vd.VidOtch = aCurrVidOtch;
    vd.PlanId = aPlan_id;
    vd.EvalType = 0;
    vd.class_id_ForKey = 0;

    TDB_CellValue::SetStrValue(&vd, fullNmbrDgvr, aCurPokazId, N_dgvr_cellid, aPlan_id, aCurrVidOtch, ValueDate);
}

void TDB_Dgvr::GetAllPartForDgvr(TQueryUni* qIn, int ObjectId) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Pokaztl WHERE enum_id = :ObjectId ORDER BY is_list");
    qIn->ParamByName("ObjectId")->AsInteger() = ObjectId;
    qIn->Open();
}

int TDB_Dgvr::GetKolPartForDgvr(int ObjectId) {
    int n = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Pokaztl WHERE enum_id = :ObjectId ORDER BY is_list");
    q->ParamByName("ObjectId")->AsInteger() = ObjectId;
    q->Open();
    while (!q->EoF()) {
        n++;
        q->Next();
    }
   //MAK q->Free();
delete q;
    return n;
}

void TDB_Dgvr::UpdNameDgvrKlons(int ObjectId, const std::string nameDgvr) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Dgvr::GetAllPartForDgvr(q, ObjectId);
    while (!q->EoF()) {
        if (q->FieldByName("pokaz_id")->AsInteger() != ObjectId) {
            TDB_Pokaztl::UpdPokazName(q->FieldByName("pokaz_id")->AsInteger(), nameDgvr + " {" + GetUserLogin() + "}");
        }
        q->Next();
    }
   //MAK q->Free();
delete q;
}

void TDB_Dgvr::getInfoByPegim(bool isGbu, int& class_id, int& field_Finansir_id, int& link_pokaz_id) {
//vernut    if (isGbu) {
//        if (!TIDsClassFieldCell::getClassId("Денежное обязательство", 1, true, class_id)) return;
//        if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования", Date, true, field_Finansir_id)) return;
//        TIDsClassFieldCell::getClassFieldId(class_id, "Мероприятие", Date, false, link_pokaz_id);
//    } else {
//        if (!TIDsClassFieldCell::getClassId("Договор", 1, true, class_id)) return;
//        if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования", Date, true, field_Finansir_id)) return;
//        TIDsClassFieldCell::getClassFieldId(class_id, "Мероприятие", Date, true, link_pokaz_id);
//    }
}

double TDB_Dgvr::GetSumNumBOByKBK(int pokaz_id, int plan_id, TDateTime value_date,
                                   int vid_otc, short cell_period, int ist_fin_id) {
    int class_BO_id;
    int field_FinansirBO_id;
    int Field_FinansirBO_cellid = 0;
    int L;
    std::string pokaz_bk_code;

//vernut    if (TDB_SystSettings::GetAppSpace() == "Контроль ГБУ") {
//        return TDB_Dgvr::GetSumNumDOByKBK(pokaz_id, plan_id, value_date, vid_otc, cell_period, ist_fin_id);
//    }
//
//    if (!TIDsClassFieldCell::getClassId("Договор", 1, true, class_BO_id)) return 0;
//    if (!TIDsClassFieldCell::getClassFieldId(class_BO_id, "Объем финансирования", Date(), true, field_FinansirBO_id)) return 0;
//    Field_FinansirBO_cellid = TIDsClassFieldCell::getPokazCellId(field_FinansirBO_id, cell_period, ist_fin_id);
//
//
//
//    pokaz_bk_code = Trim(TDB_Pokaztl::GetCodeBK(pokaz_id)).c_str();
//    L = pokaz_bk_code.length();
    double Result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Pokaztl b, Pokaztl d, Cell_value, KeyOfObjects");
    q->SQL->Add("WHERE b.pokaz_id = :pokaz_id");
    q->SQL->Add("AND d.pokaz_bk_code = b.pokaz_bk_code");
    q->SQL->Add("AND KeyOfObjects.objectid = d.pokaz_id");
    q->SQL->Add("AND Cell_value.objectid = KeyOfObjects.objectid");
    q->SQL->Add("AND Cell_value.cell_id = :cell_id");
    q->SQL->Add("AND Cell_value.plan_id = :plan_id");
    q->SQL->Add("AND Cell_value.vid_otc = :vid_otc");
    q->SQL->Add("AND Cell_value.value_date = :value_date");

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = Field_FinansirBO_cellid;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;

    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("value")->AsFloat();
    }

   //MAK q->Free();
delete q;
    return Result;
}

double TDB_Dgvr::GetSumNumDOByKBK(int pokaz_id, int plan_id, TDateTime value_date,
                                   int vid_otc, short cell_period, int ist_fin_id) {
    int class_DO_id;
    int field_FinansirDO_id;
    int Field_FinansirDO_cellid;
    int L;
    std::string pokaz_bk_code;

//vernut    if (!TIDsClassFieldCell::getClassId("Денежное обязательство", 1, true, class_DO_id)) return 0;
//    if (!TIDsClassFieldCell::getClassFieldId(class_DO_id, "Объем финансирования",
//         Date(), true, field_FinansirDO_id)) return 0;
//    Field_FinansirDO_cellid = TIDsClassFieldCell::getPokazCellId(field_FinansirDO_id, cell_period, ist_fin_id);

    double Result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    pokaz_bk_code = Trim(TDB_Pokaztl::GetCodeBK(pokaz_id)).c_str();
    L = pokaz_bk_code.length();
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    //--q->SQL->Add("FROM Pokaztl b, Pokaztl d, Cell_value, KeyOfObjects");
    q->SQL->Add("FROM Pokaztl b, Pokaztl d, Cell_value");
    q->SQL->Add("WHERE b.pokaz_id = :pokaz_id");
    q->SQL->Add("AND SUBSTRING(d.pokaz_bk_code, 1, :L) = SUBSTRING(b.pokaz_bk_code, 1, :L)");
    //--q->SQL->Add("AND KeyOfObjects.objectid = d.pokaz_id");

    //--q->SQL->Add("AND Cell_value.objectid = KeyOfObjects.objectid");
    q->SQL->Add("AND Cell_value.objectid = d.pokaz_id");
    q->SQL->Add("AND Cell_value.cell_id = :cell_id");
    q->SQL->Add("AND Cell_value.plan_id = :plan_id");
    q->SQL->Add("AND Cell_value.vid_otc = :vid_otc");
    q->SQL->Add("AND Cell_value.value_date = :value_date");

    q->ParamByName("L")->AsInteger() = L;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("cell_id")->AsInteger() = Field_FinansirDO_cellid;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;

    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("value")->AsFloat();
    }

   //MAK q->Free();
delete q;
    return Result;
}

double TDB_Dgvr::GetSumObjFinByKBK(int pokaz_id, int plan_id, TDateTime value_date,
                                   int vid_otc, short cell_period, int ist_fin_id) {
    int class_BO_id;
    int field_FinansirBO_id;
    int Field_FinansirBO_cellid = 0;
    double Result = 0;

//vernut    if (!TIDsClassFieldCell::getClassId("Договор", 1, true, class_BO_id)) return Result;
//    if (!TIDsClassFieldCell::getClassFieldId(class_BO_id, "Объем финансирования",
//                                             Date(), true, field_FinansirBO_id)) return Result;
//    Field_FinansirBO_cellid = TIDsClassFieldCell::getPokazCellId(field_FinansirBO_id, cell_period, ist_fin_id);

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Pokaztl b, Pokaztl d, Cell_value");
    q->SQL->Add("WHERE b.pokaz_id = :pokaz_id");
    q->SQL->Add("AND d.pokaz_bk_code = b.pokaz_bk_code");
    q->SQL->Add("AND Cell_value.objectid = d.pokaz_id");
    q->SQL->Add("AND Cell_value.cell_id = :cell_id");
    q->SQL->Add("AND Cell_value.plan_id = :plan_id");
    q->SQL->Add("AND Cell_value.vid_otc = :vid_otc");
    q->SQL->Add("AND Cell_value.value_date = :value_date");

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("cell_id")->AsInteger() = Field_FinansirBO_cellid;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;

    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("value")->AsFloat();
    }

   //MAK q->Free();
delete q;
    return Result;
}
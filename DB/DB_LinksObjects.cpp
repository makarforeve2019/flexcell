#include "DB_LinksObjects.h"
#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_Pokaztl.h"
#include "DB_Cell.h"
#include "DB_CodeCelSubs.h"
#include "DB_CellValue.h"
#include "DB_CodeCelSubs.h"
#include "../Pokazateli/unIDsClassFieldCell.h"
#include "../calc/UpdDBbyValue.h"

int TDB_LinksObjects::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int link_id = 0;

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as link_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('LinksObjects', 'link_id')) as link_id");
            break;
    }
    q->Open();
    link_id = q->FieldByName("link_id")->AsInteger();
    
   //MAK q->Free();
delete q;
    return link_id;
}

int TDB_LinksObjects::GetLinksObjects(int plan_id, int objectid, int pokaz_id, int link_pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int link_id = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT link_id");
    q->SQL->Add("FROM LinksObjects");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND objectid = :objectid");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->SQL->Add("AND link_pokaz_id = :link_pokaz_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->Open();
    link_id = q->FieldByName("link_id")->AsInteger();

   //MAK q->Free();
delete q;
    return link_id;
}

int TDB_LinksObjects::GetIdObjectPokaz(int plan_id, int objectid, int link_pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int link_id = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT link_id");
    q->SQL->Add("FROM LinksObjects");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND objectid = :objectid");
    q->SQL->Add("AND link_pokaz_id = :link_pokaz_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->Open();
    link_id = q->FieldByName("link_id")->AsInteger();

   //MAK q->Free();
delete q;
    return link_id;
}

int TDB_LinksObjects::GetdPokazByObject(int plan_id, int objectid, int link_pokaz_id, std::string &aPokazName) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int pokaz_id = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM LinksObjects");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND objectid = :objectid");
    q->SQL->Add("AND link_pokaz_id = :link_pokaz_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->Open();
    if (!q->EoF()) {
        pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
        aPokazName = TDB_Pokaztl::GetPokazName(pokaz_id);
    }

   //MAK q->Free();
delete q;
    return pokaz_id;
}

void TDB_LinksObjects::GetObjectsByPokaz(int plan_id, int pokaz_id, int link_pokaz_id, TStringList *aSl) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT objectid, pokaz_name");
    q->SQL->Add("FROM LinksObjects, Pokaztl");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND LinksObjects.pokaz_id = :pokaz_id");
    q->SQL->Add("AND link_pokaz_id = :link_pokaz_id");
    q->SQL->Add("AND Pokaztl.pokaz_id = objectid");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->Open();
    while (!q->EoF()) {
        aSl->AddObject(q->FieldByName("pokaz_name")->AsString(), q->FieldByName("objectid")->AsInteger());
        q->Next();
    }

   //MAK q->Free();
delete q;
}

bool TDB_LinksObjects::isObjectsByPokaz(int plan_id, int pokaz_id, int link_pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("SELECT objectid, pokaz_name");
    q->SQL->Add("FROM LinksObjects, Pokaztl");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND LinksObjects.pokaz_id = :pokaz_id");
    q->SQL->Add("AND link_pokaz_id = :link_pokaz_id");
    q->SQL->Add("AND Pokaztl.pokaz_id = objectid");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->Open();
    
    bool result = !q->EoF();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_LinksObjects::AddLinksObjects(int plan_id, int objectid, int pokaz_id, int link_pokaz_id, TDateTime date_create) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    int link_id = TDB_LinksObjects::GetIdObjectPokaz(plan_id, objectid, link_pokaz_id);
    if (link_id > 0) {
        TDB_LinksObjects::DelLinksObjects(link_id);
    }
    
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO LinksObjects");
    q->SQL->Add("(plan_id, objectid, pokaz_id, link_pokaz_id, date_create)");
    q->SQL->Add("VALUES (:plan_id, :objectid, :pokaz_id, :link_pokaz_id, :date_create)");
    q->ParamByName("plan_id")->AsInteger()= plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->ParamByName("date_create")->AsDateTime() = date_create;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
    return TDB_LinksObjects::GetLinksObjects(plan_id, objectid, pokaz_id, link_pokaz_id);
}

int TDB_LinksObjects::AddMultiLinksObjects(int plan_id, int objectid, int pokaz_id, int link_pokaz_id, TDateTime date_create) {
    if (TDB_LinksObjects::GetLinksObjects(plan_id, objectid, pokaz_id, link_pokaz_id) > 0) {
        // --SystFuncs.MessageBoxExclamation('Такая связь с мероприятием уже установлена', MsgVnim);
        return 0;
    }
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO LinksObjects");
    q->SQL->Add("(plan_id, objectid, pokaz_id, link_pokaz_id, date_create)");
    q->SQL->Add("VALUES (:plan_id, :objectid, :pokaz_id, :link_pokaz_id, :date_create)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->ParamByName("date_create")->AsDateTime() = date_create;
    q->ExecSQL();
    
    int result = TDB_LinksObjects::GetId();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_LinksObjects::UpdPokazId(int plan_id, int pokaz_id, int NewPokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE LinksObjects");
    q->SQL->Add("SET pokaz_id = :NewPokaz_id");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("NewPokaz_id")->AsInteger() = NewPokaz_id;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_LinksObjects::DelLinksObjects(int link_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM LinksObjects");
    q->SQL->Add("WHERE link_id = :link_id");
    q->ParamByName("link_id")->AsInteger() = link_id;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_LinksObjects::DelMultiLinksObjects(int plan_id, int objectid, int pokaz_id, int link_pokaz_id) {
    int link_id = TDB_LinksObjects::GetLinksObjects(plan_id, objectid, pokaz_id, link_pokaz_id);
    if (link_id == 0) return;
    TDB_LinksObjects::DelLinksObjects(link_id);
}

void TDB_LinksObjects::doAgregirUpByField(TwwStringList p, PCellValueAtr vd) {
    PAtrRow AtrRow = ((*p.ListRows)[vd->Row]);


    if (AtrRow == nullptr) return;
    int class_id = AtrRow->objekt_class_id;
    int summaDgvr_id, summaDgvr_cellid, link_pokaz_id, meropr_id, cell_idMrpr;
    double summa;
    std::string ws;

    if (!TIDsClassFieldCell::getClassFieldIdByCode(class_id, "summaDgvr", Date(), true, summaDgvr_id)) return;
    summaDgvr_cellid = TIDsClassFieldCell::getPokazCellId(summaDgvr_id, vd->cell_period, vd->ist_fin_id);
    if (!TIDsClassFieldCell::getClassFieldIdByCode(class_id, "Meroprijtie", Date(), true, link_pokaz_id)) return;
    meropr_id = TDB_LinksObjects::GetdPokazByObject(vd->PlanId, vd->ObjectId, link_pokaz_id, ws);
    if (meropr_id == 0) return;

    if (TDB_Pokaztl::getTypeEntity(vd->ObjectId) > 0) return;

    cell_idMrpr = TDB_Cell::GetCellId(meropr_id, vd->cell_period, vd->ist_fin_id);

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(Cell_value.value) as sumValue FROM LinksObjects, Cell_value ");
    q->SQL->Add("WHERE LinksObjects.plan_id = :plan_id and LinksObjects.pokaz_id = :meropr_id and LinksObjects.link_pokaz_id = :link_pokaz_id ");
    q->SQL->Add("and Cell_value.objectid = LinksObjects.objectid ");
    q->SQL->Add("and Cell_value.cell_id = :cell_id and Cell_value.plan_id = :plan_id1 and vid_otc = :vid_otc and Cell_value.value_date = :value_date ");

    q->ParamByName("plan_id")->AsInteger() = vd->PlanId;
    q->ParamByName("meropr_id")->AsInteger() = meropr_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = vd->CellId;
    q->ParamByName("plan_id1")->AsInteger() = vd->PlanId;
    q->ParamByName("vid_otc")->AsInteger() = vd->VidOtch;
    q->ParamByName("value_date")->AsDateTime() = vd->ValueDate;

    q->Open();

    summa = q->FieldByName("sumValue")->AsFloat();

    TUpdDBbyValue::UpdDBbyValue1(cell_idMrpr, vd->PlanId, vd->VidOtch, vd->ValueDate, summa, 0, false);

   //MAK q->Free();
delete q;
}

double TDB_LinksObjects::doAgregirSumForSubObjects(int plan_id, int meropr_id, int link_pokaz_id,
  int cell_idClassField, int vid_otc, TDateTime dateKV) {
    double summa = 0.0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(CV.value) as sumValue FROM LinksObjects L, Cell_value CV");
    q->SQL->Add("WHERE L.plan_id = :plan_id and L.pokaz_id = :meropr_id and L.link_pokaz_id = :link_pokaz_id ");
    q->SQL->Add("and CV.objectid = L.objectid ");
    q->SQL->Add("and CV.cell_id = :cell_id ");
    q->SQL->Add("and CV.plan_id = :plan_id1 ");
    q->SQL->Add("and CV.vid_otc = :vid_otc ");
    q->SQL->Add("and CV.value_date = :value_date ");

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("meropr_id")->AsInteger() = meropr_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;

    q->ParamByName("cell_id")->AsInteger() = cell_idClassField;
    q->ParamByName("plan_id1")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = dateKV;

    q->Open();

    summa = q->FieldByName("sumValue")->AsFloat();

    return summa;
}

double TDB_LinksObjects::doAgregirSumForSubObjects2(int plan_id, int meropr_id, int link_pokaz_id,
      int cell_idClassField, int vid_otc, TDateTime dateKV) {
    double summa = 0.0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(CV.value) as sumValue FROM LinksObjects L, Cell_value CV, Pokaztl P");
    q->SQL->Add("WHERE L.plan_id = :plan_id and L.pokaz_id = :meropr_id and L.link_pokaz_id = :link_pokaz_id ");
    q->SQL->Add("and CV.objectid = L.objectid ");
    q->SQL->Add("and CV.cell_id = :cell_id ");
    q->SQL->Add("and CV.plan_id = :plan_id1 ");
    q->SQL->Add("and CV.vid_otc = :vid_otc ");
    q->SQL->Add("and CV.value_date = :value_date ");
    q->SQL->Add("and (P.pokaz_id = L.objectid) and ((P.typeEntity = 0) or (P.typeEntity IS NULL)) ");

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("meropr_id")->AsInteger() = meropr_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;

    q->ParamByName("cell_id")->AsInteger() = cell_idClassField;
    q->ParamByName("plan_id1")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = dateKV;

    q->Open();

    summa = q->FieldByName("sumValue")->AsFloat();

   //MAK q->Free();
delete q;

    return summa;
}

double TDB_LinksObjects::doAgregirSumForSubObjects3(int plan_id, int meropr_id, int link_pokaz_id,
      int cell_idLimitZakup, int vid_otc, int god) {
    double summa = 0.0, objDgvr = 0.0, limZak = 0.0;
    int class_idZakupki = 0, objectid, S00000Id, S00000CellIdKv, S00000CellIdGod, ist_fin_id00000;
    int SummaDgvr_Id, SummaDgvr00000CellIdKV, SummaDgvr00000CellIdGod;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    TIDsClassFieldCell::getClassIdByCode("ZakupkaDgvr", 1, true, class_idZakupki);

    TDB_CodeCelSubs::findSubsidPokazCellId("0000000000", S00000Id, S00000CellIdKv, S00000CellIdGod);
    ist_fin_id00000 = TDB_CodeCelSubs::GetIstFinIdBySubs(S00000Id);

    TIDsClassFieldCell::getClassFieldIdByCode(class_idZakupki, "summaDgvr", Date(), true, SummaDgvr_Id);
    TDB_CodeCelSubs::findSummaDgvrSubsidCellId(class_idZakupki, SummaDgvr_Id, ist_fin_id00000, "0000000000", S00000Id,
      SummaDgvr00000CellIdKV, SummaDgvr00000CellIdGod);

    q->SQL->Clear();
    q->SQL->Add("SELECT P.pokaz_id FROM LinksObjects L, Pokaztl P");
    q->SQL->Add("WHERE L.plan_id = :plan_id and L.pokaz_id = :meropr_id and L.link_pokaz_id = :link_pokaz_id ");
    q->SQL->Add("AND (P.pokaz_id = L.objectid) and ((P.typeEntity = 0) or (P.typeEntity IS NULL)) ");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("meropr_id")->AsInteger() = meropr_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->Open();
    summa = 0;
    while (!q->EoF()) {
        objectid = q->FieldByName("pokaz_id")->AsInteger();
        objDgvr = TDB_CellValue::GetValue(objectid, SummaDgvr00000CellIdKV, plan_id, 7, EncodeDate(god, 3, 31));
        if (objDgvr > 0) {
            summa += objDgvr;
        } else {
            limZak = TDB_CellValue::GetValue(objectid, cell_idLimitZakup, plan_id, vid_otc, EncodeDate(god, 1, 2));
            summa += limZak;
        }
        q->Next();
    }

    return summa;
}




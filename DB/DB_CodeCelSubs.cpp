#include "DB_CodeCelSubs.h"
#include "unDefDBconnect.h"
#include "DB_CellValue.h"
#include "UpdDBbyValue.h"
#include "DB_PlanStr1n.h"
#include "DB_IstFin.h"
#include "DB_Cell.h"
#include "unIDsClassFieldCell.h"

std::string TDB_CodeCelSubs::getCodeCelSubsByIstFinId(int ist_fin_id) {
    return TIDsClassFieldCell::getCodeCelSubsByIstFinId(ist_fin_id);
}

bool TDB_CodeCelSubs::findSubsidPokazCellId(std::string codeCelSubs, int& SubsidPokazId, int& SubsidCellIdKV, int& SubsidCellIdGod) {
    return TIDsClassFieldCell::findSubsidPokazCellId(codeCelSubs, SubsidPokazId, SubsidCellIdKV, SubsidCellIdGod);
}

bool TDB_CodeCelSubs::findSummaDgvrSubsidCellId(int class_idZakupki, int SummaDgvrPokazId, int ist_fin_id, std::string codeCelSubs,
                                                int SubsidPokazId, int& SummaDgvrCellIdKV, int& SummaDgvrCellIdGod) {
    return TIDsClassFieldCell::findSummaDgvrSubsidCellId(class_idZakupki, SummaDgvrPokazId, ist_fin_id, 
        codeCelSubs, SubsidPokazId, SummaDgvrCellIdKV, SummaDgvrCellIdGod);
}

int TDB_CodeCelSubs::GetIstFinIdBySubs(int pokaz_idSubsid) {
    return TIDsClassFieldCell::GetIstFinIdBySubs(pokaz_idSubsid);
}

void TDB_CodeCelSubs::UpdDgvrSumZakontrAll(int plan_id, int god, int class_idZakupki, int objectid, double aDelta) {
    int SummaDgvr_Id;
    int SummaDgvr00000CellIdKV, SummaDgvr00000CellIdGod;
    int S00000Id, S00000CellIdKv, S00000CellIdGod;
    int ist_fin_id00000;
    double wSum;
    TDateTime dateKV1;

    if (!TIDsClassFieldCell::getClassFieldIdByCode(class_idZakupki, "summaDgvr", Date(), true, SummaDgvr_Id))
        return;

    if (!TDB_CodeCelSubs::findSubsidPokazCellId("0000000000", S00000Id, S00000CellIdKv, S00000CellIdGod)) {
        ////throw Exception("Показатель-субсидия по коду целевых средств \"0000000000\" в БД не найден. Обработка прекращается");
        return;
    }

    ist_fin_id00000 = TDB_CodeCelSubs::GetIstFinIdBySubs(S00000Id);

    if (!TDB_CodeCelSubs::findSummaDgvrSubsidCellId(class_idZakupki, SummaDgvr_Id, ist_fin_id00000, "0000000000", S00000Id,
        SummaDgvr00000CellIdKV, SummaDgvr00000CellIdGod)) {
        //throw Exception("Для характеристики \"Сумма договора\" не создан cell_id для кода целевых средств \"0000000000\"");
        return;
    }

    dateKV1 = EncodeDate(god, 3, 31);
    wSum = TDB_CellValue::GetValue(objectid, SummaDgvr00000CellIdKV, plan_id, 7, dateKV1);
    wSum += aDelta;
    TUpdDBbyValue::UpdDBbyValue1(SummaDgvr00000CellIdKV, plan_id, 7, dateKV1, wSum, objectid, false);
}

void TDB_CodeCelSubs::UpdDgvrVsSubsZakontrAll(int plan_id, int god, int class_idZakupki, int objectid, double aDelta) {
    int SummaDgvr_Id;
    int SummaDgvr00000CellIdKV, SummaDgvr00000CellIdGod;
    int S00000Id, S00000CellIdKv, S00000CellIdGod;
    int ist_fin_id00000;
    double wSum;
    TDateTime dateKV1;

    if (!TIDsClassFieldCell::getClassFieldIdByCode(class_idZakupki, "summaDgvr", Date(), true, SummaDgvr_Id))
        return;

    if (!TDB_CodeCelSubs::findSubsidPokazCellId("0000000000", S00000Id, S00000CellIdKv, S00000CellIdGod)) {
        //throw Exception("Показатель-субсидия по коду целевых средств \"0000000000\" в БД не найден. Обработка прекращается");
        return;
    }

    ist_fin_id00000 = TDB_CodeCelSubs::GetIstFinIdBySubs(S00000Id);

    if (!TDB_CodeCelSubs::findSummaDgvrSubsidCellId(class_idZakupki, SummaDgvr_Id, ist_fin_id00000, "0000000000", S00000Id,
        SummaDgvr00000CellIdKV, SummaDgvr00000CellIdGod)) {
        //throw Exception("Для характеристики \"Сумма договора\" не создан cell_id для кода целевых средств \"0000000000\"");
        return;
    }

    dateKV1 = EncodeDate(god, 3, 31);
    wSum = TDB_CellValue::GetValue(objectid, SummaDgvr00000CellIdKV, plan_id, 13, dateKV1);
    wSum += aDelta;
    TUpdDBbyValue::UpdDBbyValue1(SummaDgvr00000CellIdKV, plan_id, 13, dateKV1, wSum, objectid, false);
}

void TDB_CodeCelSubs::UpdDgvrVsOstZakontrAll(int plan_id, int god, int class_idZakupki, int objectid, double aDelta) {
    int SummaDgvr_Id;
    int SummaDgvr00000CellIdKV, SummaDgvr00000CellIdGod;
    int S00000Id, S00000CellIdKv, S00000CellIdGod;
    int ist_fin_id00000;
    double wSum;
    TDateTime dateKV1;

    if (!TIDsClassFieldCell::getClassFieldIdByCode(class_idZakupki, "summaDgvr", Date(), true, SummaDgvr_Id))
        return;

    if (!TDB_CodeCelSubs::findSubsidPokazCellId("0000000000", S00000Id, S00000CellIdKv, S00000CellIdGod)) {
        //throw Exception("Показатель-субсидия по коду целевых средств \"0000000000\" в БД не найден. Обработка прекращается");
        return;
    }

    ist_fin_id00000 = TDB_CodeCelSubs::GetIstFinIdBySubs(S00000Id);

    if (!TDB_CodeCelSubs::findSummaDgvrSubsidCellId(class_idZakupki, SummaDgvr_Id, ist_fin_id00000, "0000000000", S00000Id,
        SummaDgvr00000CellIdKV, SummaDgvr00000CellIdGod)) {
        //throw Exception("Для характеристики \"Сумма договора\" не создан cell_id для кода целевых средств \"0000000000\"");
        return;
    }

    dateKV1 = EncodeDate(god, 3, 31);

    wSum = TDB_CellValue::GetValue(objectid, SummaDgvr00000CellIdKV, plan_id, 14, dateKV1);
    wSum += aDelta;
    TUpdDBbyValue::UpdDBbyValue1(SummaDgvr00000CellIdKV, plan_id, 14, dateKV1, wSum, objectid, false);
}

double TDB_CodeCelSubs::GetSumKVforDogovor(int plan_id, int objectid, TDateTime Date1, TDateTime Date2) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double summaD = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND objectid = :objectid");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");

    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;

    q->Open();
    if (!q->IsEmpty())
        summaD = q->FieldByName("value")->AsFloat();

    delete q;

    return summaD;
}

double TDB_CodeCelSubs::GetSumKVforDogovorSubsid(int plan_id, int objectid, TDateTime Date1, TDateTime Date2, string codeCelSubs) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    double summaD = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND objectid = :objectid");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND codecelsubs = :codeCelSubs");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");

    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("codeCelSubs")->AsString() = codeCelSubs;
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;

    q->Open();
    if (!q->EoF()) {
        summaD = q->FieldByName("value")->AsFloat();
    }
    q->Close();

    delete q;

    return summaD;
}

double TDB_CodeCelSubs::GetSumKVforDogovorVsegoSubsid(int godOperDay, int plan_id, int objectid, TDateTime Date1, TDateTime Date2) {
    std::string sGod = std::to_string(godOperDay);
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double summaD = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND objectid = :objectid");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND codecelsubs LIKE :codeCelSubs");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");

    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("codeCelSubs")->AsString() = "___" + sGod + "___";
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;

    q->Open();
    if (!q->EoF()) {
        summaD = q->FieldByName("value")->AsFloat();
    }
    q->Close();

    delete q;

    return summaD;
}

double TDB_CodeCelSubs::GetSumKVforDogovorVsegoOstatk(int godOperDay, int plan_id, int objectid, TDateTime Date1, TDateTime Date2) {
    string sGod = IntToStr(godOperDay);
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
 

    double summaD = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND objectid = :objectid");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND codecelsubs NOT LIKE :codeCelSubs");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");

    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("codeCelSubs")->AsString() = "___" + sGod + "___";
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;

    q->Open();
    if (!q->EoF()) {
        summaD = q->FieldByName("value")->AsFloat();
    }
    q->Close();

    delete q;

    return summaD;
}

double TDB_CodeCelSubs::GetSumKVforMeropr(int plan_id, int meropr_id, TDateTime Date1, TDateTime Date2) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
 

    double summaD = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND objectid = 0");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");

    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = meropr_id;
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;

    q->Open();
    if (!q->EoF()) {
        summaD = q->FieldByName("value")->AsFloat();
    }
    q->Close();

    delete q;

    return summaD;
}

double TDB_CodeCelSubs::GetSumKVforMeroprSubsid(int plan_id, int meropr_id, TDateTime Date1, TDateTime Date2, string codeCelSubs) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double summaD = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND objectid = 0");
    q->SQL->Add("AND pokaz_id = :meropr_id");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND codecelsubs = :codeCelSubs");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");

    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("meropr_id")->AsInteger() = meropr_id;
    q->ParamByName("codeCelSubs")->AsString() = codeCelSubs;
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;

    q->Open();
    if (!q->EoF()) {
        summaD = q->FieldByName("value")->AsFloat();
    }
    q->Close();

    delete q;

    return summaD;
}

double TDB_CodeCelSubs::GetSumKVforMeroprVsegoSubsid(int godOperDay, int plan_id, int meropr_id, TDateTime Date1, TDateTime Date2) {
    string sGod = std::to_string(godOperDay);
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
 

    double summaD = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND objectid = 0");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND codecelsubs LIKE :codeCelSubs");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");

    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = meropr_id;
    q->ParamByName("codeCelSubs")->AsString() = "___" + sGod + "___";
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;

    q->Open();
    if (!q->EoF()) {
        summaD = q->FieldByName("value")->AsFloat();
    }
    q->Close();

    delete q;

    return summaD;
}

double TDB_CodeCelSubs::GetSumKVforMeroprVsegoOstatk(int godOperDay, int plan_id, int meropr_id, TDateTime Date1, TDateTime Date2) {
    string sGod = std::to_string(godOperDay);
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
 

    double summaD = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND objectid = 0");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND codecelsubs NOT LIKE :codeCelSubs");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");

    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = meropr_id;
    q->ParamByName("codeCelSubs")->AsString() = "___" + sGod + "___";
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;

    q->Open();
    if (!q->EoF()) {
        summaD = q->FieldByName("value")->AsFloat();
    }
    q->Close();

    delete q;

    return summaD;
}

void TDB_CodeCelSubs::setNewValuePokazSubsid(int SubsidCellIdKv, int plan_id, TDateTime Date2, double summaPP, int vid_otc) {
    double summaD = TDB_CellValue::GetValue(0, SubsidCellIdKv, plan_id, vid_otc, Date2);
    summaD += summaPP;
    TUpdDBbyValue::UpdDBbyValue1(SubsidCellIdKv, plan_id, vid_otc, Date2, summaD, 0, false);
}

int TDB_CodeCelSubs::evalGodFinansir(string codecelsubs) {
    if (codecelsubs.length() < 8) {  // Проверка на минимально допустимую длину строки
        // std::invalid_argument("Code string too short to extract year.");
    }

    std::string sgod = codecelsubs.substr(3, 4); // Извлечение подстроки начиная с 4-й позиции, длиной 4 символа

    try {
        int wgod = std::stoi(sgod); // Преобразование извлеченной подстроки в число
        return wgod;
    } catch (const std::invalid_argument& e) {
        //throw std::runtime_error("Failed to convert extracted substring to integer: " + sgod);
    } catch (const std::out_of_range& e) {
        //throw std::runtime_error("Extracted integer value out of range: " + sgod);
    }
}


double TDB_CodeCelSubs::GetSumKVforMmeroprOldYear(int plan_id, int god, int class_idZakupki, int meropr_idOldYear, int aCell_id, int aVid_otc, TDateTime aValue_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
 
    TDB_PlanStr1n::GetAllSubItemYear(q, plan_id, meropr_idOldYear, class_idZakupki, god);

    double sum = 0;
    int objectid;
    while (!q->EoF()) {
        objectid = q->FieldByName("pokaz_id")->AsInteger();
        double d = TDB_CellValue::GetValue(objectid, aCell_id, plan_id, aVid_otc, aValue_date);
        sum += d;
        q->Next();
    }

    delete q;
    return sum;
}

void TDB_CodeCelSubs::evalSubsidOnUpdMmeropr(std::string& EditString, double oldValueMrpr, PCellValueAtr vd, int& plan_idHandle,
                                             int& cell_idHandle, int& objectidHandle, int& vid_otcHandle, TDateTime& value_dateHandle) {
    int pokaz_idSubsid = TDB_IstFin::getPokazIdIFbyIstFinId(vd->ist_fin_id);
    if (pokaz_idSubsid == 0) return;

    if (vd->VidOtch != 6) return;

    double newValueMrpr = stod(EditString);
    double deltaMrpr = newValueMrpr - oldValueMrpr;

    int cell_idSubsid = TDB_Cell::GetCellId(pokaz_idSubsid, 2, 0); // Assuming the parameters are correct
    double oldCellValueSubsid = TDB_CellValue::GetValue(0, cell_idSubsid, vd->PlanId, 6, vd->ValueDate);
    double newCellValueSubsid = oldCellValueSubsid + deltaMrpr;
    TUpdDBbyValue::UpdDBbyValue1(cell_idSubsid, vd->PlanId, 6, vd->ValueDate, newCellValueSubsid, 0, false);

    plan_idHandle = vd->PlanId;
    cell_idHandle = cell_idSubsid;
    vid_otcHandle = 6;
    value_dateHandle = vd->ValueDate;
    objectidHandle = 0;
}

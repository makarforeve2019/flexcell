#include "DB_NeOdnoznPlategka.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "unDefDBconnect.h"

int TDB_NeOdnoznPlategka::IsPlategka(std::string KBKfromXML, std::string nom_plategki, TDateTime value_date,
  double ValueSumma, std::string rasc_scet) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT neodnozn_id FROM NeOdnoznPlategka");
    q->SQL->Add("WHERE KBKfromXML = :KBKfromXML");
    q->SQL->Add("AND nom_plategki = :nom_plategki");
    q->SQL->Add("AND value_date = :value_date");
    q->SQL->Add("AND ValueSumma = :ValueSumma");
    q->SQL->Add("AND rasc_scet = :rasc_scet");
    q->ParamByName("KBKfromXML")->AsString() = KBKfromXML;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ParamByName("ValueSumma")->AsFloat() = ValueSumma;
    q->ParamByName("rasc_scet")->AsString() = rasc_scet;

    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("neodnozn_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return Result;
}

void TDB_NeOdnoznPlategka::DelPlategka(int neOdnozn_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM NeOdnoznPlategka");
    q->SQL->Add("WHERE neodnozn_id = :neOdnozn_id");
    q->ParamByName("neOdnozn_id")->AsInteger() = neOdnozn_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_NeOdnoznPlategka::UpdPlategka(int neOdnozn_id, int budget_poluc_id, int kod_oshibki) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE NeOdnoznPlategka");
    q->SQL->Add("SET budget_poluc_id = :budget_poluc_id, kod_oshibki = :kod_oshibki");
    q->SQL->Add("WHERE neodnozn_id = :neOdnozn_id");
    q->ParamByName("neOdnozn_id")->AsInteger() = neOdnozn_id;
    q->ParamByName("budget_poluc_id")->AsInteger() = budget_poluc_id;
    q->ParamByName("kod_oshibki")->AsInteger() = kod_oshibki;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_NeOdnoznPlategka::SaveInNeOdnozn(std::string NLimSprav, std::string kodAU, double ValueSumma,
    std::string fromXML, TDateTime value_date, std::string file_name, TDateTime file_date, std::string reg_nomer,
    std::string short_name, int budget_poluc_id, int kod_oshibki, std::string rasc_scet, std::string nomer_BO,
    std::string nom_plategki, TDateTime date_plategki, std::string pokaz_bk_code, std::string NBOneprisoed,
    std::string BnkRS_PlatPoluc, std::string nom_documenta) {
    if (TDB_NeOdnoznPlategka::IsPlategka(pokaz_bk_code, nom_plategki, value_date, ValueSumma, rasc_scet) > 0) {
        return 0;
    }
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO NeOdnoznPlategka");
    q->SQL->Add("(NLimSprav, kodAU, ValueSumma, fromXML, value_date, "
        "file_name, file_date, reg_nomer, short_name, budget_poluc_id, kod_oshibki, rasc_scet, nomer_BO, "
        "nom_plategki, date_plategki, KBKfromXML, NBOneprisoed, BnkRS_PlatPoluc, nom_documenta)");
    q->SQL->Add("VALUES (:NLimSprav, :kodAU, :ValueSumma, :fromXML, :value_date, "
        ":file_name, :file_date, :reg_nomer, :short_name, :budget_poluc_id, :kod_oshibki, :rasc_scet, :nomer_BO, "
        ":nom_plategki, :date_plategki, :KBKfromXML, :NBOneprisoed, :BnkRS_PlatPoluc, :nom_documenta)");
    q->ParamByName("NLimSprav")->AsString() = NLimSprav;
    q->ParamByName("kodAU")->AsString() = kodAU;
    q->ParamByName("ValueSumma")->AsFloat() = ValueSumma;
    q->ParamByName("fromXML")->AsString() = fromXML;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ParamByName("file_name")->AsString() = file_name; // ExtractFileName(file_name);
    q->ParamByName("file_date")->AsDateTime() = file_date;
    q->ParamByName("reg_nomer")->AsString() = reg_nomer;
    q->ParamByName("short_name")->AsString() = short_name;
    q->ParamByName("budget_poluc_id")->AsInteger() = budget_poluc_id;
    q->ParamByName("kod_oshibki")->AsInteger() = kod_oshibki;
    q->ParamByName("rasc_scet")->AsString() = rasc_scet;
    q->ParamByName("nomer_BO")->AsString() = nomer_BO;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    q->ParamByName("KBKfromXML")->AsString() = pokaz_bk_code;
    q->ParamByName("NBOneprisoed")->AsString() = NBOneprisoed;
    q->ParamByName("BnkRS_PlatPoluc")->AsString() = BnkRS_PlatPoluc;
    q->ParamByName("nom_documenta")->AsString() = nom_documenta;

    q->ExecSQL();
    int result = TDB_NeOdnoznPlategka::GetId();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_NeOdnoznPlategka::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as neodnozn_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('NeOdnoznPlategka', 'neodnozn_id')) as neodnozn_id");
            break;
    }
    q->Open();
    int result = q->FieldByName("neodnozn_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

TDateTime TDB_NeOdnoznPlategka::getValue_date(int neOdnozn_id, double& ValueSumma) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT value_date, ValueSumma FROM NeOdnoznPlategka");
    q->SQL->Add("WHERE neodnozn_id = :neOdnozn_id");
    q->ParamByName("neOdnozn_id")->AsInteger() = neOdnozn_id;
    q->Open();
    TDateTime result;
    if (!q->EoF()) {
        result = q->FieldByName("value_date")->AsDateTime();
        ValueSumma = q->FieldByName("ValueSumma")->AsFloat();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_NeOdnoznPlategka::UpdNote(int neOdnozn_id, std::string Note) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE neodnoznplategka");
    q->SQL->Add("SET note = :Note");
    q->SQL->Add("WHERE neodnozn_id = :neOdnozn_id");
    q->ParamByName("neOdnozn_id")->AsInteger() = neOdnozn_id;
    q->ParamByName("note")->AsString() = Note;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_NeOdnoznPlategka::UpdErrorMsg(int neOdnozn_id, std::string aMsg) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE neodnoznplategka");
    q->SQL->Add("SET kodau = :kodau");
    q->SQL->Add("WHERE neodnozn_id = :neOdnozn_id");
    q->ParamByName("neOdnozn_id")->AsInteger() = neOdnozn_id;
    q->ParamByName("kodau")->AsString() = aMsg;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_NeOdnoznPlategka::UpdKOSGU(int neOdnozn_id, std::string CODE_KVR, std::string KOSGU) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE neodnoznplategka");
    q->SQL->Add("SET CODE_KVR = :CODE_KVR, KOSGU = :KOSGU");
    q->SQL->Add("WHERE neodnozn_id = :neOdnozn_id");
    q->ParamByName("neOdnozn_id")->AsInteger() = neOdnozn_id;
    q->ParamByName("CODE_KVR")->AsString() = CODE_KVR;
    q->ParamByName("KOSGU")->AsString() = KOSGU;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}


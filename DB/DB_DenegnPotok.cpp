#include "DB_DenegnPotok.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_Cell.h"
#include "DB_Dgvr.h"
#include "DB_Pokaztl.h"
#include "../calc/UpdDBbyValue.h"
#include "DB_LinksObjects.h"
#include "DB_PlanStr.h"
#include "DB_NeOdnoznPlategka.h"
#include "DB_BPolucRaj.h"
#include "DB_BPoluc.h"
#include "../Pokazateli/unIDsClassFieldCell.h"
#include "DB_IstFin.h"
//vernut  #include "unAPIDocum.h"
#include "unDefDBconnect.h"

int TDB_DenegnPotok::IsItembyCAvto(std::string wwAvtoRascet, int plan_id, int pokaz_id, int cell_id, int vid_otc,
                                   TDateTime value_date) {
    std::string attrib3;
    int den_potok_id = 0;
    int result = 0;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id, attrib3");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->SQL->Add("AND cell_id = :cell_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND value_date = :value_date");
    q->SQL->Add("AND objectid = 0");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();

    while (!q->EoF()) {
        switch (vid_otc) {
            case 5:
                attrib3 = q->FieldByName("attrib3")->AsString();
                if (attrib3.find(CAvtoRascet) != std::string::npos) {
                    result = q->FieldByName("den_potok_id")->AsInteger();
                    break;
                }
                break;
            case 4:
                den_potok_id = q->FieldByName("den_potok_id")->AsInteger();
                break;
        }
        q->Next();
        if (vid_otc == 4) {
            if (!q->EoF()) {
                DeleteItem(den_potok_id);
            } else {
                result = den_potok_id;
            }
        }
    }

    //MAK q->Free();
    delete q;
    return result;
}

int TDB_DenegnPotok::IsItembyCAvtoMrpr(int plan_id, int pokaz_id, int cell_id, int vid_otc, TDateTime value_date) {
    std::string attrib3;
    int result = 0;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id, attrib3");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->SQL->Add("AND cell_id = :cell_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND value_date = :value_date");
    q->SQL->Add("AND objectid = 0");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();

    while (!q->EoF()) {
        attrib3 = q->FieldByName("attrib3")->AsString();
        if (attrib3.find(CAvtoRascetMrpr) != std::string::npos) {
            result = q->FieldByName("den_potok_id")->AsInteger();
            break;
        }
        q->Next();
    }

    //MAK q->Free();
    delete q;
    return result;
}

int TDB_DenegnPotok::IsItembyCAvtoDgvr(int plan_id, int pokaz_id, int cell_id, int vid_otc, TDateTime value_date) {
    std::string attrib3;
    int result = 0;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id, attrib3");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->SQL->Add("AND cell_id = :cell_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND value_date = :value_date");
    q->SQL->Add("AND objectid = 0");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();

    while (!q->EoF()) {
        attrib3 = q->FieldByName("attrib3")->AsString();
        if (attrib3.find(CAvtoRascet) != std::string::npos) {
            result = q->FieldByName("den_potok_id")->AsInteger();
            break;
        }
        q->Next();
    }

    //MAK q->Free();
    delete q;
    return result;
}

int TDB_DenegnPotok::GetId() {
    int result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as den_potok_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Denegn_potok', 'den_potok_id')) as den_potok_id");
            break;
    }

    q->Open();
    result = q->FieldByName("den_potok_id")->AsInteger();
    //MAK q->Free();
    delete q;
    return result;
}

bool TDB_DenegnPotok::IsPlanGrDgvr(int vid_otc, int god, int objectid, double &summa) {
    bool result = false;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value FROM Denegn_potok");
    q->SQL->Add("WHERE vid_otc = :vid_otc");
    q->SQL->Add("AND objectid = :objectid");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND YEAR(value_date)  = :god");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', value_date)  = :god");
            break;
    }

    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();

    if (!q->EoF()) {
        summa = q->FieldByName("value")->AsFloat();
        if (summa > 0)
            result = true;
    }

    //MAK q->Free();
    delete q;
    return result;
}

bool TDB_DenegnPotok::IsPlategkaDgvrGod(int god, int objectid) {
    bool result = false;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT value FROM Denegn_potok");
    q->SQL->Add("WHERE vid_otc = 5");
    q->SQL->Add("AND objectid = :objectid");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND YEAR(value_date)  = :god");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', value_date)  = :god");
            break;
    }
    q->SQL->Add("AND nom_plategki <> :wpusto");

    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("wpusto")->AsString() = "";
    q->Open();

    if (!q->EoF())
        result = true;

    //MAK q->Free();
    delete q;
    return result;
}

bool TDB_DenegnPotok::IsPlategkaPokazGod(int god, int pokaz_id) {
    bool result = false;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT value FROM Denegn_potok");
    q->SQL->Add("WHERE vid_otc = 5");
    q->SQL->Add("AND objectid = 0");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND YEAR(value_date)  = :god");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', value_date)  = :god");
            break;
    }
    q->SQL->Add("AND nom_plategki <> :wpusto");

    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("wpusto")->AsString() = "";
    q->Open();

    if (!q->EoF())
        result = true;

    //MAK q->Free();
    delete q;
    return result;
}

int TDB_DenegnPotok::IsItembyRegNomer(std::string reg_nomer) {
    int result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id FROM Denegn_potok");
    q->SQL->Add("WHERE reg_nomer = :reg_nomer");
    q->ParamByName("reg_nomer")->AsString() = reg_nomer;
    q->Open();

    if (!q->EoF())
        result = q->FieldByName("den_potok_id")->AsInteger();

    //MAK q->Free();
    delete q;
    return result;
}

int TDB_DenegnPotok::IsItembyRegNomerKBK(std::string reg_nomer, std::string KBKfromXML, double aValue) {
    int result = 0;
    int pokaz_id;
    string pokaz_bk_code;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id, pokaz_id FROM Denegn_potok");
    q->SQL->Add("WHERE reg_nomer = :reg_nomer");
    q->SQL->Add("AND value = :aValue");
    q->ParamByName("reg_nomer")->AsString() = reg_nomer;
    q->ParamByName("aValue")->AsFloat() = aValue;
    q->Open();

    if (!q->EoF()) {
        pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
        pokaz_bk_code = TDB_Pokaztl::GetCodeBK(pokaz_id);
        if (pokaz_bk_code.length() == KBKfromXML.length()) {
            if (pokaz_bk_code == KBKfromXML)
                result = q->FieldByName("den_potok_id")->AsInteger();
        } else {
            if (pokaz_bk_code.find(KBKfromXML) != string::npos)
                result = q->FieldByName("den_potok_id")->AsInteger();
        }
    }

    //MAK q->Free();
    delete q;
    return result;
}

int TDB_DenegnPotok::IsItembyRegNomerTBandOther(std::string reg_nomer, TDateTime value_date, double value,
                                                std::string nom_plategki) {
    int result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id, pokaz_id FROM Denegn_potok");
    q->SQL->Add("WHERE reg_nomer = :reg_nomer");
    q->SQL->Add("AND value_date = :value_date");
    q->SQL->Add("AND value = :value");
    q->SQL->Add("AND nom_plategki = :nom_plategki");

    q->ParamByName("reg_nomer")->AsString() = reg_nomer;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ParamByName("value")->AsFloat() = value;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("den_potok_id")->AsInteger();
    }

    //MAK q->Free();
    delete q;
    return result;
}

int
TDB_DenegnPotok::IsPPbyInfo(int plan_id, int vid_otc, int objectid, std::string nom_plategki, TDateTime date_plategki,
                            TDateTime value_date, double value) {
    int den_potok_id = 0;
    double wvalue;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND objectid = :objectid");
    q->SQL->Add("AND nom_plategki = :nom_plategki");
    if (date_plategki != 0)
        q->SQL->Add("AND date_plategki = :date_plategki");
    q->SQL->Add("AND value_date = :value_date");

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    if (date_plategki != 0)
        q->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();

    while (!q->EoF()) {
        wvalue = q->FieldByName("value")->AsFloat();
        if (SameValue(wvalue, value, 0.001)) {
            den_potok_id = q->FieldByName("den_potok_id")->AsInteger();
            break;
        }
        q->Next();
    }

    //MAK q->Free();
    delete q;
    return den_potok_id;
}

int TDB_DenegnPotok::IsItembyPlategkaPropertyGBU(int plan_id, std::string nom_plategki, TDateTime date_plategki,
                                                 TDateTime value_date, double value) {
    int den_potok_id = 0;
    double wvalue;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id, pokaz_id, value FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND nom_plategki = :nom_plategki");
    q->SQL->Add("AND date_plategki = :date_plategki");
    q->SQL->Add("AND value_date = :value_date");

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();

    while (!q->EoF()) {
        wvalue = q->FieldByName("value")->AsFloat();
        if (SameValue(wvalue, value, 0.001)) {
            den_potok_id = q->FieldByName("den_potok_id")->AsInteger();
            break;
        }
        q->Next();
    }

    //MAK q->Free();
    delete q;
    return den_potok_id;
}

int TDB_DenegnPotok::IsItembyPlategkaProperty(int plan_id, std::string nom_plategki, TDateTime date_plategki,
                                              std::string BnkRS_PlatPoluc, std::string nBO, TDateTime operDay,
                                              double value) {
    int den_potok_id = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Add("SELECT den_potok_id, pokaz_id, value, date_plategki, BnkRS_PlatPoluc, NBOneprisoed FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND nom_plategki = :nom_plategki");
    q->SQL->Add("AND value_date = :operDay");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ParamByName("operDay")->AsDateTime() = operDay;
    q->Open();

    while (!q->EoF()) {
        if (SameValue(q->FieldByName("value")->AsFloat(), value, 0.001)) {
            if ((q->FieldByName("date_plategki")->IsNull() || (q->FieldByName("date_plategki")->AsDateTime() == 0)) &&
                (q->FieldByName("BnkRS_PlatPoluc")->IsNull() ||
                 (q->FieldByName("BnkRS_PlatPoluc")->AsString() == ""))) {
                den_potok_id = q->FieldByName("den_potok_id")->AsInteger();
                Upddate_BnkRS(den_potok_id, date_plategki, BnkRS_PlatPoluc, nBO);
                break;
            }

            if ((q->FieldByName("date_plategki")->AsDateTime() == date_plategki) &&
                (q->FieldByName("BnkRS_PlatPoluc")->AsString() == BnkRS_PlatPoluc) &&
                (q->FieldByName("NBOneprisoed")->AsString() == nBO)) {
                den_potok_id = q->FieldByName("den_potok_id")->AsInteger();
                break;
            }
        }
        q->Next();
    }

    //MAK q->Free();
    delete q;
    return den_potok_id;
}

void TDB_DenegnPotok::Upddate_BnkRS(int den_potok_id, TDateTime date_plategki,
                                    std::string BnkRS_PlatPoluc, std::string nBO) {
    TQueryUni *qDP = new TQueryUni(DefDBconnect.Conn);

    qDP->Close();
    qDP->SQL->Clear();
    qDP->SQL->Add("UPDATE Denegn_potok");
    qDP->SQL->Add(
            "SET date_plategki = :date_plategki, BnkRS_PlatPoluc = :BnkRS_PlatPoluc, NBOneprisoed = :NBOneprisoed");
    qDP->SQL->Add("WHERE den_potok_id = :den_potok_id");
    qDP->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    qDP->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    qDP->ParamByName("BnkRS_PlatPoluc")->AsString() = BnkRS_PlatPoluc;
    qDP->ParamByName("NBOneprisoed")->AsString() = nBO;
    qDP->ExecSQL();

    delete qDP;
}

int TDB_DenegnPotok::WriteItem(std::string reg_nomer, int objectId, int plan_id, int pokaz_id, int cell_id, int vid_otc,
                               TDateTime value_date, double value, int coef, std::string nom_documenta,
                               std::string attrib1, std::string attrib2,
                               std::string attrib3, std::string fromXML, std::string nom_plategki,
                               TDateTime date_plategki, std::string NBOneprisoed,
                               std::string BnkRS_PlatPoluc) {
    int den_potok_id = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Denegn_potok");
    q->SQL->Add(
            "(objectid, plan_id, pokaz_id, cell_id, vid_otc, value_date, budget_poluc_id, value, coef, nom_documenta,");
    q->SQL->Add(
            " attrib1, attrib2, attrib3, reg_nomer, fromXML, nom_plategki, date_plategki, NBOneprisoed, BnkRS_PlatPoluc)");
    q->SQL->Add(
            "VALUES (:objectid, :plan_id, :pokaz_id, :cell_id, :vid_otc, :value_date, 0, :value, :coef, :nom_documenta,");
    q->SQL->Add(
            ":attrib1, :attrib2, :attrib3, :reg_nomer, :fromXML, :nom_plategki, :date_plategki, :NBOneprisoed, :BnkRS_PlatPoluc)");

    q->ParamByName("objectid")->AsInteger() = objectId;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ParamByName("value")->AsFloat() = value;
    q->ParamByName("coef")->AsInteger() = coef;
    q->ParamByName("nom_documenta")->AsString() = nom_documenta;
    q->ParamByName("attrib1")->AsString() = attrib1;
    q->ParamByName("attrib2")->AsString() = attrib2;
    q->ParamByName("attrib3")->AsString() = attrib3;
    q->ParamByName("reg_nomer")->AsString() = reg_nomer;
    q->ParamByName("fromXML")->AsString() = fromXML;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    q->ParamByName("NBOneprisoed")->AsString() = NBOneprisoed;
    q->ParamByName("BnkRS_PlatPoluc")->AsString() = BnkRS_PlatPoluc;

    q->ExecSQL();
    //MAK q->Free();
    delete q;
    den_potok_id = TDB_DenegnPotok::GetId();
    return den_potok_id;
}

int TDB_DenegnPotok::WriteItemDO(int objectId, int plan_id, int pokaz_id, int cell_id, int vid_otc,
                                 TDateTime value_date, double value, int coef, std::string RCV_INN,
                                 std::string RCV_CPP, std::string RCV_NAME, std::string KBK_000_KOSGU,
                                 std::string PURPOSE, std::string nom_plategki, TDateTime date_plategki,
                                 std::string nomerDO, TDateTime file_date, std::string reg_nomer,
                                 std::string nom_documenta) {
    int den_potok_id = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Denegn_potok");
    q->SQL->Add("(objectid, plan_id, pokaz_id, cell_id, vid_otc, value_date, budget_poluc_id, ");
    q->SQL->Add("value, coef, attrib4, fromXML, nom_plategki, date_plategki, NBOneprisoed, ");
    q->SQL->Add("attrib1, attrib2, attrib5, attrib3, reg_nomer, nom_documenta)");
    q->SQL->Add("values (:objectid, :plan_id, :pokaz_id, :cell_id, :vid_otc, :value_date, 0, ");
    q->SQL->Add(":value, :coef, :attrib4, :fromXML, :nom_plategki, :date_plategki, :NBOneprisoed, ");
    q->SQL->Add(":attrib1, :attrib2, :attrib5, :attrib3, :reg_nomer, :nom_documenta)");
    q->ParamByName("objectid")->AsInteger() = objectId;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ParamByName("value")->AsFloat() = value;
    q->ParamByName("coef")->AsInteger() = coef;
    q->ParamByName("attrib4")->AsString() = KBK_000_KOSGU;
    q->ParamByName("fromXML")->AsString() = PURPOSE;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    q->ParamByName("NBOneprisoed")->AsString() = nomerDO;
    q->ParamByName("attrib1")->AsString() = RCV_INN;
    q->ParamByName("attrib2")->AsString() = RCV_CPP;
    if (RCV_NAME.length() > 2000)
        RCV_NAME = RCV_NAME.substr(0, 2000);
    q->ParamByName("attrib5")->AsString() = RCV_NAME;
    q->ParamByName("attrib3")->AsString() = DateTimeToStr(file_date);
    q->ParamByName("reg_nomer")->AsString() = reg_nomer;
    q->ParamByName("nom_documenta")->AsString() = nom_documenta;

    q->ExecSQL();
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as den_potok_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Denegn_potok', 'den_potok_id')) as den_potok_id");
            break;
    }
    q->Open();
    den_potok_id = q->FieldByName("den_potok_id")->AsInteger();

    //MAK q->Free();
    delete q;
    return den_potok_id;
}

int
TDB_DenegnPotok::WriteItem2(std::string reg_nomer, int objectId, int plan_id, int pokaz_id, int cell_id, int vid_otc,
                            TDateTime value_date, double value, int coef, std::string nom_documenta,
                            std::string attrib1, std::string attrib2, std::string attrib3, std::string fromXML,
                            std::string nom_plategki, TDateTime date_plategki, int budgetpokaz_id,
                            std::string NBOneprisoed, std::string BnkRS_PlatPoluc) {
    int den_potok_id = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Denegn_potok");
    q->SQL->Add(
            "(objectid, plan_id, pokaz_id, cell_id, vid_otc, value_date, budget_poluc_id, value, coef, nom_documenta, ");
    q->SQL->Add(
            "attrib1, attrib2, attrib3, reg_nomer, fromXML, nom_plategki, date_plategki, budgetpokaz_id, NBOneprisoed, BnkRS_PlatPoluc)");
    q->SQL->Add(
            "values (:objectid, :plan_id, :pokaz_id, :cell_id, :vid_otc, :value_date, 0, :value, :coef, :nom_documenta, ");
    q->SQL->Add(
            ":attrib1, :attrib2, :attrib3, :reg_nomer, :fromXML, :nom_plategki, :date_plategki, :budgetpokaz_id, :NBOneprisoed, :BnkRS_PlatPoluc)");
    q->ParamByName("objectid")->AsInteger() = objectId;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ParamByName("value")->AsFloat() = value;
    q->ParamByName("coef")->AsInteger() = coef;
    q->ParamByName("nom_documenta")->AsString() = nom_documenta;
    q->ParamByName("attrib1")->AsString() = attrib1;
    q->ParamByName("attrib2")->AsString() = attrib2;
    q->ParamByName("attrib3")->AsString() = attrib3;
    q->ParamByName("reg_nomer")->AsString() = reg_nomer;
    q->ParamByName("fromXML")->AsString() = fromXML;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    q->ParamByName("budgetpokaz_id")->AsInteger() = budgetpokaz_id;
    q->ParamByName("NBOneprisoed")->AsString() = NBOneprisoed;
    q->ParamByName("BnkRS_PlatPoluc")->AsString() = BnkRS_PlatPoluc;

    q->ExecSQL();
    //MAK q->Free();
    delete q;
    den_potok_id = TDB_DenegnPotok::GetId();
    return den_potok_id;
}

int
TDB_DenegnPotok::WriteItemBPid(std::string reg_nomer, int objectId, int plan_id, int pokaz_id, int cell_id, int vid_otc,
                               TDateTime value_date, double value, int coef, std::string nom_documenta,
                               std::string attrib1,
                               std::string attrib2, std::string attrib3, std::string fromXML, std::string nom_plategki,
                               TDateTime date_plategki, int budget_poluc_id, std::string NBOneprisoed,
                               std::string BnkRS_PlatPoluc) {
    int den_potok_id = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Denegn_potok");
    q->SQL->Add(
            "(objectid, plan_id, pokaz_id, cell_id, vid_otc, value_date, budget_poluc_id, value, coef, nom_documenta, ");
    q->SQL->Add(
            "attrib1, attrib2, attrib3, reg_nomer, fromXML, nom_plategki, date_plategki, NBOneprisoed, BnkRS_PlatPoluc)");
    q->SQL->Add(
            "values (:objectid, :plan_id, :pokaz_id, :cell_id, :vid_otc, :value_date, :budget_poluc_id, :value, :coef, :nom_documenta, ");
    q->SQL->Add(
            ":attrib1, :attrib2, :attrib3, :reg_nomer, :fromXML, :nom_plategki, :date_plategki, :NBOneprisoed, :BnkRS_PlatPoluc)");
    q->ParamByName("objectid")->AsInteger() = objectId;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ParamByName("budget_poluc_id")->AsInteger() = budget_poluc_id;
    q->ParamByName("value")->AsFloat() = value;
    q->ParamByName("coef")->AsInteger() = coef;
    q->ParamByName("nom_documenta")->AsString() = nom_documenta;
    q->ParamByName("attrib1")->AsString() = attrib1;
    q->ParamByName("attrib2")->AsString() = attrib2;
    q->ParamByName("attrib3")->AsString() = attrib3;
    q->ParamByName("reg_nomer")->AsString() = reg_nomer;
    q->ParamByName("fromXML")->AsString() = fromXML;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    q->ParamByName("NBOneprisoed")->AsString() = NBOneprisoed;
    q->ParamByName("BnkRS_PlatPoluc")->AsString() = BnkRS_PlatPoluc;

    q->ExecSQL();
    //MAK q->Free();
    delete q;
    den_potok_id = TDB_DenegnPotok::GetId();
    return den_potok_id;
}

int TDB_DenegnPotok::WriteItemZajvka(std::string grnlGrFin_idS, std::string nPoPorjdkuS, int objectId, std::string nDO,
                                     int field_id, int cell_id, int plan_id, double value, TDateTime value_date,
                                     std::string nom_plategki,
                                     TDateTime date_plategki, std::string naznacPlatega, std::string KBK,
                                     std::string kontrAgent) {
    int den_potok_id = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Denegn_potok");
    q->SQL->Add("(reg_nomer, nom_documenta, objectid, plan_id, pokaz_id, cell_id, vid_otc, value, value_date, coef,");
    q->SQL->Add("attrib3, attrib4, attrib5, fromXML, nom_plategki, date_plategki, budget_poluc_id, NBOneprisoed)");
    q->SQL->Add(
            "values (:reg_nomer, :nom_documenta, :objectid, :plan_id, :pokaz_id, :cell_id, :vid_otc, :value, :value_date, 0, ");
    q->SQL->Add(
            ":attrib3, :attrib4, :attrib5, :fromXML, :nom_plategki, :date_plategki, :budget_poluc_id, :NBOneprisoed)");

    q->ParamByName("reg_nomer")->AsString() = grnlGrFin_idS;
    q->ParamByName("nom_documenta")->AsString() = nPoPorjdkuS;
    q->ParamByName("objectid")->AsInteger() = objectId;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = field_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = 2;
    q->ParamByName("value")->AsFloat() = value;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ParamByName("attrib3")->AsString() = "Заявка";
    q->ParamByName("attrib4")->AsString() = KBK;
    q->ParamByName("attrib5")->AsString() = kontrAgent;
    q->ParamByName("fromXML")->AsString() = naznacPlatega;
    q->ParamByName("budget_poluc_id")->AsInteger() = 0;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    q->ParamByName("NBOneprisoed")->AsString() = nDO;

    q->ExecSQL();
    //MAK q->Free();
    delete q;
    den_potok_id = TDB_DenegnPotok::GetId();
    return den_potok_id;
}

bool TDB_DenegnPotok::UpdateItem(int den_potok_id, int objectId, double value, std::string nom_documenta,
                                 std::string attrib1, std::string attrib3) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;

    if (value == 0.0) {
        result = TDB_DenegnPotok::DeleteItem(den_potok_id);
        result = true;
    } else {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Denegn_potok SET ");
        q->SQL->Add("objectid = :objectid, value = :value, nom_documenta = :nom_documenta, ");
        q->SQL->Add("attrib1 = :attrib1, attrib3 = :attrib3 ");
        q->SQL->Add("WHERE den_potok_id = :den_potok_id");

        q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
        q->ParamByName("objectid")->AsInteger() = objectId;
        q->ParamByName("value")->AsFloat() = value;
        q->ParamByName("nom_documenta")->AsString() = nom_documenta;
        q->ParamByName("attrib1")->AsString() = attrib1;
        q->ParamByName("attrib3")->AsString() = attrib3;

        q->ExecSQL();
        result = true;
    }

    delete q;
    return result;
}


bool TDB_DenegnPotok::UpdateItem2(int den_potok_id, int objectId, double value, std::string nom_documenta,
                                  std::string attrib1, std::string attrib3, int budgetpokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;

    if (value == 0.0) {
        result = TDB_DenegnPotok::DeleteItem(den_potok_id);
        result = true;
    } else {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Denegn_potok SET ");
        q->SQL->Add("objectid = :objectid, ");
        q->SQL->Add("value = :value, ");
        q->SQL->Add("nom_documenta = :nom_documenta, ");
        q->SQL->Add("attrib1 = :attrib1, ");
        q->SQL->Add("attrib3 = :attrib3, ");
        q->SQL->Add("budgetpokaz_id = :budgetpokaz_id ");
        q->SQL->Add("WHERE den_potok_id = :den_potok_id");

        q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
        q->ParamByName("objectid")->AsInteger() = objectId;
        q->ParamByName("value")->AsFloat() = value;
        q->ParamByName("nom_documenta")->AsString() = nom_documenta;
        q->ParamByName("attrib1")->AsString() = attrib1;
        q->ParamByName("attrib3")->AsString() = attrib3;
        q->ParamByName("budgetpokaz_id")->AsInteger() = budgetpokaz_id;

        q->ExecSQL();
        result = true;
    }

    delete q;
    return result;
}


void TDB_DenegnPotok::UpdValue(int den_potok_id, double value) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Denegn_potok");
    q->SQL->Add("SET value = :value");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->ParamByName("value")->AsFloat() = value;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::UpdIdFromPref(int den_potok_id, int id_FromPref) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Denegn_potok");
    q->SQL->Add("SET id_FromPref = :id_FromPref");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->ParamByName("id_FromPref")->AsInteger() = id_FromPref;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::UpdBudgetpokaz_id(int den_potok_id, int budgetpokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Denegn_potok");
    q->SQL->Add("SET budgetpokaz_id = :budgetpokaz_id");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->ParamByName("budgetpokaz_id")->AsInteger() = budgetpokaz_id;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::UpdFromXML(int den_potok_id, const std::string fromXML) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Denegn_potok");
    q->SQL->Add("SET fromXML = :fromXML");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->ParamByName("fromXML")->AsString() = fromXML;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::UpdNPlategki(int den_potok_id, const std::string nom_plategki) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Denegn_potok");
    q->SQL->Add("SET nom_plategki = :nom_plategki");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->ParamByName("nom_plategki")->AsString() = nom_plategki;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::UpdDatePlategki(int den_potok_id, const TDateTime date_plategki) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Denegn_potok");
    q->SQL->Add("SET date_plategki = :date_plategki");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->ParamByName("date_plategki")->AsDateTime() = date_plategki;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::UpdNBOneprisoedObj(int den_potok_id, const std::string NBOneprisoed, int objectid) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Denegn_potok");
    q->SQL->Add("SET NBOneprisoed = :NBOneprisoed, objectid = :objectid");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->ParamByName("NBOneprisoed")->AsString() = NBOneprisoed;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void
TDB_DenegnPotok::UpdNBOneprisoedToNULL(int den_potok_id, int objectid, int field_Finansir_id, int cell_id_FinansirMes) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Denegn_potok");
    q->SQL->Add("SET NBOneprisoed = NULL, objectid = :objectid, pokaz_id = :pokaz_id, cell_id = :cell_id");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("pokaz_id")->AsInteger() = field_Finansir_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id_FinansirMes;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::UpdKFO_CelSubs(int den_potok_id, const std::string CODE_KFO, const std::string codeCelSubs) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Denegn_potok");
    q->SQL->Add("SET CODE_KFO = :CODE_KFO, codeCelSubs = :codeCelSubs");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->ParamByName("CODE_KFO")->AsString() = CODE_KFO;
    q->ParamByName("codeCelSubs")->AsString() = codeCelSubs;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

bool TDB_DenegnPotok::DeleteItem(int den_potok_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT docum_id FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->SQL->Add("AND docum_id is not NULL");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    DeleteDocums(q);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    bool result = q->ExecSQL();
    delete q;
    return result;
}

void TDB_DenegnPotok::DeleteItemByObj(int objectid) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT docum_id FROM Denegn_potok");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND docum_id is not NULL");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->Open();
    DeleteDocums(q);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Denegn_potok");
    q->SQL->Add("WHERE objectid = :objectid");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ExecSQL();
    delete q;
}

void TDB_DenegnPotok::DeleteItemByCellId(int cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT docum_id FROM Denegn_potok");
    q->SQL->Add("WHERE cell_id = :cell_id");
    q->SQL->Add("AND docum_id is not NULL");
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->Open();
    DeleteDocums(q);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Denegn_potok");
    q->SQL->Add("WHERE cell_id = :cell_id");
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::DeleteItemByPlanCellId(int plan_id, int cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT docum_id FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND cell_id = :cell_id");
    q->SQL->Add("AND docum_id is not NULL");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->Open();

    DeleteDocums(q);

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND cell_id = :cell_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ExecSQL();

    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::DeleteDocums(TQueryUni *q) {
    q->First();
    while (!q->EoF()) {
        string docum_id = q->FieldByName("docum_id")->AsString();
//vernut         TAPIDocum::DelDocum(docum_id);
        q->Next();
    }
}

bool TDB_DenegnPotok::GetItem(TQueryUni *qIn, int den_potok_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT ");
    qIn->SQL->Add("den_potok_id,plan_id,pokaz_id,cell_id,vid_otc,value_date,budget_poluc_id,nom_documenta,");
    qIn->SQL->Add("value,coef,str_value,attrib1,attrib2,attrib3,attrib4,attrib5,objectid,reg_nomer,fromXML,");
    qIn->SQL->Add(
            "nom_plategki,date_plategki,payer_id,budgetpokaz_id,id_FromPref,NBOneprisoed,BnkRS_PlatPoluc,nom_documenta");
    qIn->SQL->Add("FROM Denegn_potok");
    qIn->SQL->Add("WHERE den_potok_id = :den_potok_id");
    qIn->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    qIn->Open();
    if (qIn->EoF()) {
        return false;
    }
    return true;
}

void TDB_DenegnPotok::getDataForDenPotokId(int den_potok_id, int &plan_id, int &objectid, int &pokaz_id, int &cell_id,
                                           int &budgetpokaz_id, int &budget_poluc_id, double &summa,
                                           TDateTime &value_date, std::string &attrib3, std::string &file_name,
                                           TDateTime &file_date, std::string &fromXML, std::string &reg_nomer,
                                           std::string &nom_plategki, TDateTime &date_plategki,
                                           std::string &NBOneprisoed, std::string &BnkRS_PlatPoluc,
                                           std::string &nom_documenta) {

    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    GetItem(qW, den_potok_id);

    plan_id = qW->FieldByName("plan_id")->AsInteger();
    objectid = qW->FieldByName("objectid")->AsInteger();
    pokaz_id = qW->FieldByName("pokaz_id")->AsInteger();
    budgetpokaz_id = qW->FieldByName("budgetpokaz_id")->AsInteger();
    cell_id = qW->FieldByName("cell_id")->AsInteger();
    budget_poluc_id = qW->FieldByName("budget_poluc_id")->AsInteger();

    summa = qW->FieldByName("value")->AsFloat();
    value_date = qW->FieldByName("value_date")->AsDateTime();

    attrib3 = qW->FieldByName("attrib3")->AsString();

    std::string s = attrib3.substr(14);
    s = Trim(s);
    int n = s.find(".xml");
    if (n != std::string::npos) {
        file_name = s.substr(0, n + 4);
        s = s.substr(n + 5);
        s = Trim(s);
        try {
            file_date = StrToDateTime(s.c_str());
        }
        catch (...) {
            file_date = Now();
        }
    } else {
        file_name = s;
        file_date = Now();
    }

    fromXML = qW->FieldByName("fromXML")->AsString();
    reg_nomer = qW->FieldByName("reg_nomer")->AsString();
    nom_plategki = qW->FieldByName("nom_plategki")->AsString();
    date_plategki = qW->FieldByName("date_plategki")->AsDateTime();

    NBOneprisoed = qW->FieldByName("NBOneprisoed")->AsString();
    BnkRS_PlatPoluc = qW->FieldByName("BnkRS_PlatPoluc")->AsString();
    nom_documenta = qW->FieldByName("nom_documenta")->AsString();

    delete qW;
}

std::string TDB_DenegnPotok::GetAttrib3(int den_potok_id, std::string &fromXML, std::string &NBOneprisoed,
                                        std::string &BnkRS_PlatPoluc) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT attrib3, fromXML, NBOneprisoed, BnkRS_PlatPoluc");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("attrib3")->AsString();
        fromXML = q->FieldByName("fromXML")->AsString();
        NBOneprisoed = q->FieldByName("NBOneprisoed")->AsString();
        BnkRS_PlatPoluc = q->FieldByName("BnkRS_PlatPoluc")->AsString();
    }
    //MAK q->Free();
    delete q;
    return result;
}

std::string TDB_DenegnPotok::GetNPlategki(int den_potok_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    if (den_potok_id == 0) return result;
    q->SQL->Clear();
    q->SQL->Add("SELECT nom_plategki");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("nom_plategki")->AsString();
    }
    //MAK q->Free();
    delete q;
    return result;
}

TDateTime TDB_DenegnPotok::GetDPlategki(int den_potok_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDateTime result = 0;
    if (den_potok_id == 0) return result;
    q->SQL->Clear();
    q->SQL->Add("SELECT date_plategki");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("date_plategki")->AsDateTime();
    }
    //MAK q->Free();
    delete q;
    return result;
}

int TDB_DenegnPotok::GetPayerId(int den_potok_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT payer_id");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("payer_id")->AsInteger();
    }
    //MAK q->Free();
    delete q;
    return result;
}

int TDB_DenegnPotok::GetBudgetpokaz_id(int den_potok_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    if (den_potok_id == 0) return result;
    q->SQL->Clear();
    q->SQL->Add("SELECT budgetpokaz_id");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("budgetpokaz_id")->AsInteger();
    }
    //MAK q->Free();
    delete q;
    return result;
}

TDateTime TDB_DenegnPotok::GetValue_date(int den_potok_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDateTime result = 0;
    if (den_potok_id == 0) return result;
    q->SQL->Clear();
    q->SQL->Add("SELECT value_date");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("value_date")->AsDateTime();
    }
    //MAK q->Free();
    delete q;
    return result;
}

double TDB_DenegnPotok::GetValue(int den_potok_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0;
    if (den_potok_id == 0) return result;
    q->SQL->Clear();
    q->SQL->Add("SELECT value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("value")->AsFloat();
    }
    //MAK q->Free();
    delete q;
    return result;
}

std::string TDB_DenegnPotok::GetNom_documenta(int den_potok_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    if (den_potok_id == 0) return result;
    q->SQL->Clear();
    q->SQL->Add("SELECT nom_documenta");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("nom_documenta")->AsString();
    }
    //MAK q->Free();
    delete q;
    return result;
}

std::string TDB_DenegnPotok::GetFromXML(int den_potok_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    if (den_potok_id == 0) return result;
    q->SQL->Clear();
    q->SQL->Add("SELECT fromXML");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("fromXML")->AsString();
    }
    //MAK q->Free();
    delete q;
    return result;
}

void TDB_DenegnPotok::GetPokazCell_id(int den_potok_id, int &pokaz_id, int &cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id, cell_id");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE den_potok_id = :den_potok_id");
    q->ParamByName("den_potok_id")->AsInteger() = den_potok_id;
    q->Open();
    if (!q->EoF()) {
        pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
        cell_id = q->FieldByName("cell_id")->AsInteger();
    }
    //MAK q->Free();
    delete q;
}

void TDB_DenegnPotok::SumByDogsForMeropr_Write(int plan_id, int pokaz_field_id, int pokaz_meropr_id, int link_pokaz_id,
                                               int vid_otc, TDateTime value_date, int cell_id_Mes_Meropr,
                                               int budgetpokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string wwAvtoRascet = CAvtoRascet;
    if (pokaz_meropr_id == budgetpokaz_id) {
        wwAvtoRascet = CAvtoRascetMrprBdg;
    }
    int Y, M, D;
    DecodeDate(value_date, Y, M, D);
    double value = 0;
    int den_potok_id = 0;
    std::string attrib3;
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM LinksObjects, Denegn_potok");
    q->SQL->Add("WHERE LinksObjects.plan_id = :plan_id");
    q->SQL->Add("AND LinksObjects.pokaz_id = :pokaz_meropr_id");
    q->SQL->Add("AND LinksObjects.link_pokaz_id = :link_pokaz_id");
    q->SQL->Add("AND Denegn_potok.plan_id = LinksObjects.plan_id");
    q->SQL->Add("AND Denegn_potok.objectid = LinksObjects.objectid");
    q->SQL->Add("AND Denegn_potok.vid_otc = :vid_otc");
    q->SQL->Add("AND Denegn_potok.pokaz_id = :pokaz_field_id");
    q->SQL->Add("AND Denegn_potok.value_date = :value_date");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_meropr_id")->AsInteger() = pokaz_meropr_id;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("pokaz_field_id")->AsInteger() = pokaz_field_id;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();
    if (!q->EoF()) {
        value = q->FieldByName("value")->AsFloat();
    }
    //MAK q->Free();
    delete q;
    den_potok_id = TDB_DenegnPotok::IsItembyCAvto(wwAvtoRascet, plan_id, pokaz_meropr_id, cell_id_Mes_Meropr, vid_otc,
                                                  value_date);
    attrib3 = "";
    if (vid_otc == 5) {
        attrib3 = wwAvtoRascet + DateTimeToStr(Now()) + " (" + GetUserLogin() + ")";
    }
    if (den_potok_id == 0) {
        den_potok_id = TDB_DenegnPotok::WriteItem2("Авт", 0, plan_id, pokaz_meropr_id, cell_id_Mes_Meropr, vid_otc,
                                                   value_date, value, 0, "", "", "", attrib3, "", "", 0, budgetpokaz_id,
                                                   "", "");
    } else {
        TDB_DenegnPotok::UpdateItem2(den_potok_id, 0, value, "", "", attrib3, budgetpokaz_id);
    }
}

double TDB_DenegnPotok::GetSumKVforMeropr(int plan_id, int pokaz_meropr_id, int cell_id_Mes_Meropr,
                                          TDateTime Date1, TDateTime Date2) {
    double summaD = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND pokaz_id = :PokazId");
    q->SQL->Add("AND cell_id = :CellIdMes");
    q->SQL->Add("AND objectid = 0");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");
    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("PokazId")->AsInteger() = pokaz_meropr_id;
    q->ParamByName("CellIdMes")->AsInteger() = cell_id_Mes_Meropr;
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;
    q->Open();
    if (!q->EoF()) {
        summaD = q->FieldByName("value")->AsFloat();
    }
    //MAK q->Free();
    delete q;
    return summaD;
}

double TDB_DenegnPotok::GetSumZaPeriodforP_M_D(int plan_id, int pokaz_id, int cell_id, int ObjectId,
                                               TDateTime Date1, TDateTime Date2) {
    double summaD = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND pokaz_id = :PokazId");
    q->SQL->Add("AND cell_id = :CellIdMes");
    q->SQL->Add("AND objectid = :objectid");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND value_date BETWEEN :Date1 AND :Date2");
    q->ParamByName("PlanId")->AsInteger() = plan_id;
    q->ParamByName("PokazId")->AsInteger() = pokaz_id;
    q->ParamByName("CellIdMes")->AsInteger() = cell_id;
    q->ParamByName("objectid")->AsInteger() = ObjectId;
    q->ParamByName("Date1")->AsDateTime() = Date1;
    q->ParamByName("Date2")->AsDateTime() = Date2;
    q->Open();
    if (!q->EoF()) {
        summaD = q->FieldByName("value")->AsFloat();
        if (IsZero(summaD, 0.0000001)) summaD = 0;
    }
    //MAK q->Free();
    delete q;
    return summaD;
}

double TDB_DenegnPotok::GetValueItogoMesjc(int aPokaz_id, int aPlan_id, TDateTime aFromDate,
                                           TDateTime aToDate, int aVidOtch, int aCell_id) {
    double ValueItogoMesjc = 0;
    TFDStoredProc *sp = new TFDStoredProc(DefDBconnect.Conn);
    sp->storedProcName = "GetValueItogoMesjc";

    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->Params->CreateParam(ftInteger, "@aPokaz_id", ptInput);
            sp->Params->CreateParam(ftInteger, "@aPlan_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "@aFromDate", ptInput);
            sp->Params->CreateParam(ftDateTime, "@aToDate", ptInput);
            sp->Params->CreateParam(ftInteger, "@aVidOtch", ptInput);
            sp->Params->CreateParam(ftInteger, "@aCellId", ptInput);
            sp->Params->CreateParam(ftFloat, "@ValueItogoMesjc", ptOutput);

            sp->Params->ParamByName("@aPokaz_id")->AsInteger() = aPokaz_id;
            sp->Params->ParamByName("@aPlan_id")->AsInteger() = aPlan_id;
            sp->Params->ParamByName("@aFromDate")->AsDateTime() = aFromDate;
            sp->Params->ParamByName("@aToDate")->AsDateTime() = aToDate;
            sp->Params->ParamByName("@aVidOtch")->AsInteger() = aVidOtch;
            sp->Params->ParamByName("@aCellId")->AsInteger() = aCell_id;

            sp->ExecProc();
            ValueItogoMesjc = sp->ParamByName("@ValueItogoMesjc")->AsFloat();
            break;
        }
        case TDBType::Postgre: {
            sp->Params->CreateParam(ftInteger, "aPokaz_id", ptInput);
            sp->Params->CreateParam(ftInteger, "aPlan_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "aFromDate", ptInput);
            sp->Params->CreateParam(ftDateTime, "aToDate", ptInput);
            sp->Params->CreateParam(ftInteger, "aVidOtch", ptInput);
            sp->Params->CreateParam(ftInteger, "aCellId", ptInput);
            sp->Params->CreateParam(ftFloat, "ValueItogoMesjc", ptOutput);

            sp->Params->ParamByName("aPokaz_id")->AsInteger() = aPokaz_id;
            sp->Params->ParamByName("aPlan_id")->AsInteger() = aPlan_id;
            sp->Params->ParamByName("aFromDate")->AsDateTime() = aFromDate;
            sp->Params->ParamByName("aToDate")->AsDateTime() = aToDate;
            sp->Params->ParamByName("aVidOtch")->AsInteger() = aVidOtch;
            sp->Params->ParamByName("aCellId")->AsInteger() = aCell_id;

            sp->ExecProc();
            ValueItogoMesjc = sp->ParamByName("ValueItogoMesjc")->AsFloat();
            break;
        }
    }
    delete sp;
    return ValueItogoMesjc;
}

double TDB_DenegnPotok::GetSummaFactNI(int plan_id, int pokaz_id, int ist_fin_id, int fmForm_CurrYear) {
    TDateTime ToDate = Now();
    if (fmForm_CurrYear < YearOf(ToDate)) {
        ToDate = EncodeDate(fmForm_CurrYear, 12, 31);
    }
    TDateTime po = ToDate;
    int Year, Month, Day;
    DecodeDate(po, Year, Month, Day);
    TDateTime s = EncodeDate(Year, 1, 1);

    int link_pokaz_id = TDB_Dgvr::GetIdFieldMeropr();
    double summa = 0;

    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);

    qW->SQL->Clear();
    qW->SQL->Add("SELECT a.value, a.value_date, a.reg_nomer");
    qW->SQL->Add("FROM Denegn_potok a");
    qW->SQL->Add("WHERE a.plan_id = :plan_id");
    qW->SQL->Add("AND a.value_date BETWEEN :s AND :po");
    qW->SQL->Add("AND a.objectid = 0");
    qW->SQL->Add("AND a.vid_otc = 5");
    qW->SQL->Add("AND a.Pokaz_id = :pokaz_id");
    qW->SQL->Add("UNION");
    qW->SQL->Add("SELECT a.value, a.value_date, a.reg_nomer");
    qW->SQL->Add("FROM Denegn_potok a, LinksObjects d");
    qW->SQL->Add("WHERE a.plan_id = :plan_id1");
    qW->SQL->Add("AND a.value_date BETWEEN :s1 AND :po1");
    qW->SQL->Add("AND a.objectid <> 0");
    qW->SQL->Add("AND a.vid_otc = 5");
    qW->SQL->Add("AND d.plan_id = a.plan_id");
    qW->SQL->Add("AND d.objectid = a.objectid");
    qW->SQL->Add("AND d.link_pokaz_id = :link_pokaz_id");
    qW->SQL->Add("AND d.Pokaz_id = :pokaz_id1");
    qW->SQL->Add("ORDER BY value_date");

    qW->ParamByName("plan_id")->AsInteger() = plan_id;
    qW->ParamByName("plan_id1")->AsInteger() = plan_id;
    qW->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qW->ParamByName("pokaz_id1")->AsInteger() = pokaz_id;
    qW->ParamByName("s")->AsDateTime() = s;
    qW->ParamByName("s1")->AsDateTime() = s;
    qW->ParamByName("po")->AsDateTime() = po;
    qW->ParamByName("po1")->AsDateTime() = po;
    qW->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    qW->Open();

    while (!qW->EoF()) {
        if (TDB_DenegnPotok::isFromAisUBP(qW->FieldByName("reg_nomer")->AsString())) {
            summa += qW->FieldByName("value")->AsFloat();
        }
        qW->Next();
    }
    delete qW;
    return summa;
}

void
TDB_DenegnPotok::SumbyMeroprsForBudgPokaz_Write(int plan_id, int budgetpokaz_id, int cell_id_Mes_Budgpokaz, int vid_otc,
                                                TDateTime value_date) {
    int Y, M, D;
    double value = 0;
    int den_potok_id;
    std::string attrib3;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    DecodeDate(value_date, Y, M, D);

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id AND budgetpokaz_id = :budgetpokaz_id AND pokaz_id <> :budgetpokaz_id");
    q->SQL->Add("AND objectid = 0 AND vid_otc = :vid_otc AND value_date = :value_date");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("budgetpokaz_id")->AsInteger() = budgetpokaz_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();
    if (!q->EoF()) {
        value = q->FieldByName("value")->AsFloat();
    }
    //MAK q->Free();
    delete q;

    if (vid_otc == 5) {
        attrib3 = "CAvtoRascetMrpr{'Автом. расчет от меропр '} + DateTimeToStr(Now) + ' (ФКУ)'";
    }

    den_potok_id = TDB_DenegnPotok::IsItembyCAvtoMrpr(plan_id, budgetpokaz_id, cell_id_Mes_Budgpokaz, vid_otc,
                                                      value_date);
    if (den_potok_id == 0 && value != 0) {
        TDB_DenegnPotok::WriteItem("Авт'{reg_nomer}", 0, plan_id, budgetpokaz_id, cell_id_Mes_Budgpokaz, vid_otc,
                                   value_date, value, 0, "", "", "", attrib3, "", "", 0, "", "");
    } else {
        TDB_DenegnPotok::UpdateItem(den_potok_id, 0, value, "", "", attrib3);
    }
}

void TDB_DenegnPotok::SumByDogsForBudgPokaz_Write(int plan_id, int field_Finansir_cellid_Mes, int budgetpokaz_id,
                                                  int cell_id_Mes_Budgpokaz, int vid_otc, TDateTime value_date,
                                                  std::string KBK) {
    double value = 0;
    int den_potok_id;
    std::string attrib3;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value FROM Denegn_potok, Pokaztl");
    q->SQL->Add("WHERE plan_id = :plan_id AND objectid > 0 AND Denegn_potok.cell_id = :field_Finansir_cellid_Mes");
    q->SQL->Add("AND Pokaztl.pokaz_id = objectid AND Pokaztl.pokaz_bk_code = :KBK AND vid_otc = :vid_otc");
    q->SQL->Add("AND value_date = :value_date");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("field_Finansir_cellid_Mes")->AsInteger() = field_Finansir_cellid_Mes;
    q->ParamByName("KBK")->AsString() = KBK;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();
    if (!q->EoF()) {
        value = q->FieldByName("value")->AsFloat();
    }
    //MAK q->Free();
    delete q;

    if (vid_otc == 5) {
        attrib3 = "CAvtoRascet{'Автом. расчет от договоров '} + DateTimeToStr(Now) + '(' + GetUserLogin() + ')'";
    }

    den_potok_id = TDB_DenegnPotok::IsItembyCAvtoDgvr(plan_id, budgetpokaz_id, cell_id_Mes_Budgpokaz, vid_otc,
                                                      value_date);
    if (den_potok_id == 0 && value != 0) {
        TDB_DenegnPotok::WriteItem("Авт'{reg_nomer}", 0, plan_id, budgetpokaz_id, cell_id_Mes_Budgpokaz, vid_otc,
                                   value_date, value, 0, "", "", "", attrib3, "", "", 0, "", "");
    } else {
        TDB_DenegnPotok::UpdateItem(den_potok_id, 0, value, "", "", attrib3);
    }
}

void TDB_DenegnPotok::SumByDogsNoMRPRforBudgPokaz_Write(int plan_id, int field_Finansir_cellid_Mes, int budgetpokaz_id,
                                                        int cell_id_Mes_Budgpokaz, int vid_otc, TDateTime value_date,
                                                        std::string KBK) {
    double value = 0;
    int den_potok_id;
    std::string attrib3;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value FROM Denegn_potok DP, Pokaztl");
    q->SQL->Add("WHERE plan_id = :plan_id AND objectid > 0 AND DP.cell_id = :field_Finansir_cellid_Mes");
    q->SQL->Add("AND Pokaztl.pokaz_id = objectid AND Pokaztl.pokaz_bk_code = :KBK AND vid_otc = :vid_otc");
    q->SQL->Add("AND value_date = :value_date");
    q->SQL->Add(
            "AND NOT EXISTS (SELECT * FROM LinksObjects LO WHERE plan_id = :plan_id1 AND LO.objectid = DP.objectid)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_id1")->AsInteger() = plan_id;
    q->ParamByName("field_Finansir_cellid_Mes")->AsInteger() = field_Finansir_cellid_Mes;
    q->ParamByName("KBK")->AsString() = KBK;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();
    if (!q->EoF()) {
        value = q->FieldByName("value")->AsFloat();
    }
    //MAK q->Free();
    delete q; // Free the memory allocated for q

    if (vid_otc == 5) {
        attrib3 = "CAvtoRascet{'Автом. расчет от договоров '} + DateTimeToStr(Now) + '(' + GetUserLogin() + ')'";
    }

    den_potok_id = TDB_DenegnPotok::IsItembyCAvtoDgvr(plan_id, budgetpokaz_id, cell_id_Mes_Budgpokaz, vid_otc,
                                                      value_date);
    if (den_potok_id == 0 && value != 0) {
        TDB_DenegnPotok::WriteItem("Авт'{reg_nomer}", 0, plan_id, budgetpokaz_id, cell_id_Mes_Budgpokaz, vid_otc,
                                   value_date, value, 0, "", "", "", attrib3, "", "", 0, "", "");
    } else {
        TDB_DenegnPotok::UpdateItem(den_potok_id, 0, value, "", "", attrib3);
    }
}

bool TDB_DenegnPotok::isFromAisUBP(std::string reg_nomer) {
    bool Result = false;
    if (reg_nomer.length() < 4) return Result;
    // if not ((reg_nomer[1] in ['0'..'9']) and (reg_nomer[2] in ['0'..'9']) and (reg_nomer[3] = ':')) then exit;
    if (!((reg_nomer[0] >= '0' && reg_nomer[0] <= '9') && (reg_nomer[1] >= '0' && reg_nomer[1] <= '9')) ||
        (reg_nomer[2] == ':'))
        return Result; // 22 янв 2011
    Result = true;
    return Result;
}

void TDB_DenegnPotok::GetItemsPokazDate(TQueryUni *qIn, int plan_id, int pokaz_id, TDateTime s, TDateTime po) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT ");
    qIn->SQL->Add("den_potok_id,plan_id,pokaz_id,cell_id,vid_otc,value_date,budget_poluc_id,nom_documenta,");
    qIn->SQL->Add("value,coef,str_value,attrib1,attrib2,attrib3,attrib4,attrib5,objectid,reg_nomer,fromXML,");
    qIn->SQL->Add("nom_plategki,date_plategki,payer_id,budgetpokaz_id,id_FromPref,NBOneprisoed,BnkRS_PlatPoluc");
    qIn->SQL->Add("FROM Denegn_potok");
    qIn->SQL->Add("WHERE plan_id = :plan_id");
    qIn->SQL->Add("AND value_date BETWEEN :s AND :po");
    qIn->SQL->Add("AND vid_otc = :vid_otc");
    qIn->SQL->Add("AND pokaz_id = :pokaz_id");
    qIn->SQL->Add("ORDER BY value_date");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->ParamByName("s")->AsDateTime() = s;
    qIn->ParamByName("po")->AsDateTime() = po;
    qIn->ParamByName("vid_otc")->AsInteger() = 5;
    qIn->Open();
}

void
TDB_DenegnPotok::GetFilenameFiledateFromAttrib3(std::string attrib3, std::string &file_name, TDateTime &file_date) {
    std::string s;
    int n;
    s = attrib3.substr(14); // сначала с датой
    s = Trim(s);
    n = s.find(".xml");
    if (n != std::string::npos) {
        file_name = s.substr(0, n + 4);
        s = s.substr(n + 4);
        s = Trim(s);
        try {
            file_date = StrToDateTime(s.c_str());
        }
        catch (...) {
            file_date = Now();
        }
    } else {
        file_name = s;
        file_date = Now();
    }
}

bool TDB_DenegnPotok::doEvalforMrpr_forBpokaz(bool isBudgetpokaz_id, int budgetpokaz_id, int plan_id,
                                              int pokaz_meropr_id, int cell_id_Mes_Meropr, TDateTime value_date,
                                              int ist_fin_grad_id) {
    bool Result = true;
    int Year, Month, Day;
    double summaD;
    int kv;
    int cell_id_Mes_budgetpokaz;
    DecodeDate(value_date, Year, Month, Day);
    kv = GetNKv(Month);
    TDateTime Date1 = EncodeDate(Year, kv * 3 - 2, 1);
    TDateTime Date2 = GetLastDayDate(Year, kv, 0);

    // расчет квартального значения мероприятия
    summaD = TDB_DenegnPotok::GetSumKVforMeropr(plan_id, pokaz_meropr_id, cell_id_Mes_Meropr, Date1, Date2);
    // квартальное значение мероприятия сохраняем в БД
    bool aIsMsg = false;
    TUpdDBbyValue::UpdDBbyValue1(cell_id_Mes_Meropr - 1, plan_id, 5, Date2, summaD, 0, aIsMsg);

    if (isBudgetpokaz_id) {
        cell_id_Mes_budgetpokaz = TDB_Cell::GetCellId(budgetpokaz_id, 3, ist_fin_grad_id);
        // собирает из мероприятий и выводит для бюджетного показателя
        TDB_DenegnPotok::SumbyMeroprsForBudgPokaz_Write(plan_id, budgetpokaz_id, cell_id_Mes_budgetpokaz, 5,
                                                        value_date);

        // расчет квартального значения бюджетного показателя
        summaD = TDB_DenegnPotok::GetSumKVforMeropr(plan_id, budgetpokaz_id, cell_id_Mes_budgetpokaz, Date1, Date2);
        // квартальное значение бюджетного показателя сохраняем в БД
        bool aIsMsg = false;
        TUpdDBbyValue::UpdDBbyValue1(cell_id_Mes_budgetpokaz - 1, plan_id, 5, Date2, summaD, 0, aIsMsg);
    }
    return Result;
}

bool TDB_DenegnPotok::doEvalforDgvr_forMrpr_forBpokaz(bool isBudgetpokaz_id, string KBK, int plan_id, int objectid,
                                                      TDateTime value_date, int ist_fin_grad_id,
                                                      int &outBudgetpokaz_id) {
    bool Result = true;
    int class_Dog_id;
    int field_Finansir_id;
    int link_pokaz_id;
    int Year, Month, Day;
    int kv;
    TDateTime Date1, Date2;
    double summaD;
    int pokaz_meropr_id;
    int cell_id_Mes_Meropr;
    int budgetpokaz_id;
    int cell_id_Mes_budgetpokaz;
    string dogName;
    bool b;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    DecodeDate(value_date, Year, Month, Day);
    class_Dog_id = TDB_Pokaztl::GetClassId(objectid);
    if (class_Dog_id == 0)
        return false;
    if (!TIDsClassFieldCell::getClassFieldId(class_Dog_id, "Объем финансирования", Date(), true, field_Finansir_id))
        return false;
    TIDsClassFieldCell::getClassFieldId(class_Dog_id, "Мероприятие", Date(), false, link_pokaz_id);
    if (cell_id_FinansirMes[ist_fin_grad_id] == 0) {
        cell_id_FinansirMes[ist_fin_grad_id] = TDB_Cell::GetCellId(field_Finansir_id, 3, ist_fin_grad_id);
        if (cell_id_FinansirMes[ist_fin_grad_id] == 0) {
            ////throw Exception("Ошибка при вычислении Cell_id");
            return false;
        }
    }
    kv = GetNKv(Month);
    Date1 = EncodeDate(Year, kv * 3 - 2, 1);
    Date2 = GetLastDayDate(Year, kv, 0);
    summaD = TDB_DenegnPotok::GetSumZaPeriodforP_M_D(plan_id, field_Finansir_id, cell_id_FinansirMes[ist_fin_grad_id],
                                                     objectid, Date1, Date2);
    bool aIsMsg = false;
    TUpdDBbyValue::UpdDBbyValue1(cell_id_FinansirMes[ist_fin_grad_id] - 1, plan_id, 5, Date2, summaD, objectid, aIsMsg);
    pokaz_meropr_id = TDB_LinksObjects::GetdPokazByObject(plan_id, objectid, link_pokaz_id, dogName);
    cell_id_Mes_Meropr = TDB_Cell::GetCellId(pokaz_meropr_id, 3, ist_fin_grad_id);
    budgetpokaz_id = 0;
    if (isBudgetpokaz_id) {
        budgetpokaz_id = TDB_Pokaztl::GetPokazIdbyPokazBkCode(KBK, Year, CastNameTBSmets);
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        b = TDB_PlanStr::GetPlanStr1(q, plan_id, budgetpokaz_id);
        //MAK q->Free();
        delete q;
        if (!b) {
            //throw Exception("Для субъекта соответствующий бюджетный показатель отсутствует");
            return false;
        }
        cell_id_Mes_budgetpokaz = TDB_Cell::GetCellId(budgetpokaz_id, 3, ist_fin_grad_id);
        if (cell_id_Mes_budgetpokaz == 0) {
            //throw Exception("Ошибка при вычислении cell_id");
            return false;
        }
        if (budgetpokaz_id == pokaz_meropr_id)
            isBudgetpokaz_id = false;
    }
    if (pokaz_meropr_id > 0) {
        TDB_DenegnPotok::SumByDogsForMeropr_Write(plan_id, field_Finansir_id, pokaz_meropr_id, link_pokaz_id, 5,
                                                  value_date,
                                                  cell_id_Mes_Meropr, budgetpokaz_id);
        summaD = TDB_DenegnPotok::GetSumKVforMeropr(plan_id, pokaz_meropr_id, cell_id_Mes_Meropr, Date1, Date2);
        bool aIsMsg = false;
        TUpdDBbyValue::UpdDBbyValue1(cell_id_Mes_Meropr - 1, plan_id, 5, Date2, summaD, 0, aIsMsg);
        if (isBudgetpokaz_id)
            TDB_DenegnPotok::SumbyMeroprsForBudgPokaz_Write(plan_id, budgetpokaz_id, cell_id_Mes_budgetpokaz, 5,
                                                            value_date);
        if (isBudgetpokaz_id)
            TDB_DenegnPotok::SumByDogsNoMRPRforBudgPokaz_Write(plan_id, cell_id_FinansirMes[ist_fin_grad_id],
                                                               budgetpokaz_id,
                                                               cell_id_Mes_budgetpokaz, 5, value_date, KBK);
    } else {
        if (isBudgetpokaz_id)
            TDB_DenegnPotok::SumByDogsForBudgPokaz_Write(plan_id, cell_id_FinansirMes[ist_fin_grad_id], budgetpokaz_id,
                                                         cell_id_Mes_budgetpokaz, 5, value_date, KBK);
    }
    outBudgetpokaz_id = budgetpokaz_id;
    if (isBudgetpokaz_id) {
        summaD = TDB_DenegnPotok::GetSumKVforMeropr(plan_id, budgetpokaz_id, cell_id_Mes_budgetpokaz, Date1, Date2);
        TUpdDBbyValue::UpdDBbyValue1(cell_id_Mes_budgetpokaz - 1, plan_id, 5, Date2, summaD, 0, aIsMsg);
    }
    return Result;
}

double TDB_DenegnPotok::GetValueMonth(int objectid, int cell_id, int plan_id, int vid_otc, int aYear, int aMonth) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT sum(value) as value FROM Cell, Denegn_potok");
    q->SQL->Add("WHERE Cell.cell_id = :cell_id");
    q->SQL->Add("AND Denegn_potok.cell_id = Cell.cell_id");
    q->SQL->Add("AND Denegn_potok.pokaz_id = Cell.pokaz_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            q->SQL->Add("AND MONTH(value_date) = :Month");
            q->SQL->Add("AND YEAR(value_date) = :Year");
            break;
        }
        case TDBType::Postgre: {
            q->SQL->Add("AND date_part('month', value_date) = :Month");
            q->SQL->Add("AND date_part('year', value_date) = :Year");
            break;
        }
    }
    q->SQL->Add("AND Denegn_potok.objectid = :objectid");
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("Month")->AsInteger() = aMonth;
    q->ParamByName("Year")->AsInteger() = aYear;
    q->ParamByName("objectid")->AsInteger() = objectid;
    if (!q->EoF())
        Result = q->FieldByName("value")->AsFloat();
    //MAK q->Free();
    delete q;
    return Result;
}

int TDB_DenegnPotok::fromNeOdnoznPlategkaToDenegn_potokKBK(int neOdnozn_id, int plan_id, int budgetpokaz_id,
                                                           int cell_id_Mes_budgetpokaz) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM NeOdnoznPlategka");
    q->SQL->Add("WHERE neodnozn_id = :neOdnozn_id");
    q->ParamByName("neOdnozn_id")->AsInteger() = neOdnozn_id;
    q->Open();
    std::string NLimSprav = q->FieldByName("NLimSprav")->AsString();
    TDateTime DLimSprav = q->FieldByName("DLimSprav")->AsDateTime();
    std::string kodAU = q->FieldByName("kodAU")->AsString();
    double ValueSumma = q->FieldByName("ValueSumma")->AsFloat();
    std::string fromXML = q->FieldByName("fromXML")->AsString();
    TDateTime value_date = q->FieldByName("value_date")->AsDateTime();
    TDateTime file_date = q->FieldByName("file_date")->AsDateTime();
    std::string file_name = q->FieldByName("file_name")->AsString();
    std::string reg_nomer = q->FieldByName("reg_nomer")->AsString();
    std::string short_name = q->FieldByName("short_name")->AsString();
    int budget_poluc_id = q->FieldByName("budget_poluc_id")->AsInteger();
    std::string rasc_scet = q->FieldByName("rasc_scet")->AsString();
    std::string nomer_BO = q->FieldByName("nomer_BO")->AsString();
    std::string nom_plategki = q->FieldByName("nom_plategki")->AsString();
    TDateTime date_plategki = q->FieldByName("date_plategki")->AsDateTime();
    std::string KBKfromXML = q->FieldByName("KBKfromXML")->AsString();
    std::string NBOneprisoed = q->FieldByName("NBOneprisoed")->AsString();
    std::string BnkRS_PlatPoluc = q->FieldByName("BnkRS_PlatPoluc")->AsString();
    std::string nom_documenta = q->FieldByName("nom_documenta")->AsString();
    //MAK q->Free();
    delete q;

    if (TDB_DenegnPotok::IsItembyPlategkaPropertyGBU(plan_id, nom_plategki, date_plategki, value_date, ValueSumma) >
        0) {
        //--SystFuncs.MessageBoxExclamation('Данная платежка имеется среди ранее обработанных', MsgVnim);
        Result = 0;
        return Result;
    }

    int den_potok_id = TDB_DenegnPotok::WriteItemDO(0, plan_id, budgetpokaz_id, cell_id_Mes_budgetpokaz, 5, value_date,
                                                    ValueSumma,
                                                    0, NLimSprav, kodAU, file_name, KBKfromXML, fromXML, nom_plategki,
                                                    date_plategki,
                                                    nomer_BO, file_date, reg_nomer, nom_documenta);

    TDB_NeOdnoznPlategka::DelPlategka(neOdnozn_id);

    Result = den_potok_id;
    return Result;
}

int TDB_DenegnPotok::fromNeOdnoznPlategkaToDenegn_potokObject(int objectId, int neOdnozn_id, int plan_id,
                                                              int field_Finansir_id, int cell_id_FinansirMes) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM NeOdnoznPlategka");
    q->SQL->Add("WHERE neodnozn_id = :neOdnozn_id");
    q->ParamByName("neOdnozn_id")->AsInteger() = neOdnozn_id;
    q->Open();
    std::string NLimSprav = q->FieldByName("NLimSprav")->AsString();
    TDateTime DLimSprav = q->FieldByName("DLimSprav")->AsDateTime();
    std::string kodAU = q->FieldByName("kodAU")->AsString();
    double ValueSumma = q->FieldByName("ValueSumma")->AsFloat();
    std::string fromXML = q->FieldByName("fromXML")->AsString();
    TDateTime value_date = q->FieldByName("value_date")->AsDateTime();
    TDateTime file_date = q->FieldByName("file_date")->AsDateTime();
    std::string file_name = q->FieldByName("file_name")->AsString();
    std::string reg_nomer = q->FieldByName("reg_nomer")->AsString();
    std::string short_name = q->FieldByName("short_name")->AsString();
    int budget_poluc_id = q->FieldByName("budget_poluc_id")->AsInteger();
    std::string rasc_scet = q->FieldByName("rasc_scet")->AsString();
    std::string nomer_BO = q->FieldByName("nomer_BO")->AsString();
    std::string nom_plategki = q->FieldByName("nom_plategki")->AsString();
    TDateTime date_plategki = q->FieldByName("date_plategki")->AsDateTime();
    std::string KBKfromXML = q->FieldByName("KBKfromXML")->AsString();
    std::string NBOneprisoed = q->FieldByName("NBOneprisoed")->AsString();
    std::string BnkRS_PlatPoluc = q->FieldByName("BnkRS_PlatPoluc")->AsString();
    std::string nom_documenta = q->FieldByName("nom_documenta")->AsString();
    //MAK q->Free();
    delete q;

    if (TDB_DenegnPotok::IsItembyPlategkaPropertyGBU(plan_id, nom_plategki, date_plategki, value_date, ValueSumma) >
        0) {
        //--SystFuncs.MessageBoxExclamation('Данная платежка имеется среди ранее обработанных', MsgVnim);
        Result = 0;
        return Result;
    }

    int den_potok_id = TDB_DenegnPotok::WriteItemDO(objectId, plan_id, field_Finansir_id, cell_id_FinansirMes, 5,
                                                    value_date, ValueSumma,
                                                    0, NLimSprav, kodAU, file_name, KBKfromXML, fromXML, nom_plategki,
                                                    date_plategki,
                                                    nomer_BO, file_date, reg_nomer, nom_documenta);

    TDB_NeOdnoznPlategka::DelPlategka(neOdnozn_id);

    Result = den_potok_id;
    return Result;
}

int TDB_DenegnPotok::fromDenegn_potokToNeOdnoznPlategka(int den_potok_id, string rasc_scet) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int neOdnozn_id = 0;
    int objectid, plan_id, pokaz_id, cell_id, vid_otc, coef, budget_poluc_id;
    double value;
    string attrib4, fromXML, reg_nomer, nom_plategki, NBOneprisoed, attrib1, attrib2, attrib5, attrib3, BnkRS_PlatPoluc, nom_documenta, short_name, wINN;
    TDateTime value_date, date_plategki, file_date;
    q->SQL->Clear();
    TDB_DenegnPotok::GetItem(q, den_potok_id);
    objectid = q->FieldByName("objectid")->AsInteger();
    plan_id = q->FieldByName("plan_id")->AsInteger();
    pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
    cell_id = q->FieldByName("cell_id")->AsInteger();
    vid_otc = q->FieldByName("vid_otc")->AsInteger();
    value_date = q->FieldByName("value_date")->AsDateTime();
    value = q->FieldByName("value")->AsFloat();
    coef = q->FieldByName("coef")->AsInteger();
    attrib4 = q->FieldByName("attrib4")->AsString();
    fromXML = q->FieldByName("fromXML")->AsString();
    reg_nomer = q->FieldByName("reg_nomer")->AsString();
    nom_plategki = q->FieldByName("nom_plategki")->AsString();
    date_plategki = q->FieldByName("date_plategki")->AsDateTime();
    NBOneprisoed = q->FieldByName("NBOneprisoed")->AsString();
    attrib1 = q->FieldByName("attrib1")->AsString();
    attrib2 = q->FieldByName("attrib2")->AsString();
    attrib5 = q->FieldByName("attrib5")->AsString();
    attrib3 = q->FieldByName("attrib3")->AsString();
    try {
        file_date = StrToDateTime(attrib3);
    } catch (...) {
        file_date = 0;
    }
    BnkRS_PlatPoluc = q->FieldByName("BnkRS_PlatPoluc")->AsString();
    nom_documenta = q->FieldByName("nom_documenta")->AsString();
    short_name = q->FieldByName("short_name")->AsString();
    budget_poluc_id = q->FieldByName("budget_poluc_id")->AsInteger();
    //MAK q->Free();
    delete q;

    // проверить, нет ли такой платежки в NeOdnoznPlategka
    if (TDB_NeOdnoznPlategka::IsPlategka(attrib4, nom_plategki, value_date, value, rasc_scet) > 0) {
        //--SystFuncs.MessageBoxExclamation('Данная платежка имеется среди ранее обработанных', MsgVnim);
        return 0;
    }

    neOdnozn_id = TDB_NeOdnoznPlategka::SaveInNeOdnozn(attrib1, attrib2, value, fromXML, value_date, attrib5, file_date,
                                                       reg_nomer, short_name, budget_poluc_id, -2, rasc_scet, "",
                                                       nom_plategki, date_plategki, attrib4, NBOneprisoed,
                                                       BnkRS_PlatPoluc, nom_documenta);

    TDB_DenegnPotok::DeleteItem(den_potok_id);

    return neOdnozn_id;
}

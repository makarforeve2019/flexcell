#include "unDefDBconnect.h"
#include "DB_Enum.h"
#include "DB_BPoluc.h"
#include "DB_KeyOfObjects.h"
#include "DB_LinksObjects.h"
#include "DB_Cell.h"
#include "DB_Pokaztl.h"
//#include "unAPIDocum.h"

#include <string>
//#include "unDBType.h"
#include "DB_Common.h"
#include "DB_CellValue.h"
#include "../SystFuncs/SystFuncsLogic.h"

bool TDB_CellValue::IsValue(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT cell_id FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();
    if (!q->EoF()) {
        result = true;
    }
    delete q;
    return result;
}

bool TDB_CellValue::IsValue(int PokazId, int ComnpId, int VidPeriod, int God, int Kvrtl, int Mesjc, int Den, int PlanId, int VidOtch) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id FROM cnp_value");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id AND comnp_id = :comnp_id");
    q->SQL->Add("AND vid_period = :vid_period AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc AND god  = :god");
    q->SQL->Add("AND kvrtl = :kvrtl AND mesjc  = :mesjc");
    q->SQL->Add("AND den = :den");
    q->ParamByName("pokaz_id")->AsInteger() = PokazId;
    q->ParamByName("comnp_id")->AsInteger() = ComnpId;
    q->ParamByName("vid_period")->AsInteger() = VidPeriod;
    q->ParamByName("plan_id")->AsInteger() = PlanId;
    q->ParamByName("vid_otc")->AsInteger() = VidOtch;
    q->ParamByName("god")->AsInteger() = God;
    q->ParamByName("kvrtl")->AsInteger() = Kvrtl;
    q->ParamByName("mesjc")->AsInteger() = Mesjc;
    q->ParamByName("den")->AsInteger() = Den;
    q->Open();
    if (!q->EoF()) {
        result = true;
    }
    delete q;
    return result;
}

void TDB_CellValue::GetDataForObject(TQueryUni* qIn, int objectid, int plan_id, std::string spCellId) {
    qIn->SQL->Clear();
//    TQueryUni *qIn = new TQueryUni(DefDBconnect.Conn);
    qIn->SQL->Add("SELECT * FROM Cell_value");
    qIn->SQL->Add("WHERE objectid = :objectid");
    qIn->SQL->Add("AND plan_id = :plan_id");
    qIn->SQL->Add("AND cell_id in " + spCellId);
    qIn->ParamByName("objectid")->AsInteger() = objectid;
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->Open();
}

double TDB_CellValue::GetValue(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT value FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("value")->AsFloat();
    }
    delete q;
    return result;
}

double TDB_CellValue::GetValuePeriod(int objectid, int cell_id, int plan_id, int vid_otc, int aYear, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT value, value_date FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND vid_otc = :vid_otc AND DATEPART(YEAR, value_date) = :Year");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND vid_otc = :vid_otc AND date_part('year', value_date) = :Year");
            break;
    }
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("Year")->AsInteger() = aYear;
    q->Open();
    while (!q->EoF()) {
        if (q->FieldByName("value_date")->AsDateTime() > value_date)
            break;
        result += q->FieldByName("value")->AsFloat();
        q->Next();
    }
    delete q;
    return result;
}

std::string TDB_CellValue::GetStrValue(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT str_value FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("str_value")->AsString();
    }
    delete q;
    return result;
}

std::string TDB_CellValue::GetNI_byStr(TDateTime PokazDate, int objectid, int CurrCellId, int CurrPlanId, int CurrVidOtch, int CurrYear) {
    std::string s = "";
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT value_date, str_value FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :CurrCellId AND plan_id = :CurrPlanId");
    q->SQL->Add("AND vid_otc = :CurrVidOtch");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND DATEPART(YEAR, value_date) = :CurrYear");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', value_date) = :CurrYear");
            break;
    }
    q->SQL->Add("ORDER BY value_date");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("CurrCellId")->AsInteger() = CurrCellId;
    q->ParamByName("CurrPlanId")->AsInteger() = CurrPlanId;
    q->ParamByName("CurrVidOtch")->AsInteger() = CurrVidOtch;
    q->ParamByName("CurrYear")->AsInteger() = CurrYear;
    q->Open();
    while (!q->EoF()) {
        if (q->FieldByName("value_date")->AsDateTime() > PokazDate)
            break;
        if (s == "") {
            s = q->FieldByName("str_value")->AsString();
        } else {
            //!!s = s + #10#13 + q->FieldByName("str_value")->AsString();
            s = s + "\n" + q->FieldByName("str_value")->AsString();
        }
        q->Next();
    }
    delete q;
    return s;
}

void TDB_CellValue::SetValue(double value, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (IsValue(objectid, cell_id, plan_id, vid_otc, value_date)) {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Cell_value");
        q->SQL->Add("SET value = :value");
        q->SQL->Add("WHERE objectid = :objectid");
        q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("cell_id")->AsInteger() = cell_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("value_date")->AsDateTime() = value_date;
        q->ParamByName("value")->AsFloat() = value;
        q->ExecSQL();
    } else {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO Cell_value");
        q->SQL->Add("(value, objectid, cell_id, plan_id, vid_otc, value_date)");
        q->SQL->Add("VALUES (:value, :objectid, :cell_id, :plan_id, :vid_otc, :value_date)");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("cell_id")->AsInteger() = cell_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("value_date")->AsDateTime() = value_date;
        q->ParamByName("value")->AsFloat() = value;
        q->ExecSQL();
    }
    delete q;
}

void TDB_CellValue::SetValueNote(double value, std::string Note, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (IsValue(objectid, cell_id, plan_id, vid_otc, value_date)) {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Cell_value");
        q->SQL->Add("SET value = :value, Note = :Note");
        q->SQL->Add("WHERE objectid = :objectid");
        q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("cell_id")->AsInteger() = cell_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("value_date")->AsDateTime() = value_date;
        q->ParamByName("value")->AsFloat() = value;
        q->ParamByName("Note")->AsString() = Note;
        q->ExecSQL();
    } else {
        q->SQL->Clear();
         
        q->SQL->Add("INSERT INTO Cell_value");
        q->SQL->Add("(value, objectid, cell_id, plan_id, vid_otc, value_date, Note)");
        q->SQL->Add("VALUES (:value, :objectid, :cell_id, :plan_id, :vid_otc, :value_date, :Note)");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("cell_id")->AsInteger() = cell_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("value_date")->AsDateTime() = value_date;
        q->ParamByName("value")->AsFloat() = value;
        q->ParamByName("Note")->AsString() = Note;
        q->ExecSQL();
    }
    delete q;
}

void TDB_CellValue::SetValue2(double value, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date, int coef) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (IsValue(objectid, cell_id, plan_id, vid_otc, value_date)) {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Cell_value");
        q->SQL->Add("SET value = :value, coef = :coef");
        q->SQL->Add("WHERE objectid = :objectid");
        q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("cell_id")->AsInteger() = cell_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("value_date")->AsDateTime() = value_date;
        q->ParamByName("value")->AsFloat() = value;
        q->ParamByName("coef")->AsInteger() = coef;
        q->ExecSQL();
    } else {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO Cell_value");
        q->SQL->Add("(value, objectid, cell_id, plan_id, vid_otc, value_date, coef)");
        q->SQL->Add("VALUES (:value, :objectid, :cell_id, :plan_id, :vid_otc, :value_date, :coef)");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("cell_id")->AsInteger() = cell_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("value_date")->AsDateTime() = value_date;
        q->ParamByName("value")->AsFloat() = value;
        q->ParamByName("coef")->AsInteger() = coef;
        q->ExecSQL();
    }
    delete q;
}

bool TDB_CellValue::SetStrValue(PCellValueAtr vd, std::string str_value, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    bool result = false;
    int Year, Month, Day;
    DecodeDate(value_date, Year, Month, Day);
    if (vd->class_id_ForKey > 0 && vd->ObjectId > 0) {
        if (str_value != "" && TDB_KeyOfObjects::IsKeyObject(vd->class_id_ForKey, str_value, Year, vd->ObjectId)) {
            //throw Exception("В базе данных уже имеется объект с таким ключом");
        }
    }
    result = true;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (IsValue(objectid, cell_id, plan_id, vid_otc, value_date)) {
        if (str_value != "") {
            q->SQL->Clear();
            q->SQL->Add("UPDATE Cell_value");
            q->SQL->Add("SET str_value = :str_value");
            q->SQL->Add("WHERE objectid = :objectid");
            q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
            q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
            q->ParamByName("objectid")->AsInteger() = objectid;
            q->ParamByName("cell_id")->AsInteger() = cell_id;
            q->ParamByName("plan_id")->AsInteger() = plan_id;
            q->ParamByName("vid_otc")->AsInteger() = vid_otc;
            q->ParamByName("value_date")->AsDateTime() = value_date;
            q->ParamByName("str_value")->AsString() = str_value;
            q->ExecSQL();
        } else {
            DelOnlyValue(objectid, cell_id, plan_id, vid_otc, value_date);
        }
    } else {
        if (str_value != "") {
            q->SQL->Clear();
            q->SQL->Add("INSERT INTO Cell_value");
            q->SQL->Add("(str_value, objectid, cell_id, plan_id, vid_otc, value_date)");
            q->SQL->Add("VALUES (:str_value, :objectid, :cell_id, :plan_id, :vid_otc, :value_date)");
            q->ParamByName("objectid")->AsInteger() = objectid;
            q->ParamByName("cell_id")->AsInteger() = cell_id;
            q->ParamByName("plan_id")->AsInteger() = plan_id;
            q->ParamByName("vid_otc")->AsInteger() = vid_otc;
            q->ParamByName("value_date")->AsDateTime() = value_date;
            q->ParamByName("str_value")-> AsString() = str_value;
            q->ExecSQL();
        }
    }
    delete q;
    if (vd->class_id_ForKey > 0 && vd->ObjectId > 0) {
        if (str_value == "") {
            TDB_KeyOfObjects::DelKeyByIdObjectGod(vd->ObjectId, Year);
        } else {
            TDB_KeyOfObjects::SetKeyObject(vd->ObjectId, vd->class_id_ForKey, str_value, Year);
        }
    }
    return result;
}

void TDB_CellValue::SetStrValue2(std::string str_value, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (IsValue(objectid, cell_id, plan_id, vid_otc, value_date)) {
        if (str_value != "") {
            q->SQL->Clear();
            q->SQL->Add("UPDATE Cell_value");
            q->SQL->Add("SET str_value = :str_value");
            q->SQL->Add("WHERE objectid = :objectid");
            q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
            q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
            q->ParamByName("objectid")->AsInteger() = objectid;
            q->ParamByName("cell_id")->AsInteger() = cell_id;
            q->ParamByName("plan_id")->AsInteger() = plan_id;
            q->ParamByName("vid_otc")->AsInteger() = vid_otc;
            q->ParamByName("value_date")->AsDateTime() = value_date;
            q->ParamByName("str_value")-> AsString() = str_value;
            q->ExecSQL();
        } else {
            DelOnlyValue(objectid, cell_id, plan_id, vid_otc, value_date);
        }
    } else {
        if (str_value != "") {
            q->SQL->Clear();
            q->SQL->Add("INSERT INTO Cell_value");
            q->SQL->Add("(str_value, objectid, cell_id, plan_id, vid_otc, value_date)");
            q->SQL->Add("VALUES (:str_value, :objectid, :cell_id, :plan_id, :vid_otc, :value_date)");
            q->ParamByName("objectid")->AsInteger() = objectid;
            q->ParamByName("cell_id")->AsInteger() = cell_id;
            q->ParamByName("plan_id")->AsInteger() = plan_id;
            q->ParamByName("vid_otc")->AsInteger() = vid_otc;
            q->ParamByName("value_date")->AsDateTime() = value_date;
            q->ParamByName("str_value")-> AsString() = str_value;
            q->ExecSQL();
        }
    }
    delete q;
}

void TDB_CellValue::SetStrValue2note(std::string str_value, std::string Note, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (IsValue(objectid, cell_id, plan_id, vid_otc, value_date)) {
        if (str_value != "") {
            q->SQL->Clear();
             
            q->SQL->Add("UPDATE Cell_value");
            q->SQL->Add("SET str_value = :str_value, Note = :Note");
            q->SQL->Add("WHERE objectid = :objectid");
            q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
            q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
            q->ParamByName("objectid")->AsInteger() = objectid;
            q->ParamByName("cell_id")->AsInteger() = cell_id;
            q->ParamByName("plan_id")->AsInteger() = plan_id;
            q->ParamByName("vid_otc")->AsInteger() = vid_otc;
            q->ParamByName("value_date")->AsDateTime() = value_date;
            q->ParamByName("str_value")->AsString() = str_value;
            q->ParamByName("Note")->AsString() = Note;
            q->ExecSQL();
        } else {
            DelOnlyValue(objectid, cell_id, plan_id, vid_otc, value_date);
        }
    } else {
        if (str_value != "") {
            q->SQL->Clear();
             
            q->SQL->Add("INSERT INTO Cell_value");
            q->SQL->Add("(str_value, Note, objectid, cell_id, plan_id, vid_otc, value_date)");
            q->SQL->Add("VALUES (:str_value, :Note, :objectid, :cell_id, :plan_id, :vid_otc, :value_date)");
            q->ParamByName("objectid")->AsInteger() = objectid;
            q->ParamByName("cell_id")->AsInteger() = cell_id;
            q->ParamByName("plan_id")->AsInteger() = plan_id;
            q->ParamByName("vid_otc")->AsInteger() = vid_otc;
            q->ParamByName("value_date")->AsDateTime() = value_date;
            q->ParamByName("str_value")->AsString() = str_value;
            q->ParamByName("Note")->AsString() = Note;
            q->ExecSQL();
        }
    }
    delete q;
}

void TDB_CellValue::SetDcum_id(std::string docum_id, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (IsValue(objectid, cell_id, plan_id, vid_otc, value_date)) {
        q->SQL->Clear();
         
        q->SQL->Add("UPDATE Cell_value");
        q->SQL->Add("SET docum_id = :docum_id");
        q->SQL->Add("WHERE objectid = :objectid");
        q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("cell_id")->AsInteger() = cell_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("value_date")->AsDateTime() = value_date;
        q->ParamByName("docum_id")->AsString() = docum_id;
        q->ExecSQL();
    } else {
        q->SQL->Clear();
         
        q->SQL->Add("INSERT INTO Cell_value");
        q->SQL->Add("(docum_id, objectid, cell_id, plan_id, vid_otc, value_date)");
        q->SQL->Add("VALUES (:docum_id, :objectid, :cell_id, :plan_id, :vid_otc, :value_date)");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("cell_id")->AsInteger() = cell_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("value_date")->AsDateTime() = value_date;
        q->ParamByName("docum_id")->AsString() = docum_id;
        q->ExecSQL();
    }
    delete q;
}

std::string TDB_CellValue::GetDcum_id(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    q->SQL->Clear();
     
    q->SQL->Add("SELECT docum_id FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("docum_id")->AsString();
    }
    delete q;
    return result;
}

std::string TDB_CellValue::GetValueUslPost(PCellValueAtr vd, int& Id_Znac, TDateTime& ValueDate) {
    std::string result = "";
    if (vd->pokaz_type == 8) { // связь договора с мероприятием
        Id_Znac = TDB_LinksObjects::GetdPokazByObject(vd->PlanId, vd->ObjectId, vd->PokazId, result);
//        ValueDate = 0;
        return result;
    }
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT value, str_value, value_date FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("ORDER BY value_date DESC");
    q->ParamByName("objectid")->AsInteger() = vd->ObjectId;
    q->ParamByName("cell_id")->AsInteger() = vd->CellId;
    q->ParamByName("plan_id")->AsInteger() = vd->PlanId;
    q->ParamByName("vid_otc")->AsInteger() = vd->VidOtch;
    q->Open();
    if (!q->EoF()) {
        switch (vd->pokaz_type) {
            case 1:
                result = q->FieldByName("value")->AsString();
                break;
            case 2:
                result = q->FieldByName("str_value")->AsString();
                break;
            case 3:
                result = TDB_Enum::GetEnumName(q->FieldByName("value")->AsInteger()); // enum_id
                break;
            case 4:
                result = TDB_BPoluc::GetNameBP(q->FieldByName("value")->AsInteger());
                Id_Znac = q->FieldByName("value")->AsInteger();
                break;
            case 5:
                result = q->FieldByName("str_value")->AsString();
                break;
            case 7:
                Id_Znac = q->FieldByName("value")->AsInteger();
                result = TDB_Pokaztl::GetPokazName(Id_Znac);
                break;
        }
        ValueDate = q->FieldByName("value_date")->AsDateTime();
    } else {
        result = "";
        Id_Znac = 0;
//        ValueDate = 0;
    }
    delete q;
    return result;
}

std::string TDB_CellValue::GetValueUslPostBP(int objectid, int pokaz_id, int PlanId, int VidOtch, TDateTime ValueDate, int& Id_Znac) {
    std::string result = "";
    Id_Znac = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT value, str_value FROM Cell, Cell_value");
    q->SQL->Add("WHERE Cell.pokaz_id = :pokaz_id");
    q->SQL->Add("AND Cell.cell_period = 0");
    q->SQL->Add("AND Cell.ist_fin_id = 0");
    q->SQL->Add("AND Cell_value.objectid = :objectid");
    q->SQL->Add("AND Cell_value.cell_id = Cell.cell_id");
    q->SQL->Add("AND Cell_value.plan_id = :plan_id");
    q->SQL->Add("AND Cell_value.vid_otc = :vid_otc");
    //!!q->SQL->Add("AND value_date <= :value_date");
    q->SQL->Add("ORDER BY value_date DESC");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("plan_id")->AsInteger() = PlanId;
    q->ParamByName("vid_otc")->AsInteger() = VidOtch;
    //!!q->ParamByName("value_date")->AsDateTime() = ValueDate;
    q->Open();
    if (!q->EoF()) {
        result = TDB_BPoluc::GetNameBP(q->FieldByName("value")->AsInteger());
        Id_Znac = q->FieldByName("value")->AsInteger();
    }
    delete q;
    return result;
}

double TDB_CellValue::GetNumValueUslPost(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0;
    TDateTime ValueDate;
    q->SQL->Clear();
    q->SQL->Add("SELECT value, value_date FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("ORDER BY value_date DESC");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->Open();
    while (!q->EoF()) {
        ValueDate = q->FieldByName("value_date")->AsDateTime();
        if (ValueDate <= value_date) {
            result = q->FieldByName("value")->AsFloat();
            break;
        }
        q->Next();
    }
    delete q;
    return result;
}

std::string TDB_CellValue::GetStrValueUslPost(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    TDateTime ValueDate;
    q->SQL->Clear();
    q->SQL->Add("SELECT str_value, value_date FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("ORDER BY value_date DESC");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->Open();
    while (!q->EoF()) {
        ValueDate = q->FieldByName("value_date")->AsDateTime();
        if (ValueDate <= value_date) {
            result = q->FieldByName("str_value")->AsString();
            break;
        }
        q->Next();
    }
    delete q;
    return result;
}

bool TDB_CellValue::SetValueUslPost(std::string EditString, PCellValueAtr vd) {
    bool result = true;
    switch (vd->DataType) {
        case 1: TDB_CellValue::SetValue(PreobrValue(std::stof(EditString), vd->DataType, vd->coef_Forma, vd->coef_Pokaz), vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
                break;
        case 2: result = TDB_CellValue::SetStrValue(vd, EditString, vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
                break;
    }
    return result;
}

bool TDB_CellValue::IsSumma(int& vid_otc, int god, int pokaz_id, double& summa) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT vid_otc, SUM(value) as value");
    q->SQL->Add("FROM Cell, Cell_value");
    q->SQL->Add("WHERE Cell.pokaz_id = :pokaz_id");
    q->SQL->Add("AND Cell.cell_period = 2");
    q->SQL->Add("AND Cell.cell_is_eval = 0");
    q->SQL->Add("AND Cell_value.objectid = 0");
    q->SQL->Add("AND Cell_value.cell_id = Cell.cell_id");
    switch (GetDBType()) {
        case TDBType::MSSQL: q->SQL->Add("AND YEAR(value_date) = :god");
                    break;
        case TDBType::Postgre: q->SQL->Add("AND date_part('year', value_date) = :god");
                      break;
    }
    q->SQL->Add("GROUP BY vid_otc");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();
    while (!q->EoF()) {
        if (q->FieldByName("value")->AsFloat() > 0) {
            result = true;
            summa = q->FieldByName("value")->AsFloat();
            vid_otc = q->FieldByName("vid_otc")->AsInteger();
            break;
        }
        q->Next();
    }
    delete q;
    return result;
}

double TDB_CellValue::GetSummaUtPlNI(int plan_id, int pokaz_id, int ist_fin_id, int fmForm_CurrYear) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double summa = 0.0;
    TDateTime ToDate = Now();
    int Year, Month, Day;
    DecodeDate(ToDate, Year, Month, Day);
    if (fmForm_CurrYear < YearOf(ToDate)) {
        ToDate = EncodeDate(fmForm_CurrYear, 12, 31);
    }
    TDateTime dt = GetLastDayDate(Year, GetNKv(Month), 0);
    int cell_id = TDB_Cell::GetCellId(pokaz_id, 2, ist_fin_id);
    q->SQL->Clear();
    q->SQL->Add("SELECT value_date, value FROM Cell_value");
    q->SQL->Add("WHERE objectid = 0");
    q->SQL->Add("AND cell_id = :cell_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = 4");
    switch (GetDBType()) {
        case TDBType::MSSQL: q->SQL->Add("AND DATEPART(YEAR, value_date) = :god");
                    break;
        case TDBType::Postgre: q->SQL->Add("AND date_part('year', value_date) = :god");
                      break;
    }
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("god")->AsInteger() = Year;
    q->Open();
    while (!q->EoF()) {
        if (q->FieldByName("value_date")->AsDateTime() <= dt) {
            summa += q->FieldByName("value")->AsFloat();
        }
        q->Next();
    }
    delete q;
    return summa;
}

void TDB_CellValue::DelOnlyValue(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Cell_value ");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ExecSQL();
    delete q;
}

void TDB_CellValue::DelForClear(int pokaz_id, int vid_otc, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    if (GetisDocumConected()) {
        q->SQL->Add("SELECT docum_id FROM Cell_value");
        q->SQL->Add("WHERE cell_id in (SELECT cell_id FROM Cell WHERE pokaz_id = :pokaz_id)");
        switch (GetDBType()) {
            case TDBType::MSSQL: q->SQL->Add("AND DATEPART(YEAR, value_date) = :god");
                        break;
            case TDBType::Postgre: q->SQL->Add("AND date_part('year', value_date) = :god");
                          break;
        }
        q->SQL->Add("AND vid_otc = :vid_otc");
        q->SQL->Add("AND docum_id IS NOT NULL");
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("god")->AsInteger() = god;
        q->Open();
        TDB_CellValue::DeleteDocums(q);
    }
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Cell_value ");
    q->SQL->Add("WHERE cell_id in (SELECT cell_id FROM Cell WHERE pokaz_id = :pokaz_id)");
    switch (GetDBType()) {
        case TDBType::MSSQL: q->SQL->Add("AND DATEPART(YEAR, value_date) = :god");
                    break;
        case TDBType::Postgre: q->SQL->Add("AND date_part('year', value_date) = :god");
                      break;
    }
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("god")->AsInteger() = god;
    q->ExecSQL();
    delete q;
}

void TDB_CellValue::DeleteDocums(TQueryUni* q) {
    q->First();
    while (!q->EoF()) {
        std::string docum_id = q->FieldByName("docum_id")->AsString();
        //vernut TAPIDocum::DelDocum(docum_id);
        q->Next();
    }
}

void TDB_CellValue::SetNote(std::string Note, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Cell_value");
    q->SQL->Add("SET Note = :Note");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->ParamByName("Note")->AsString() = Note;
    q->ExecSQL();
    delete q;
}

std::string TDB_CellValue::GetNote(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";

    q->SQL->Add("SELECT note FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc AND value_date = :value_date");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = value_date;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("note")->AsString();
    }

    delete q;
    return result;
}

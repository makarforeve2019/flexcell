#include "DB_FormStrTMP.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_Cell.h"
#include "DB_FormStr.h"


void TDB_FormStrTMP::CreateForm_strTMP() {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);

    switch (GetDBType()) {
        case TDBType::MSSQL: {
            qTP->SQL->Add("CREATE TABLE #Form_str (str_id INT IDENTITY, form_id VARCHAR(20) NOT NULL, form_str_n SMALLINT NOT NULL,");
            qTP->SQL->Add("stmnt CHAR(20) NULL, attr_1 SMALLINT NULL, attr_2 SMALLINT NULL,");
            qTP->SQL->Add("attr_3 SMALLINT NULL, attr_4 SMALLINT NULL, is_ist_fin TINYINT NULL,");
            qTP->SQL->Add("ist_fin_grad_id INT NULL, note1 VARCHAR(2000) NULL, vid_otch TINYINT NULL,");
            qTP->SQL->Add("natur_vid INT NULL, note2 VARCHAR(3000) NULL, period TINYINT NULL,");
            qTP->SQL->Add("din_izm CHAR(50) NULL, is_group TINYINT NULL, align_vert TINYINT NULL,");
            qTP->SQL->Add("align_gor TINYINT NULL, font VARCHAR(50) NULL, h TINYINT NULL,");
            qTP->SQL->Add("is_bold TINYINT NULL, plan_id INT NULL, pokaz_id INT NULL,");
            qTP->SQL->Add("CoefPlus1 TINYINT NULL, ValuePeriod TINYINT NULL, readonly TINYINT NULL,");
            qTP->SQL->Add("is_hidden TINYINT NULL, is_set_god TINYINT NULL, god_plus SMALLINT NULL,");
            qTP->SQL->Add("god_ravno SMALLINT NULL, field_id INT NULL, is_sort TINYINT NULL,");
            qTP->SQL->Add("uroven_sort TINYINT NULL, is_svod TINYINT NULL, name_svod VARCHAR(200) NULL,");
            qTP->SQL->Add("is_filter TINYINT NULL, is_filter2 TINYINT NULL, ColOnFilter2 TINYINT NULL DEFAULT 0, is_Check TINYINT NULL, cell_id INT NULL,");
            qTP->SQL->Add("nValidator INT NULL, vid_uslovij TINYINT NULL, shablon VARCHAR(200) NULL, Msg VARCHAR(1000) NULL,");
            qTP->SQL->Add("value FLOAT NULL, valueB FLOAT NULL, str_value VARCHAR(500) NULL,");
            qTP->SQL->Add("value2 FLOAT NULL, valueB2 FLOAT NULL, str_value2 VARCHAR(500) NULL,");
            qTP->SQL->Add("value3 FLOAT NULL, valueB3 FLOAT NULL, str_value3 VARCHAR(500) NULL,");
            qTP->SQL->Add("value4 FLOAT NULL, valueB4 FLOAT NULL, str_value4 VARCHAR(500) NULL,");
            qTP->SQL->Add("value5 FLOAT NULL, valueB5 FLOAT NULL, str_value5 VARCHAR(500) NULL,");
            qTP->SQL->Add("value6 FLOAT NULL, valueB6 FLOAT NULL, str_value6 VARCHAR(500) NULL,");
            qTP->SQL->Add("value7 FLOAT NULL, valueB7 FLOAT NULL, str_value7 VARCHAR(500) NULL)");
            qTP->ExecSQL();
            qTP->SQL->Clear();
            qTP->SQL->Add("CREATE UNIQUE CLUSTERED INDEX XPKForm_str ON #Form_str (str_id)");
            qTP->ExecSQL();
            qTP->SQL->Clear();
            qTP->SQL->Add("CREATE UNIQUE INDEX XAK1Form_str ON #Form_str (form_id, form_str_n)");
            qTP->ExecSQL();
        }
        case TDBType::Postgre: {
            qTP->SQL->Add("CREATE TEMP TABLE Form_str_ (str_id INT IDENTITY, form_id VARCHAR(20) NOT NULL, form_str_n SMALLINT NOT NULL,");
            qTP->SQL->Add("stmnt CHAR(20) NULL, attr_1 SMALLINT NULL, attr_2 SMALLINT NULL,");
            qTP->SQL->Add("attr_3 SMALLINT NULL, attr_4 SMALLINT NULL, is_ist_fin SMALLINT NULL,");
            qTP->SQL->Add("ist_fin_grad_id INT NULL, note1 VARCHAR(2000) NULL, vid_otch SMALLINT NULL,");
            qTP->SQL->Add("natur_vid INT NULL, note2 VARCHAR(3000) NULL, period SMALLINT NULL,");
            qTP->SQL->Add("din_izm CHAR(50) NULL, is_group SMALLINT NULL, align_vert SMALLINT NULL,");
            qTP->SQL->Add("align_gor SMALLINT NULL, font VARCHAR(50) NULL, h SMALLINT NULL,");
            qTP->SQL->Add("is_bold SMALLINT NULL, plan_id INT NULL, pokaz_id INT NULL,");
            qTP->SQL->Add("CoefPlus1 SMALLINT NULL, ValuePeriod SMALLINT NULL, readonly SMALLINT NULL,");
            qTP->SQL->Add("is_hidden SMALLINT NULL, is_set_god SMALLINT NULL, god_plus SMALLINT NULL,");
            qTP->SQL->Add("god_ravno SMALLINT NULL, field_id INT NULL, is_sort SMALLINT NULL,");
            qTP->SQL->Add("uroven_sort SMALLINT NULL, is_svod SMALLINT NULL, name_svod VARCHAR(200) NULL,");
            qTP->SQL->Add("is_filter SMALLINT NULL, is_filter2 SMALLINT NULL, ColOnFilter2 SMALLINT NULL DEFAULT 0, is_Check SMALLINT NULL, cell_id INT NULL,");
            qTP->SQL->Add("nValidator INT NULL, vid_uslovij SMALLINT NULL, shablon VARCHAR(200) NULL, Msg VARCHAR(1000) NULL,");
            qTP->SQL->Add("value FLOAT NULL, valueB FLOAT NULL, str_value VARCHAR(500) NULL,");
            qTP->SQL->Add("value2 FLOAT NULL, valueB2 FLOAT NULL, str_value2 VARCHAR(500) NULL,");
            qTP->SQL->Add("value3 FLOAT NULL, valueB3 FLOAT NULL, str_value3 VARCHAR(500) NULL,");
            qTP->SQL->Add("value4 FLOAT NULL, valueB4 FLOAT NULL, str_value4 VARCHAR(500) NULL,");
            qTP->SQL->Add("value5 FLOAT NULL, valueB5 FLOAT NULL, str_value5 VARCHAR(500) NULL,");
            qTP->SQL->Add("value6 FLOAT NULL, valueB6 FLOAT NULL, str_value6 VARCHAR(500) NULL,");
            qTP->SQL->Add("value7 FLOAT NULL, valueB7 FLOAT NULL, str_value7 VARCHAR(500) NULL)");
            qTP->ExecSQL();
            qTP->SQL->Clear();
            qTP->SQL->Add("CREATE UNIQUE CLUSTERED INDEX XPKForm_str ON Form_str_ (str_id)");
            qTP->ExecSQL();
            qTP->SQL->Clear();
            qTP->SQL->Add("CREATE UNIQUE INDEX XAK1Form_str ON Form_str_ (form_id, form_str_n)");
            qTP->ExecSQL();
        }
    }
    delete qTP;
}

void TDB_FormStrTMP::DropForm_strTMP() {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);

    switch (GetDBType()) {
        case TDBType::MSSQL: qTP->SQL->Add("DROP TABLE #Form_str"); break;
        case TDBType::Postgre: qTP->SQL->Add("DROP TABLE Form_str_"); break;
    }
    qTP->ExecSQL();
    qTP->Free();
}

int TDB_FormStrTMP::GetMaxNum(int form_id) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    int result = 1500; // Default value

    qTP->SQL->Clear();
    qTP->SQL->Add("SELECT MAX(form_str_n) AS str_n ");
    switch (GetDBType()) {
        case TDBType::MSSQL: qTP->SQL->Add("FROM #Form_str"); break;
        case TDBType::Postgre: qTP->SQL->Add("FROM Form_str_"); break;
    }
    qTP->SQL->Add("WHERE form_id = :form_id ");
    qTP->SQL->Add("AND form_str_n > 1499 ");
    qTP->SQL->Add("AND form_str_n < :cMaxStrN ");
    qTP->ParamByName("form_id")->AsInteger() = form_id;
    qTP->ParamByName("cMaxStrN")->AsInteger() = cMaxStrN;
    qTP->Open();
    if (!qTP->FieldByName("str_n")->IsNull())
        result = qTP->FieldByName("str_n")->AsInteger();

    qTP->Free();
    return result;
}

void TDB_FormStrTMP::AddPokaz(int form_id, int &aForm_str_n, string note1, int pokaz_id, int plan_id,
                              int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec,
                              int noPrint, int is_filter, int period, int ist_fin_grad_id) {
    int form_str_n;
    int cell_id;
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    cell_id = TDB_Cell::GetCellId(pokaz_id, period, ist_fin_grad_id);
    form_str_n = TDB_FormStrTMP::GetMaxNum(form_id) + 1;

    aForm_str_n = form_str_n;
    qTP->SQL->Clear();
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("INSERT INTO #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("INSERT INTO Form_str_");
            break;
    }
    qTP->SQL->Add("(form_id,form_str_n,stmnt,note1,pokaz_id,plan_id,attr_3,attr_4,readonly,is_hidden,attr_2,is_group,is_filter,cell_id)");
    qTP->SQL->Add("VALUES (:form_id,:form_str_n,:Stmnt,:note1,:pokaz_id,:plan_id,:attr_3,:attr_4,:readonly,:is_hidden,:attr_2,:is_group,:is_filter,:cell_id)");
    qTP->ParamByName("form_id")->AsInteger() = form_id;
    qTP->ParamByName("form_str_n")->AsInteger() = form_str_n;
    qTP->ParamByName("Stmnt")->AsString() = "ПОКАЗАТЕЛЬ";
    qTP->ParamByName("note1")->AsString() = note1;
    qTP->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qTP->ParamByName("plan_id")->AsInteger() = plan_id;
    qTP->ParamByName("attr_3")->AsInteger() = Level_sign;
    qTP->ParamByName("attr_4")->AsInteger() = Is_open;
    qTP->ParamByName("readonly")->AsInteger() = readonly;
    qTP->ParamByName("is_hidden")->AsInteger() = is_hidden;
    qTP->ParamByName("attr_2")->AsInteger() = nStolbec;
    qTP->ParamByName("is_group")->AsInteger() = noPrint;
    if (is_filter < 0) {
        //        qTP->ParamByName("is_filter")->DataType() = ftInteger;
        //        qTP->ParamByName("is_filter")->Clear();
    }
    else {
        qTP->ParamByName("is_filter")->AsInteger() = is_filter;
    }
    qTP->ParamByName("cell_id")->AsInteger() = cell_id;
    qTP->ExecSQL();
    qTP->Free();
}

void TDB_FormStrTMP::GetAllPokaztl(TQueryUni *qIn, int form_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
   switch (GetDBType()) {
        case TDBType::MSSQL:
            qIn->SQL->Add("FROM #Form_str");
            break;
        case TDBType::Postgre:
            qIn->SQL->Add("FROM Form_str_");
            break;
    }
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND Stmnt = :Stmnt");
    qIn->SQL->Add("ORDER BY form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("Stmnt")->AsString() = "ПОКАЗАТЕЛЬ";
    qIn->Open();
}

void TDB_FormStrTMP::AddProizStr(int form_id, int& aForm_str_n, string note1, int Level_sign, int nStolbec) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    int form_str_n = TDB_FormStrTMP::GetMaxNum(form_id) + 1;
    aForm_str_n = form_str_n;
    qTP->SQL->Clear();
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("INSERT INTO #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("INSERT INTO Form_str_");
            break;
    }
    qTP->SQL->Add("(form_id, form_str_n, stmnt, note1, attr_3, attr_2)");
    qTP->SQL->Add("values (:form_id, :form_str_n, :Stmnt, :note1, :attr_3, :attr_2)");
    qTP->ParamByName("form_id")->AsInteger() = form_id;
    qTP->ParamByName("form_str_n")->AsInteger() = form_str_n;
    qTP->ParamByName("Stmnt")->AsString() = "ПРОИЗВСТР";
    qTP->ParamByName("note1")->AsString() = note1;
    qTP->ParamByName("attr_3")->AsInteger() = Level_sign;
    qTP->ParamByName("attr_2")->AsInteger() = nStolbec;
    qTP->ExecSQL();
    qTP->Free();
}

void TDB_FormStrTMP::DelStr(int str_id) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    qTP->SQL->Clear();
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("DELETE FROM #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("DELETE FROM Form_str_");
            break;
    }
    qTP->SQL->Add("WHERE str_id = " + std::to_string(str_id));
    qTP->ExecSQL();
    qTP->Free();
}

void TDB_FormStrTMP::SetValue(int str_id, double value) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    qTP->SQL->Clear();
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("UPDATE #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("UPDATE Form_str_");
            break;
    }
    qTP->SQL->Add("SET value = :value");
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    qTP->ParamByName("value")->AsFloat() = value;
    qTP->ExecSQL();
    qTP->Free();
}

void TDB_FormStrTMP::SetValueB(int str_id, double valueB) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    qTP->SQL->Clear();
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("UPDATE #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("UPDATE Form_str_");
            break;
    }
    qTP->SQL->Add("SET valueB = :valueB");
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    qTP->ParamByName("valueB")->AsFloat() = valueB;
    qTP->ExecSQL();
    qTP->Free();
}

void TDB_FormStrTMP::SetValue_Iparu(int str_id, int i, double value, double valueB) {
    if (!(-i == 0 || -i == 6)) {
        //throw Exception("Номер пары точек в TDB_FormStrTMP.SetValue_Iparu неверно. Описание формы неверно!");
    }
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    qTP->SQL->Clear();
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("UPDATE #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("UPDATE Form_str_");
            break;
    }
    switch (i) {
        case 0:
            qTP->SQL->Add("SET value = :value, valueB = :valueB");
            break;
        case -1:
            qTP->SQL->Add("SET value2 = :value2, valueB2 = :valueB2");
            break;
        case -2:
            qTP->SQL->Add("SET value3 = :value3, valueB3 = :valueB3");
            break;
        case -3:
            qTP->SQL->Add("SET value4 = :value4, valueB4 = :valueB4");
            break;
        case -4:
            qTP->SQL->Add("SET value5 = :value5, valueB5 = :valueB5");
            break;
        case -5:
            qTP->SQL->Add("SET value6 = :value6, valueB6 = :valueB6");
            break;
        case -6:
            qTP->SQL->Add("SET value7 = :value7, valueB7 = :valueB7");
            break;
    }
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    switch (i) {
        case 0:
            qTP->ParamByName("value")->AsFloat() = value;
            qTP->ParamByName("valueB")->AsFloat() = valueB;
            break;
        case -1:
            qTP->ParamByName("value2")->AsFloat() = value;
            qTP->ParamByName("valueB2")->AsFloat() = valueB;
            break;
        case -2:
            qTP->ParamByName("value3")->AsFloat() = value;
            qTP->ParamByName("valueB3")->AsFloat() = valueB;
            break;
        case -3:
            qTP->ParamByName("value4")->AsFloat() = value;
            qTP->ParamByName("valueB4")->AsFloat() = valueB;
            break;
        case -4:
            qTP->ParamByName("value5")->AsFloat() = value;
            qTP->ParamByName("valueB5")->AsFloat() = valueB;
            break;
        case -5:
            qTP->ParamByName("value6")->AsFloat() = value;
            qTP->ParamByName("valueB6")->AsFloat() = valueB;
            break;
        case -6:
            qTP->ParamByName("value7")->AsFloat() = value;
            qTP->ParamByName("valueB7")->AsFloat() = valueB;
            break;
    }
    qTP->ExecSQL();
    qTP->Free();
}

double TDB_FormStrTMP::GetValue(int str_id) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    qTP->SQL->Add("SELECT value");
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("FROM #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("FROM Form_str_");
            break;
    }
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    qTP->Open();
    double result = qTP->FieldByName("value")->AsFloat();
    qTP->Free();
    return result;
}

double TDB_FormStrTMP::GetValueB(int str_id) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    qTP->SQL->Add("SELECT valueB");
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("FROM #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("FROM Form_str_");
            break;
    }
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    qTP->Open();
    double result = qTP->FieldByName("valueB")->AsFloat();
    qTP->Free();
    return result;
}

void TDB_FormStrTMP::GetValue_Iparu(int str_id, int i, double& value, double& valueB) {
    if (!(i == 0 || i == -6)) {
        //throw Exception("Номер пары точек в TDB_FormStrTMP.GetValue_Iparu неверно. Описание формы неверно!");
    }
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    switch (i) {
        case 0:
            qTP->SQL->Add("SELECT value, valueB");
            break;
        case -1:
            qTP->SQL->Add("SELECT value2, valueB2");
            break;
        case -2:
            qTP->SQL->Add("SELECT value3, valueB3");
            break;
        case -3:
            qTP->SQL->Add("SELECT value4, valueB4");
            break;
        case -4:
            qTP->SQL->Add("SELECT value5, valueB5");
            break;
        case -5:
            qTP->SQL->Add("SELECT value6, valueB6");
            break;
        case -6:
            qTP->SQL->Add("SELECT value7, valueB7");
            break;
    }
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("FROM #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("FROM Form_str_");
            break;
    }
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    qTP->Open();
    switch (i) {
        case 0:
            value = qTP->FieldByName("value")->AsFloat();
            valueB = qTP->FieldByName("valueB")->AsFloat();
            break;
        case -1:
            value = qTP->FieldByName("value2")->AsFloat();
            valueB = qTP->FieldByName("valueB2")->AsFloat();
            break;
        case -2:
            value = qTP->FieldByName("value3")->AsFloat();
            valueB = qTP->FieldByName("valueB3")->AsFloat();
            break;
        case -3:
            value = qTP->FieldByName("value4")->AsFloat();
            valueB = qTP->FieldByName("valueB4")->AsFloat();
            break;
        case -4:
            value = qTP->FieldByName("value5")->AsFloat();
            valueB = qTP->FieldByName("valueB5")->AsFloat();
            break;
        case -5:
            value = qTP->FieldByName("value6")->AsFloat();
            valueB = qTP->FieldByName("valueB6")->AsFloat();
            break;
        case -6:
            value = qTP->FieldByName("value7")->AsFloat();
            valueB = qTP->FieldByName("valueB7")->AsFloat();
            break;
    }
    qTP->Free();
}

void TDB_FormStrTMP::SetStrValue(int str_id, std::string str_value)
{
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    switch (GetDBType())
    {
        case TDBType::MSSQL: qTP->SQL->Add("UPDATE #Form_str"); break;
        case TDBType::Postgre: qTP->SQL->Add("UPDATE Form_str_"); break;
    }
    qTP->SQL->Add("SET str_value = :str_value");
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    qTP->ParamByName("str_value")->AsString() = str_value;
    qTP->ExecSQL();
    qTP->Free();
}

std::string TDB_FormStrTMP::GetStrValue(int str_id) {
    std::string result;
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    qTP->SQL->Add("SELECT str_value");
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("FROM #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("FROM Form_str_");
            break;
    }
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    qTP->Open();
    result = qTP->FieldByName("str_value")->AsString().c_str();
    qTP->Free();
    return result;
}

void TDB_FormStrTMP::UpdNote1(int str_id, std::string note1) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("UPDATE #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("UPDATE Form_str_");
            break;
    }
    qTP->SQL->Add("SET note1 = :note1");
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    qTP->ParamByName("note1")->AsString() = note1;
    qTP->ExecSQL();
    qTP->Free();
}

void TDB_FormStrTMP::UpdForm_str_n(int form_id, int str_id) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("UPDATE #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("UPDATE Form_str_");
            break;
    }
    qTP->SQL->Add("SET form_str_n = :form_str_n");
    qTP->SQL->Add("WHERE str_id = :str_id");
    qTP->ParamByName("str_id")->AsInteger() = str_id;
    qTP->ParamByName("form_str_n")->AsInteger() = TDB_FormStrTMP::GetMaxNum(form_id) + 1;
    qTP->ExecSQL();
    qTP->Free();
}

void TDB_FormStrTMP::GetItemsN1N2(TQueryUni *qIn, int form_id, int N1, int N2) {
    qIn->SQL->Clear();

    qIn->SQL->Add("SELECT *");
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qIn->SQL->Add("FROM #Form_str");
            break;
        case TDBType::Postgre:
            qIn->SQL->Add("FROM Form_str_");
            break;
    }
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND form_str_n BETWEEN :N1 AND :N2");
    qIn->SQL->Add("ORDER BY form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("N1")->AsInteger() = N1;
    qIn->ParamByName("N2")->AsInteger() = N2;
    qIn->Open();
}

void TDB_FormStrTMP::GetItemsAll(TQueryUni *qIn, int form_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
     switch (GetDBType()) {
        case TDBType::MSSQL:
            qIn->SQL->Add("FROM #Form_str");
            break;
        case TDBType::Postgre:
            qIn->SQL->Add("FROM Form_str_");
            break;
    }
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("ORDER BY form_id, form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->Open();
}

void TDB_FormStrTMP::SetFont(int form_id, int form_str_n, std::string font, int h, int is_bold) {
    TQueryUni *qTP = new TQueryUni(DefDBconnect.Conn);
    switch (GetDBType()) {
        case TDBType::MSSQL:
            qTP->SQL->Add("UPDATE #Form_str");
            break;
        case TDBType::Postgre:
            qTP->SQL->Add("UPDATE Form_str_");
            break;
    }
    qTP->SQL->Add("SET font = :font, h = :h, is_bold = :is_bold");
    qTP->SQL->Add("WHERE form_id = :form_id");
    qTP->SQL->Add("AND form_str_n = :form_str_n");
    qTP->ParamByName("form_id")->AsInteger() = form_id;
    qTP->ParamByName("form_str_n")->AsInteger() = form_str_n;
    qTP->ParamByName("font")->AsString() = font.c_str();
    qTP->ParamByName("h")->AsInteger() = h;
    qTP->ParamByName("is_bold")->AsInteger() = is_bold;
    qTP->ExecSQL();
    qTP->Free();
}

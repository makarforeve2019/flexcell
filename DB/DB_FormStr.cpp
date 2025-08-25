#include "DB_FormStr.h"
#include "unDefDBconnect.h"
#include "dmGlobalVars.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include <../Form/FormTypes.h>
#include <string>


void TDB_FormStr::initSqlText() {
    GlobalVars->quFormStr_ColW->SQL->Clear();
    GlobalVars->quFormStr_ColW->SQL->Add("SELECT form_str_n FROM Form_str");
    GlobalVars->quFormStr_ColW->SQL->Add("WHERE (form_id = :CurFormId) AND (stmnt = :stmntWidth)");
    GlobalVars->quFormStr_ColW->SQL->Add("ORDER BY form_str_n");

    GlobalVars->quFormStrWidth->SQL->Clear();
    GlobalVars->quFormStrWidth->SQL->Add("SELECT form_id, stmnt, form_str_n, attr_1, attr_2 FROM Form_str");
    GlobalVars->quFormStrWidth->SQL->Add("WHERE (form_id = :CurrFormId) AND (stmnt = :stmntWidth) AND");
    GlobalVars->quFormStrWidth->SQL->Add("      (attr_1 = :CurrColNum)");

    GlobalVars->quFormStrText->SQL->Clear();
    GlobalVars->quFormStrText->SQL->Add("SELECT *");
    GlobalVars->quFormStrText->SQL->Add("FROM Form_str");
    GlobalVars->quFormStrText->SQL->Add("WHERE (form_id = :CurrFormId) AND (stmnt = :stmntText)");

    GlobalVars->quFormStrColDest->SQL->Clear();
    GlobalVars->quFormStrColDest->SQL->Add("SELECT * FROM Form_str");
    GlobalVars->quFormStrColDest->SQL->Add("WHERE (form_id = :CurrFormId)  AND (attr_1 = :CurrColNum)");
    GlobalVars->quFormStrColDest->SQL->Add("AND form_str_n >= 1000");
}




std::string TDB_FormStr::GetNote1(int form_id, int form_str_n) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";

    q->SQL->Clear();
    q->SQL->Add("SELECT note1 FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("note1")->AsString();
    }

   //MAK q->Free();
    delete q;
    return result;
}

void TDB_FormStr::AddRow(int form_id, int& aForm_str_n, std::string Stmnt, int attr_1, int attr_2, int attr_3, int attr_4,
    int is_ist_fin, int ist_fin_grad_id, std::string note1, int vid_otch, int natur_vid, std::string note2, int period,
    std::string din_izm, int is_group, int align_vert, int align_gor, std::string font, int h, int is_bold, int plan_id,
    int pokaz_id, int CoefPlus1, int ValuePeriod, int readonly, int is_hidden, int is_set_god, int god_plus, int god_ravno,
    int field_id, int is_sort, int uroven_sort, int is_svod, std::string name_svod, int is_filter, int is_Check, int is_filter2,
    int ColOnFilter2, int nValidator, int vid_uslovij, double value1, double value2, std::string shablon, std::string Msg) {

    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    form_str_n = GetMaxNum(form_id) + 1;
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, attr_1, attr_2, attr_3, attr_4, is_ist_fin, ist_fin_grad_id,"
        "note1, vid_otch, natur_vid, note2, period, din_izm, is_group, align_vert, align_gor, font, h,"
        "is_bold, plan_id, pokaz_id, CoefPlus1, ValuePeriod, readonly,"
        "is_hidden, is_set_god, god_plus, god_ravno, field_id,"
        "is_sort, uroven_sort, is_svod, name_svod, is_filter, is_Check, is_filter2, ColOnFilter2,"
        "nValidator, vid_uslovij, value1, value2, shablon, Msg)");
    q->SQL->Add("values (:form_id, :form_str_n, :stmnt, :attr_1, :attr_2, :attr_3, :attr_4, :is_ist_fin, :ist_fin_grad_id,"
        ":note1, :vid_otch, :natur_vid, :note2, :period, :din_izm, :is_group, :align_vert, :align_gor, :font, :h,"
        ":is_bold, :plan_id, :pokaz_id, :CoefPlus1, :ValuePeriod, :readonly,"
        ":is_hidden, :is_set_god, :god_plus, :god_ravno, :field_id,"
        ":is_sort, :uroven_sort, :is_svod, :name_svod, :is_filter, :is_Check, :is_filter2, :ColOnFilter2,"
        ":nValidator, :vid_uslovij, :value1, :value2, :shablon, :Msg)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("stmnt")->AsString() = Stmnt;
    q->ParamByName("attr_1")->AsInteger() = attr_1;
    q->ParamByName("attr_2")->AsInteger() = attr_2;
    q->ParamByName("attr_3")->AsInteger() = attr_3;
    q->ParamByName("attr_4")->AsInteger() = attr_4;
    q->ParamByName("is_ist_fin")->AsInteger() = is_ist_fin;
    q->ParamByName("ist_fin_grad_id")->AsInteger() = ist_fin_grad_id;
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("vid_otch")->AsInteger() = vid_otch;
    q->ParamByName("natur_vid")->AsInteger() = natur_vid;
    q->ParamByName("note2")->AsString() = note2;
    q->ParamByName("period")->AsInteger() = period;
    q->ParamByName("din_izm")->AsString() = din_izm;
    q->ParamByName("is_group")->AsInteger() = is_group;
    q->ParamByName("align_vert")->AsInteger() = align_vert;
    q->ParamByName("align_gor")->AsInteger() = align_gor;
    q->ParamByName("font")->AsString() = font;
    q->ParamByName("h")->AsInteger() = h;
    q->ParamByName("is_bold")->AsInteger() = is_bold;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("CoefPlus1")->AsInteger() = CoefPlus1;
    q->ParamByName("ValuePeriod")->AsInteger() = ValuePeriod;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("is_set_god")->AsInteger() = is_set_god;
    q->ParamByName("god_plus")->AsInteger() = god_plus;
    q->ParamByName("god_ravno")->AsInteger() = god_ravno;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ParamByName("is_sort")->AsInteger() = is_sort;
    q->ParamByName("uroven_sort")->AsInteger() = uroven_sort;
    q->ParamByName("is_svod")->AsInteger() = is_svod;
    q->ParamByName("name_svod")->AsString() = name_svod;
    q->ParamByName("is_filter")->AsInteger() = is_filter;
    q->ParamByName("is_Check")->AsInteger() = is_Check;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ParamByName("nValidator")->AsInteger() = nValidator;
    q->ParamByName("vid_uslovij")->AsInteger() = vid_uslovij;
    q->ParamByName("value1")->AsFloat() = value1;
    q->ParamByName("value2")->AsFloat() = value2;
    q->ParamByName("shablon")->AsString() = shablon;
    q->ParamByName("Msg")->AsString() = Msg;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

int TDB_FormStr::AddRow1(int form_id, int form_str_n) {
    int wform_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    wform_str_n = GetMaxNum(form_id) + 1;
    int result = wform_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str ");
    q->SQL->Add("(form_id, form_str_n, stmnt, attr_1, attr_2, attr_3, attr_4, is_ist_fin, ist_fin_grad_id,"
        "note1, vid_otch, natur_vid, note2, period, din_izm, is_group, align_vert, align_gor, font, h,"
        "is_bold, plan_id, pokaz_id, CoefPlus1, ValuePeriod, readonly,"
        "is_hidden, is_set_god, god_plus, god_ravno, field_id,"
        "is_sort, uroven_sort, is_svod, name_svod, is_filter, is_Check, is_filter2, ColOnFilter2,"
        "nValidator, vid_uslovij, value1, value2, shablon, Msg)");
    q->SQL->Add("SELECT :form_id, :wform_str_n, stmnt, attr_1, attr_2, attr_3, attr_4, is_ist_fin, ist_fin_grad_id,"
        "note1, vid_otch, natur_vid, note2, period, din_izm, is_group, align_vert, align_gor, font, h,"
        "is_bold, plan_id, pokaz_id, CoefPlus1, ValuePeriod, readonly,"
        "is_hidden, is_set_god, god_plus, god_ravno, field_id,"
        "is_sort, uroven_sort, is_svod, name_svod, is_filter, is_Check, is_filter2, ColOnFilter2,"
        "nValidator, vid_uslovij, value1, value2, shablon, Msg");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id1");
    q->SQL->Add("AND form_str_n = :form_str_n");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("wform_str_n")->AsInteger() = wform_str_n;
    q->ParamByName("form_id1")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
    return result;
}

int TDB_FormStr::AddRowFromOtherForm(int form_id, int form_idOther, int form_str_nOther, bool isNewstr) {
    int wform_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (form_str_nOther >= 1 && form_str_nOther < cColWidthNum)
        wform_str_n = TDB_FormStr::GetMaxNum0(form_id) + 1;
    else if (form_str_nOther >= cColWidthNum && form_str_nOther < 1000)
        wform_str_n = TDB_FormStr::GetMaxNum1(form_id) + 1;
    else if (form_str_nOther >= 1500 && form_str_nOther < cMaxStrN)
        wform_str_n = TDB_FormStr::GetMaxNum(form_id) + 1;

    int result = wform_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str ");
    q->SQL->Add("(form_id, form_str_n, stmnt, attr_1, attr_2, attr_3, attr_4, is_ist_fin, ist_fin_grad_id,"
        "note1, vid_otch, natur_vid, note2, period, din_izm, is_group, align_vert, align_gor, font, h,"
        "is_bold, plan_id, pokaz_id, CoefPlus1, ValuePeriod, readonly,"
        "is_hidden, is_set_god, god_plus, god_ravno, field_id,"
        "is_sort, uroven_sort, is_svod, name_svod, is_filter, is_Check, is_filter2, ColOnFilter2,"
        "nValidator, vid_uslovij, value1, value2, shablon, Msg)");
    q->SQL->Add("SELECT :form_id, :wform_str_n, stmnt, attr_1, attr_2, attr_3, attr_4, is_ist_fin, ist_fin_grad_id,"
        "note1, vid_otch, natur_vid, note2, period, din_izm, is_group, align_vert, align_gor, font, h,"
        "is_bold, plan_id, pokaz_id, CoefPlus1, ValuePeriod, readonly,"
        "is_hidden, is_set_god, god_plus, god_ravno, field_id,"
        "is_sort, uroven_sort, is_svod, name_svod, is_filter, is_Check, is_filter2, ColOnFilter2,"
        "nValidator, vid_uslovij, value1, value2, shablon, Msg");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_idOther");
    q->SQL->Add("AND form_str_n = :form_str_nOther");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("wform_str_n")->AsInteger() = isNewstr ? wform_str_n : form_str_nOther;
    q->ParamByName("form_idOther")->AsInteger() = form_idOther;
    q->ParamByName("form_str_nOther")->AsInteger() = form_str_nOther;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
    return result;
}

void TDB_FormStr::AddPokaz(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n, std::string note1, int pokaz_id, int plan_id,
    int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
    int is_filter, int is_filter2, int ColOnFilter2, int Filter2Variant) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str ");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, pokaz_id, plan_id, attr_3, attr_4, readonly, is_hidden, attr_2, is_group, ");
    q->SQL->Add("is_filter, is_filter2, ColOnFilter2, isorientation)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :pokaz_id, :plan_id, :attr_3, :attr_4, :readonly, :is_hidden, :attr_2, ");
    q->SQL->Add(":is_group, :is_filter, :is_filter2, :ColOnFilter2, :Filter2Variant)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ПОКАЗАТЕЛЬ";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_4")->AsInteger() = Is_open;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_group")->AsInteger() = noPrint;

    if (is_filter < 0) {
//vernut        q->ParamByName("is_filter")->DataType() = ftInteger;
//        q->ParamByName("is_filter")->Clear();
    }
    else {
        q->ParamByName("is_filter")->AsInteger() = is_filter;
    }
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ParamByName("Filter2Variant")->AsInteger() = Filter2Variant;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddKorteg(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n, std::string note1, int pokaz_id,
    std::string note2, int plan_id, int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
    int is_filter, int is_filter2, int ColOnFilter2) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str ");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, pokaz_id, note2, plan_id, attr_3, attr_4, readonly, is_hidden, attr_2, is_group, ");
    q->SQL->Add("is_filter, is_filter2, ColOnFilter2)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :pokaz_id, :note2, :plan_id, :attr_3, :attr_4, :readonly, :is_hidden, :attr_2, ");
    q->SQL->Add(":is_group, :is_filter, :is_filter2, :ColOnFilter2)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "КОРТЕЖ";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("note2")->AsString() = note2;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_4")->AsInteger() = Is_open;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_group")->AsInteger() = noPrint;

    if (is_filter < 0) {
//vernut        q->ParamByName("is_filter")->DataType() = ftInteger;
//vernut        q->ParamByName("is_filter")->Clear();
    }
    else {
        q->ParamByName("is_filter")->AsInteger() = is_filter;
    }
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddProizStr(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n,
                   std::string note1, int Level_sign, int nStolbec, int is_hidden) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, attr_3, attr_2, is_hidden)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :attr_3, :attr_2, :is_hidden)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ПРОИЗВСТР";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddColForNamePokaz(int form_id, int ColForNamePokaz) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    form_str_n = ColForNamePokaz + 1000;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, attr_1)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :attr_1)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "НАИМЕНОВАНИЕ";
    q->ParamByName("attr_1")->AsInteger() = ColForNamePokaz;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddFormF1(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n, std::string note1, std::string note2, int Level_sign,
     int nStolbec, int is_hidden, int is_filter2, int ColOnFilter2, int plan_id, int is_svod) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, note2, attr_3, attr_2, is_hidden, is_filter2, ColOnFilter2, plan_id, is_svod)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :note2, :attr_3, :attr_2, :is_hidden, :is_filter2, :ColOnFilter2, :plan_id, :is_svod)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ФОРМУЛАF1";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("note2")->AsString() = note2;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("is_svod")->AsInteger() = is_svod;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddGroupRowsInfo(int form_id, std::string JSONGroupRows) {
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note2)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note2)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = groupRowsForm_str_n;
    q->ParamByName("Stmnt")->AsString() = stmntGroupRows;
    q->ParamByName("note2")->AsString() = JSONGroupRows;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddZamena(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n, std::string note1, std::string note2, int Level_sign,
                   int nStolbec, int is_hidden, int is_filter2, int ColOnFilter2) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, note2, attr_3, attr_2, is_hidden, is_filter2, ColOnFilter2)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :note2, :attr_3, :attr_2, :is_hidden, :is_filter2, :ColOnFilter2)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ЗАМЕНА";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("note2")->AsString() = note2;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddPrisvoit(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n, std::string note1, std::string note2, int Level_sign,
             int nStolbec, int is_hidden, int toCol, int plan_id, bool aIsDoOnlyByCellUpd, int isRow) {
    
    int form_str_n, IsDoOnlyByCellUpd;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    IsDoOnlyByCellUpd = aIsDoOnlyByCellUpd ? 1 : 0;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, note2, attr_3, attr_2, is_hidden, attr_4, plan_id, is_group, isGroupSelect)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :note2, :attr_3, :attr_2, :is_hidden, :attr_4, :plan_id, :IsDoOnlyByCellUpd, :isRow)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ПРИСВОИТЬ";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("note2")->AsString() = note2;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_4")->AsInteger() = toCol;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("IsDoOnlyByCellUpd")->AsInteger() = IsDoOnlyByCellUpd;
    q->ParamByName("isRow")->AsInteger() = isRow;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddPologit(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n, std::string note1, std::string note2, int Level_sign,
                   int nStolbec, int is_hidden, int toCol, int plan_id) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, note2, attr_3, attr_2, is_hidden, attr_4, plan_id)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :note2, :attr_3, :attr_2, :is_hidden, :attr_4, :plan_id)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ПОЛОЖИТЬ";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("note2")->AsString() = note2;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_4")->AsInteger() = toCol;
    q->ParamByName("plan_id")->AsInteger() = plan_id;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddBlock(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n,
  std::string note1, int field_id, int is_filter2, int ColOnFilter2, int plan_id) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, field_id, is_filter2, ColOnFilter2, plan_id)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :field_id, :is_filter2, :ColOnFilter2, :plan_id)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "БЛОК";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ParamByName("plan_id")->AsInteger() = plan_id;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddDialogBlock(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n,
  std::string note1, int field_id, int is_filter2, int ColOnFilter2) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, field_id, is_filter2, ColOnFilter2)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :field_id, :is_filter2, :ColOnFilter2)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ДИАЛОГБЛОК";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddSsilkaNaStr(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n,
               std::string note1, int Level_sign, int nStolbec) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, attr_3, attr_2)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :attr_3, :attr_2)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ССЫЛКАНАСТР";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddAgregat(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n, std::string note1, int pokaz_id, int plan_id,
                   int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                   int field_id, int is_svod, int is_filter2, int ColOnFilter2) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, pokaz_id, plan_id, attr_3, attr_4, readonly, is_hidden, attr_2, is_group, field_id, is_svod, ");
    q->SQL->Add("is_filter2, ColOnFilter2)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :pokaz_id, :plan_id, :attr_3, :attr_4, :readonly, :is_hidden, :attr_2, :is_group, :field_id, :is_svod, ");
    q->SQL->Add(":is_filter2, :ColOnFilter2)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "АГРЕГАТ";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_4")->AsInteger() = Is_open;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_group")->AsInteger() = noPrint;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ParamByName("is_svod")->AsInteger() = is_svod;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddUslovie(bool isInsert, int posleFormStrN, int form_id, int &aForm_str_n, std::string note1, int plan_id,
                   int Level_sign, int readonly, int is_hidden, int nStolbec, int prefToCol) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, plan_id, attr_3, readonly, is_hidden, attr_2, attr_4)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :Stmnt, :note1, :plan_id, :attr_3, :readonly, :is_hidden, :attr_2, :attr_4)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "УСЛОВИЕ";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("attr_4")->AsInteger() = prefToCol;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddButton(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string& note1, std::string& note2,
                            int nCol, int h, std::string& msg) {
    
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    }
    else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, note2, attr_1, h, msg)");
    q->SQL->Add("VALUES (:form_id, :form_str_n, :stnmt, :note1, :note2, :attr_1, :h, :msg)");

    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("stnmt")->AsString() = "КНОПКА";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("note2")->AsString() = note2;
    q->ParamByName("attr_1")->AsInteger() = nCol;
    q->ParamByName("h")->AsInteger() = h;
    q->ParamByName("msg")->AsString() = msg;

    q->ExecSQL();
   //MAK q->Free();
delete q; // Free the memory allocated for q
}

void TDB_FormStr::AddObjects1n(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, int pokaz_id, int plan_id,
                                int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint, int is_filter, int is_filter2,
                                int ColOnFilter2) {
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    } else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, pokaz_id, plan_id, attr_3, attr_4, readonly, is_hidden, attr_2, is_group, "
                "is_filter, is_filter2, ColOnFilter2)");
    q->SQL->Add("values (:form_id, :form_str_n, :Stmnt, :note1, :pokaz_id, :plan_id, :attr_3, :attr_4, :readonly, :is_hidden, :attr_2, "
                ":is_group, :is_filter, :is_filter2, :ColOnFilter2)");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ОбъектыПоСвязи1:N";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_4")->AsInteger() = Is_open;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_group")->AsInteger() = noPrint;
    if (is_filter < 0) {
//vernut          q->ParamByName("is_filter")->DataType() = ftInteger;
//vernut          q->ParamByName("is_filter")->Clear();
    } else {
        q->ParamByName("is_filter")->AsInteger() = is_filter;
    }
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_FormStr::AddStyleOforml(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n,
                                 std::string note1, std::string note2)
                                 {
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    } else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, note2)");
    q->SQL->Add("values (:form_id, :form_str_n, :Stmnt, :note1, :note2)");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "Стиль оформления";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("note2")->AsString() = note2;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_FormStr::AddSummaPatternKBK(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, std::string note2, int Level_sign,
                   int nStolbec, int is_hidden, int is_filter, std::string shablon, int plan_id, int pokaz_id,
                   int is_filter2, int ColOnFilter2) {
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (isInsert) {
        form_str_n = posleFormStrN + 1;
    } else {
        form_str_n = GetMaxNum(form_id) + 1;
    }
    aForm_str_n = form_str_n;
    
    q->SQL->Clear();
     
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note1, note2, attr_3, attr_2, is_hidden, is_filter, shablon, plan_id, pokaz_id, ");
    q->SQL->Add("is_filter2, ColOnFilter2)");
    q->SQL->Add("values (:form_id, :form_str_n, :Stmnt, :note1, :note2, :attr_3, :attr_2, :is_hidden, :is_filter, :shablon, :plan_id, :pokaz_id, ");
    q->SQL->Add(":is_filter2, :ColOnFilter2)");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "СУММАшаблонКБК";
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("note2")->AsString() = note2;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("is_filter")->AsInteger() = is_filter;
    q->ParamByName("shablon")->AsString() = shablon;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}


void TDB_FormStr::UpdPokaz(int form_id, int form_str_n, std::string note1, int pokaz_id, int plan_id,
                   int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                   int is_filter, int is_filter2, int ColOnFilter2, int Filter2Variant) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, pokaz_id = :pokaz_id, plan_id = :plan_id, attr_3 = :attr_3, attr_4 = :attr_4, readonly = :readonly, ");
    q->SQL->Add("is_hidden = :is_hidden, attr_2 = :attr_2, is_group = :is_group, is_filter = :is_filter, is_filter2 = :is_filter2, ");
    q->SQL->Add("ColOnFilter2 = :ColOnFilter2");
    q->SQL->Add(", isorientation = :Filter2Variant");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_4")->AsInteger() = Is_open;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_group")->AsInteger() = noPrint;
    if (is_filter < 0) {
//vernut          q->ParamByName("is_filter")->DataType() = ftInteger;
//vernut          q->ParamByName("is_filter")->Clear();
    } else {
        q->ParamByName("is_filter")->AsInteger() = is_filter;
    }
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ParamByName("Filter2Variant")->AsInteger() = Filter2Variant;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdKorteg(int form_id, int form_str_n, std::string note1, int pokaz_id, std::string note2, int plan_id,
                   int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                   int is_filter, int is_filter2, int ColOnFilter2) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, pokaz_id = :pokaz_id, note2 = :Note2, plan_id = :plan_id, attr_3 = :attr_3, attr_4 = :attr_4, ");
    q->SQL->Add("readonly = :readonly, is_hidden = :is_hidden, attr_2 = :attr_2, is_group = :is_group, is_filter = :is_filter, ");
    q->SQL->Add("is_filter2 = :is_filter2, ColOnFilter2 = :ColOnFilter2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("Note2")->AsString() = note2;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_4")->AsInteger() = Is_open;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_group")->AsInteger() = noPrint;
    if (is_filter < 0) {
//vernut          q->ParamByName("is_filter")->DataType() = ftInteger;
//vernut          q->ParamByName("is_filter")->Clear();
    } else {
        q->ParamByName("is_filter")->AsInteger() = is_filter;
    }
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdUslovie(int form_id, int form_str_n, std::string note1, int plan_id,
                   int Level_sign, int readonly, int is_hidden, int nStolbec, int prefToCol) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = (:Note1), plan_id = (:plan_id), attr_3 = (:attr_3), ");
    q->SQL->Add("readonly = (:readonly), is_hidden = (:is_hidden), attr_2 = (:attr_2), attr_4 = (:attr_4)");
    q->SQL->Add("WHERE form_id = (:form_id)");
    q->SQL->Add("AND form_str_n = (:form_str_n)");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("attr_4")->AsInteger() = prefToCol;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdSummaPatternKBK(int form_id, int form_str_n, std::string note1, std::string note2, int Level_sign,
                  int nStolbec, int is_hidden, int is_filter, std::string shablon, int plan_id, int pokaz_id,
                  int is_filter2, int ColOnFilter2) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = (:Note1), note2 = (:Note2), attr_3 = (:attr_3), attr_2 = (:attr_2), is_hidden = (:is_hidden), is_filter = (:is_filter), ");
    q->SQL->Add("shablon = (:shablon), plan_id = (:plan_id), pokaz_id = (:pokaz_id), is_filter2 = (:is_filter2), ColOnFilter2 = (:ColOnFilter2)");
    q->SQL->Add("WHERE form_id = (:form_id)");
    q->SQL->Add("AND form_str_n = (:form_str_n)");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("Note2")->AsString() = note2;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("is_filter")->AsInteger() = is_filter;
    q->ParamByName("shablon")->AsString() = shablon;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdBlock(int form_id, int form_str_n, std::string note1, int field_id,
    int is_filter2, int ColOnFilter2, int plan_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
     
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, field_id = :field_id, is_filter2 = :is_filter2, ColOnFilter2 = :ColOnFilter2, plan_id = :plan_id");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdDialogBlock(int form_id, int form_str_n, std::string note1, int field_id,
    int is_filter2, int ColOnFilter2)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
     
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, field_id = :field_id, is_filter2 = :is_filter2, ColOnFilter2 = :ColOnFilter2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdStyleOforml(int form_id, int form_str_n, const std::string note2){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
     
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note2 = :note2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("note2")->AsString() = note2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdStmnt(int form_id, int form_str_n, const std::string Stmnt){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET stmnt = :stmnt");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("stmnt")->AsString() = Stmnt;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdNote2(int form_id, int form_str_n, const std::string note2){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note2 = :note2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("note2")->AsString() = note2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

std::string TDB_FormStr::GetNote2(int form_id, int form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result = "";
    q->SQL->Clear();
 
    q->SQL->Add("SELECT note2");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) Result = q->FieldByName("note2")->AsString();
   //MAK q->Free();
delete q;
    return Result;
}

void TDB_FormStr::UpdFilter2(int form_id, int form_str_n, int is_filter2, int ColOnFilter2)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET is_filter2 = :is_filter2, ColOnFilter2 = :ColOnFilter2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::GetFilter2(int form_id, int form_str_n, int& is_filter2, int& ColOnFilter2)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
 
    q->SQL->Add("SELECT is_filter2, ColOnFilter2");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        is_filter2 = q->FieldByName("is_filter2")->AsInteger();
        ColOnFilter2 = q->FieldByName("ColOnFilter2")->AsInteger();
    } else {
        is_filter2 = 0;
        ColOnFilter2 = 0;
    }
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdPokazUstGod(int form_id, int form_str_n, int is_set_god, int god_plus, int god_ravno)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (is_set_god >= 0) {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Form_str");
        q->SQL->Add("SET is_set_god = :is_set_god, god_plus = :god_plus, god_ravno = :god_ravno");
        q->SQL->Add("WHERE form_id = :form_id");
        q->SQL->Add("AND form_str_n = :form_str_n");
        q->ParamByName("form_id")->AsInteger() = form_id;
        q->ParamByName("form_str_n")->AsInteger() = form_str_n;
        q->ParamByName("is_set_god")->AsInteger() = is_set_god;
        q->ParamByName("god_plus")->AsInteger() = god_plus;
        q->ParamByName("god_ravno")->AsInteger() = god_ravno;
        q->ExecSQL();
    } else {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Form_str");
        q->SQL->Add("SET is_set_god = NULL, god_plus = NULL, god_ravno = NULL");
        q->SQL->Add("WHERE form_id = :form_id");
        q->SQL->Add("AND form_str_n = :form_str_n");
        q->ParamByName("form_id")->AsInteger() = form_id;
        q->ParamByName("form_str_n")->AsInteger() = form_str_n;
        q->ExecSQL();
    }
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdPokazUstPeriod(int form_id, int form_str_n, int period, int ValuePeriod)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (period >= 0) {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Form_str");
        q->SQL->Add("SET period = :period, ValuePeriod = :ValuePeriod");
        q->SQL->Add("WHERE form_id = :form_id");
        q->SQL->Add("AND form_str_n = :form_str_n");
        q->ParamByName("form_id")->AsInteger() = form_id;
        q->ParamByName("form_str_n")->AsInteger() = form_str_n;
        q->ParamByName("period")->AsInteger() = period;
        q->ParamByName("ValuePeriod")->AsInteger() = ValuePeriod;
        q->ExecSQL();
    } else {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Form_str");
        q->SQL->Add("SET period = NULL, ValuePeriod = NULL");
        q->SQL->Add("WHERE form_id = :form_id");
        q->SQL->Add("AND form_str_n = :form_str_n");
        q->ParamByName("form_id")->AsInteger() = form_id;
        q->ParamByName("form_str_n")->AsInteger() = form_str_n;
        q->ExecSQL();
    }
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdIstFinGradId(int form_id, int form_str_n, int is_ist_fin, int ist_fin_grad_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET is_ist_fin = :is_ist_fin, ist_fin_grad_id = :ist_fin_grad_id");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("is_ist_fin")->AsInteger() = is_ist_fin;
    q->ParamByName("ist_fin_grad_id")->AsInteger() = ist_fin_grad_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdTocnost(int form_id, int form_str_n, int tocnost)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET tocnost = :tocnost");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("tocnost")->AsInteger() = tocnost;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::AddAlignmentObjedinit(int form_id, int &aForm_str_n, int is_group, int align_vert, int align_gor, std::string font, int h, int is_bold)
{
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    form_str_n = GetMaxNum1(form_id);
    if (form_str_n > 0)
        form_str_n += 1;
    else
        form_str_n = cColWidthNum;
    aForm_str_n = form_str_n;
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, is_group, align_vert, align_gor, font, h, is_bold)");
    q->SQL->Add("values (:form_id, :form_str_n, :Stmnt, :is_group, :align_vert, :align_gor, :font, :h, :is_bold)");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "AlignmentObjedinit";
    q->ParamByName("is_group")->AsInteger() = is_group;
    q->ParamByName("align_vert")->AsInteger() = align_vert;
    q->ParamByName("align_gor")->AsInteger() = align_gor;
    q->ParamByName("font")->AsString() = font;
    q->ParamByName("h")->AsInteger() = h;
    q->ParamByName("is_bold")->AsInteger() = is_bold;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::AddOdinakovZnac(int form_id, int &aForm_str_n, std::string note2)
{
    int form_str_n;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    form_str_n = GetMaxNum1(form_id);
    if (form_str_n > 0)
        form_str_n += 1;
    else
        form_str_n = cColWidthNum;
    aForm_str_n = form_str_n;
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt, note2)");
    q->SQL->Add("values (:form_id, :form_str_n, :Stmnt, :note2)");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "OdinakovZnac";
    q->ParamByName("note2")->AsString() = note2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdProizStr(int form_id, int form_str_n, std::string note1, int Level_sign, int nStolbec, int is_hidden)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, attr_3 = :attr_3, attr_2 = :attr_2, is_hidden = :is_hidden");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdFormF1(int form_id, int form_str_n, std::string note1, std::string note2, int Level_sign,
    int nStolbec, int is_hidden, int is_filter2, int ColOnFilter2, int plan_id, int is_svod)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, note2 = :Note2, attr_3 = :attr_3, attr_2 = :attr_2, is_hidden = :is_hidden, ");
    q->SQL->Add("is_filter2 = :is_filter2, ColOnFilter2 = :ColOnFilter2, plan_id = :plan_id, is_svod = :is_svod");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("Note2")->AsString() = note2;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("is_svod")->AsInteger() = is_svod;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdZamena(int form_id, int form_str_n, std::string note1, std::string note2)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, note2 = :Note2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("Note2")->AsString() = note2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdPrisvoit(int form_id, int form_str_n, std::string note1, std::string note2, int Level_sign,
    int nStolbec, int is_hidden, int toCol, int plan_id, bool aIsDoOnlyByCellUpd, int isRow)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int IsDoOnlyByCellUpd;
    if (aIsDoOnlyByCellUpd == true){
        IsDoOnlyByCellUpd = 1;
    } else IsDoOnlyByCellUpd = 0;
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, note2 = :Note2, attr_3 = :attr_3, attr_2 = :attr_2, is_hidden = :is_hidden, ");
    q->SQL->Add("attr_4 = :attr_4, plan_id = :plan_id, is_group = :IsDoOnlyByCellUpd, isGroupSelect = :isRow");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("Note2")->AsString() = note2;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_4")->AsInteger() = toCol;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("IsDoOnlyByCellUpd")->AsInteger() = IsDoOnlyByCellUpd;
    q->ParamByName("isRow")->AsInteger() = isRow;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdSsilkaNaStr(int form_id, int form_str_n, std::string note1, int Level_sign, int nStolbec)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, attr_3 = :attr_3, attr_2 = :attr_2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdAgregat(int form_id, int form_str_n, std::string note1, int pokaz_id, int plan_id,
    int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
    int field_id, int is_svod, int is_filter2, int ColOnFilter2)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :Note1, pokaz_id = :pokaz_id, plan_id = :plan_id, attr_3 = :attr_3, ");
    q->SQL->Add("attr_4 = :attr_4, readonly = :readonly, is_hidden = :is_hidden, attr_2 = :attr_2, is_group = :is_group, field_id = :field_id, ");
    q->SQL->Add("is_svod = :is_svod, is_filter2 = :is_filter2, ColOnFilter2 = :ColOnFilter2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Note1")->AsString() = note1;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("attr_3")->AsInteger() = Level_sign;
    q->ParamByName("attr_4")->AsInteger() = Is_open;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("is_hidden")->AsInteger() = is_hidden;
    q->ParamByName("attr_2")->AsInteger() = nStolbec;
    q->ParamByName("is_group")->AsInteger() = noPrint;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ParamByName("is_svod")->AsInteger() = is_svod;
    q->ParamByName("is_filter2")->AsInteger() = is_filter2;
    q->ParamByName("ColOnFilter2")->AsInteger() = ColOnFilter2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdButton(int form_id, int form_str_n, std::string& note1, std::string& note2,
                            int nCol, int h, std::string& msg)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note1 = :note1, note2 = :note2, attr_1 = :attr_1, h = :h, msg = :msg");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("note1")->AsString() = note1;
    q->ParamByName("note2")->AsString() = note2;
    q->ParamByName("attr_1")->AsInteger() = nCol;
    q->ParamByName("h")->AsInteger() = h;
    q->ParamByName("msg")->AsString() = msg;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::DelObjAtrs(int form_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE ");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND stmnt = 'ХАРАКТЕРИСТИКА'");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::DelAllItems(int form_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE ");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::DelItemsN1N2(int form_id, int N1, int N2)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE ");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n BETWEEN :N1 AND :N2");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("N1")->AsInteger() = N1;
    q->ParamByName("N2")->AsInteger() = N2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_FormStr::GetMaxNum0(int form_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(form_str_n) as str_n");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n >= 1");
    //--q->SQL->Add("AND form_str_n < 500");
    q->SQL->Add("AND form_str_n < :cColWidthNum");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("cColWidthNum")->AsInteger() = cColWidthNum;
    q->Open();
    if (q->FieldByName("str_n")->AsInteger() == 0)
        result = 0;
    else
        result = q->FieldByName("str_n")->AsInteger();
    if (result == (cColWidthNum - 1))
    {
        q->Close();
        q->SQL->Clear();
        q->SQL->Add("SELECT MAX(form_str_n) as str_n");
        q->SQL->Add("FROM Form_str");
        q->SQL->Add("WHERE form_id = :form_id");
        q->SQL->Add("AND form_str_n >= :cCellContentNumNextBeg");
        q->SQL->Add("AND form_str_n < :cCellContentNumNextEnd");
        q->ParamByName("form_id")->AsInteger() = form_id;
        q->ParamByName("cCellContentNumNextBeg")->AsInteger() = cCellContentNumNextBeg;
        q->ParamByName("cCellContentNumNextEnd")->AsInteger() = cCellContentNumNextEnd;
        q->Open();
        if (q->FieldByName("str_n")->AsInteger() == 0)
            result = cCellContentNumNextBeg;
        else
            result = q->FieldByName("str_n")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_FormStr::GetMaxNum1(int form_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(form_str_n) as str_n");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    //--q->SQL->Add("AND form_str_n >= 500");
    q->SQL->Add("AND form_str_n >= :cColWidthNum");
    q->SQL->Add("AND form_str_n < 1000");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("cColWidthNum")->AsInteger() = cColWidthNum;
    q->Open();
    if (q->FieldByName("str_n")->AsInteger() == 0)
        result = 0;
    else
        result = q->FieldByName("str_n")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_FormStr::GetMaxNum(int form_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(form_str_n) as str_n");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n > 1499");
    q->SQL->Add("AND form_str_n < :cMaxStrN");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("cMaxStrN")->AsInteger() = cMaxStrN;
    q->Open();
    if (q->FieldByName("str_n")->AsInteger() == 0)
        result = 1500;
    else
        result = q->FieldByName("str_n")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::GetItemsN1N2(TQueryUni *qIn, int form_id, int N1, int N2)
{
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Form_str");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND form_str_n BETWEEN :N1 AND :N2");
    qIn->SQL->Add("ORDER BY form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("N1")->AsInteger() = N1;
    qIn->ParamByName("N2")->AsInteger() = N2;
    qIn->Open();
}

void TDB_FormStr::GetItemsN1N2WithFieldName(TQueryUni *qIn, int form_id, int N1, int N2)
{
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT Form_str.*, Pokaztl.pokaz_name as field_name");
    qIn->SQL->Add("FROM Form_str LEFT JOIN Pokaztl ON Pokaztl.pokaz_id = Form_str.field_id");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND form_str_n BETWEEN :N1 AND :N2");
    qIn->SQL->Add("ORDER BY form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("N1")->AsInteger() = N1;
    qIn->ParamByName("N2")->AsInteger() = N2;
    qIn->Open();
}

void TDB_FormStr::GetItemsN1N2_N3N4(TQueryUni *qIn, int form_id, int N1, int N2, int N3, int N4)
{
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Form_str");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND ((form_str_n BETWEEN :N1 AND :N2) or (form_str_n BETWEEN :N3 AND :N4))");
    qIn->SQL->Add("ORDER BY form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("N1")->AsInteger() = N1;
    qIn->ParamByName("N2")->AsInteger() = N2;
    qIn->ParamByName("N3")->AsInteger() = N3;
    qIn->ParamByName("N4")->AsInteger() = N4;
    qIn->Open();
}

void TDB_FormStr::GetItemsAll(TQueryUni *qIn, int form_id)
{
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Form_str");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("ORDER BY form_id, form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->Open();
}

void TDB_FormStr::GetStmntItemsN1N2(TQueryUni* qIn, int form_id, int N1, int N2, const std::string& stmnt) {
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Form_str");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND form_str_n BETWEEN :N1 AND :N2");
    qIn->SQL->Add("AND stmnt = :stmnt");
    qIn->SQL->Add("ORDER BY form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("N1")->AsInteger() = N1;
    qIn->ParamByName("N2")->AsInteger() = N2;
    qIn->ParamByName("stmnt")->AsString() = stmnt;
    qIn->Open();
}

void TDB_FormStr::GetStmnt(TQueryUni *qIn, int form_id, int form_str_n)
{
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Form_str");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND form_str_n BETWEEN 1000 AND 1499");
    qIn->SQL->Add("AND form_str_n = :form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("form_str_n")->AsInteger() = form_str_n;
    qIn->Open();
}

void TDB_FormStr::GetRow(TQueryUni *qIn, int form_id, int form_str_n)
{
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Form_str");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND form_str_n = :form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("form_str_n")->AsInteger() = form_str_n;
    qIn->Open();
}

int TDB_FormStr::GetAttr_3(int form_id, int form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_FormStr::GetRow(q, form_id, form_str_n);
    int result = q->FieldByName("attr_3")->AsInteger() - 1;
   //MAK q->Free();
delete q;
    return result;
}

int TDB_FormStr::GetAttr_2(int form_id, int form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_FormStr::GetRow(q, form_id, form_str_n);
    int result = q->FieldByName("attr_2")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_FormStr::GetVidStmnt(int form_id, int form_str_n, int &pokaz_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_FormStr::GetRow(q, form_id, form_str_n);
    string result = q->FieldByName("stmnt")->AsString();
    pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::DelSort(int form_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET is_sort = 0, uroven_sort = 0, is_svod = 0, name_svod = :name_svod");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n BETWEEN 1000 AND 1499");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("name_svod")->AsString() = "";
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::AddSort(int form_id, int NCol, int is_sort, int uroven_sort, int is_svod, const std::string& name_svod) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET is_sort = :is_sort, uroven_sort = :uroven_sort, is_svod = :is_svod, name_svod = :name_svod");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n BETWEEN 1000 AND 1499");
    q->SQL->Add("AND attr_1 = :NCol");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("NCol")->AsInteger() = NCol;
    q->ParamByName("is_sort")->AsInteger() = is_sort;
    q->ParamByName("uroven_sort")->AsInteger() = uroven_sort;
    q->ParamByName("is_svod")->AsInteger() = is_svod;
    q->ParamByName("name_svod")->AsString() = name_svod;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

bool TDB_FormStr::IsClmn(int form_id, int NCol)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT form_id");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n BETWEEN 1000 AND 1499");
    q->SQL->Add("AND attr_1 = :NCol");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("NCol")->AsInteger() = NCol;
    q->Open();
    if (q->EoF()) result = false;
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::GetCellStmnt(TQueryUni* qIn, int form_id, const std::string& stmntText, const std::string& stmnt, int Col, int Row) {
    qIn->SQL->Clear();
 
//mak???    qIn->RequestLive = true;
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Form_str");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND stmnt = :stmntText");
    qIn->SQL->Add("AND note2 = :stmnt");
    qIn->SQL->Add("AND attr_1 = :Col");
    qIn->SQL->Add("AND attr_2 = :Row");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("stmntText")->AsString() = stmntText;
    qIn->ParamByName("stmnt")->AsString() = stmnt;
    qIn->ParamByName("Col")->AsInteger() = Col;
    qIn->ParamByName("Row")->AsInteger() = Row;
    qIn->Open();
}

int TDB_FormStr::GetForm_str_n(int form_id, const std::string& stmnt, int CellContentNum){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = CellContentNum;
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND stmnt = :stmnt");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("stmnt")->AsString() = stmnt;
    q->Open();
    if (!q->EoF()) {
        q->Last();
        Result = q->FieldByName("form_str_n")->AsInteger() + 1;
    }
   //MAK q->Free();
delete q;
    return Result;
}

bool TDB_FormStr::GetGroupRowsInfo(int form_id, std::string &JSONGroupRows)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT note2");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = groupRowsForm_str_n;
    q->Open();
    if (!q->EoF()) {
        Result = true;
        JSONGroupRows = q->FieldByName("note2")->AsString();
    }
   //MAK q->Free();
delete q;
    return Result;
}

void TDB_FormStr::GetStmntAll(TQueryUni *qIn, int form_id)
{
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Form_str");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->SQL->Add("AND form_str_n BETWEEN 1000 AND 1499");
    qIn->SQL->Add("ORDER BY form_str_n");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->Open();
}

void TDB_FormStr::GetFontInfo(int form_id, int form_str_n, std::string &font, int &h, int &is_bold)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    font = "";
    h = 0;
    is_bold = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        font = q->FieldByName("font")->AsString();
        h = q->FieldByName("h")->AsInteger();
        is_bold = q->FieldByName("is_bold")->AsInteger();
    }
   //MAK q->Free();
delete q;
}

void TDB_FormStr::SetFont(int form_id, int form_str_n, const std::string& font, int h, int is_bold)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET font = :font, h = :h, is_bold = :is_bold");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("font")->AsString() = font;
    q->ParamByName("h")->AsInteger() = h;
    q->ParamByName("is_bold")->AsInteger() = is_bold;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::AddFont(int form_id, int form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Form_str");
    q->SQL->Add("(form_id, form_str_n, stmnt)");
    q->SQL->Add("values (:form_id, :form_str_n, :Stmnt)");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("Stmnt")->AsString() = "ШРИФТ";
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_FormStr::getfontColor(int form_id, int form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Add("SELECT fontColor");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("fontColor")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::setfontColor(int form_id, int form_str_n, int fontColor)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET fontColor = :fontColor");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("fontColor")->AsInteger() = fontColor;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_FormStr::getBGcolor(int form_id, int form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 16777215; // Default background color
    q->SQL->Add("SELECT patternBGcolor");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        if (q->FieldByName("patternBGcolor")->AsInteger() == 0) {
            result = 16777215; // Default background color
        } else {
            result = q->FieldByName("patternBGcolor")->AsInteger();
        }
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::setBGcolor(int form_id, int form_str_n, int BGcolor)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET patternBGcolor = :patternBGcolor");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("patternBGcolor")->AsInteger() = BGcolor;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_FormStr::getBorderStyle(int form_id, int form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0; // Default border style
    q->SQL->Add("SELECT borderStyle");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("borderStyle")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::setBorderStyle(int form_id, int form_str_n, int borderStyle)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET borderStyle = :borderStyle");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("borderStyle")->AsInteger() = borderStyle;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_FormStr::getBorderColor(int form_id, int form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Add("SELECT borderColor");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("borderColor")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::setBorderColor(int form_id, int form_str_n, int borderColor)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET borderColor = :borderColor");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("borderColor")->AsInteger() = borderColor;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::GetAlignment(int form_id, int form_str_n, int& align_vert, int& align_gor)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    align_vert = 0;
    align_gor = 0;
    q->SQL->Clear();
 
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        align_vert = q->FieldByName("align_vert")->AsInteger();
        align_gor = q->FieldByName("align_gor")->AsInteger();
    }
   //MAK q->Free();
delete q;
}

void TDB_FormStr::SetAlignment(int form_id, int form_str_n, int align_vert, int align_gor)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
 
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET align_vert = :align_vert, align_gor = :align_gor");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("align_vert")->AsInteger() = align_vert;
    q->ParamByName("align_gor")->AsInteger() = align_gor;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_FormStr::GetIsHidden(int form_id, int form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Add("SELECT is_hidden");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("is_hidden")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::SetOdinakovZnac(int form_id, int form_str_n, const std::string& note2)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
 
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note2 = :note2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("note2")->AsString() = note2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::GetObjedinitKolCell(int form_id, int form_str_n, int& is_group)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    is_group = 0;
    q->SQL->Clear();
 
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        is_group = q->FieldByName("is_group")->AsInteger();
    }
   //MAK q->Free();
delete q;
}

void TDB_FormStr::SetObjedinitKolCell(int form_id, int form_str_n, int is_group)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
 
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET is_group = :is_group");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("is_group")->AsInteger() = is_group;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::SetNValidator(int form_id, int form_str_n, int nValidator)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
 
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET nValidator = :nValidator");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("nValidator")->AsInteger() = nValidator;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdPokazId(int form_id, int form_str_n, int pokaz_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
 
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET pokaz_id = :pokaz_id");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdFieldId(int form_id, int form_str_n, int field_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
 
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET field_id = :field_id");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

bool TDB_FormStr::getFieldId(int form_id, int form_str_n, int& field_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT field_id");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->Open();
    if (!q->EoF()) {
        field_id = q->FieldByName("field_id")->AsInteger();
        result = true;
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::UpdPlanId(int form_id, int form_str_n, int plan_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
 
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET plan_id = :plan_id");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdNIerarxii(int form_id, int form_str_n, int nIerarxii)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
 
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET form_str_nSub = :nIerarxii");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("nIerarxii")->AsInteger() = nIerarxii;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdForm_str_n(int form_id, int form_str_n, int new_form_str_n)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET form_str_n = :new_form_str_n");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("new_form_str_n")->AsInteger() = new_form_str_n;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdForm_str_nAttr_1(int form_id, int form_str_n, int new_form_str_n, int attr_1)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET form_str_n = :new_form_str_n, attr_1 = :attr_1");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("new_form_str_n")->AsInteger() = new_form_str_n;
    q->ParamByName("attr_1")->AsInteger() = attr_1;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdGroupRowsInfo(int form_id, std::string JSONGroupRows)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET note2 = :note2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("note2")->AsString() = JSONGroupRows;
    q->ParamByName("form_str_n")->AsInteger() = groupRowsForm_str_n;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdAttr_2(int form_id, int form_str_n, int attr_2)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET attr_2 = :attr_2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("attr_2")->AsInteger() = attr_2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::NaznacCol_LeftOrRight(int form_id, int aCol, bool isLeft)
{
    int nColTo;
    if (isLeft)
        nColTo = aCol - 1;
    else
        nColTo = aCol + 1;

    TDB_FormStr::UpdForm_str_n(form_id, aCol + 1000, aCol + 1000 + 30000);
    TDB_FormStr::UpdForm_str_n(form_id, nColTo + 1000, nColTo + 1000 + 30000);

    TDB_FormStr::UpdForm_str_nAttr_1(form_id, aCol + 1000 + 30000, nColTo + 1000, nColTo);
    TDB_FormStr::UpdForm_str_nAttr_1(form_id, nColTo + 1000 + 30000, aCol + 1000, aCol);
}

int TDB_FormStr::GetIndexByStmnt(std::string stmnt)
{
    int result = -1;

    int maxIndex = std::min(Kolic, static_cast<int>(stmnt_all.size()) - 1);

    for (int i = 0; i <= maxIndex; ++i) {
        if (stmnt_all[i].empty())
            return -1;
        if (stmnt_all[i] != stmnt)
            continue;

        result = i;
        break;
    }

    return result;
}

void TDB_FormStr::UpdValidator(int form_id, int form_str_n, int nValidator, int vid_uslovij, double value1, double value2, const std::string& shablon, const std::string& Msg)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET nValidator = :nValidator, vid_uslovij = :vid_uslovij, value1 = :value1, value2 = :value2, shablon = :shablon, Msg = :Msg");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("nValidator")->AsInteger() = nValidator;
    q->ParamByName("vid_uslovij")->AsInteger() = vid_uslovij;
    q->ParamByName("value1")->AsFloat() = value1;
    q->ParamByName("value2")->AsFloat() = value2;
    q->ParamByName("shablon")->AsString() = shablon;
    q->ParamByName("Msg")->AsString() = Msg;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::UpdShablon(int form_id, int form_str_n, std::string shablon)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET shablon = :shablon");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_str_n")->AsInteger() = form_str_n;
    q->ParamByName("shablon")->AsString() = shablon;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

std::string TDB_FormStr::getNumberFormat(int form_id, int form_str_n) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";

    q->SQL->Clear();
 
    q->SQL->Add("SELECT numberformat");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id; // Added () after AsInteger
    q->ParamByName("form_str_n")->AsInteger() = form_str_n; // Added () after AsInteger
    q->Open();
    
    if (!q->EoF()) {
        result = q->FieldByName("numberformat")->AsString(); // Added () after AsString
    }
    
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::UpdnumberFormat(int form_id, int form_str_n, const std::string& numberFormat) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET numberFormat=:numberFormat");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id; // Added () after AsInteger
    q->ParamByName("form_str_n")->AsInteger() = form_str_n; // Added () after AsInteger
    q->ParamByName("numberFormat")->AsString() = numberFormat; // Added () after AsString
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_FormStr::GetColDestList(TQueryUni *qIn, int form_id) {
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT attr_1, attr_2, attr_3, attr_4, is_group, period, stmnt");
    qIn->SQL->Add("FROM Form_str");
    qIn->SQL->Add("WHERE form_id = :form_id AND");
    qIn->SQL->Add("(stmnt = :stmnt01 OR stmnt = :stmnt02 OR");
    qIn->SQL->Add("stmnt = :stmnt03 OR stmnt = :stmnt04 OR");
    qIn->SQL->Add("stmnt = :stmnt05 OR stmnt = :stmnt06 OR");
    qIn->SQL->Add("stmnt = :stmnt07 OR");
    qIn->SQL->Add("stmnt = :stmnt08 OR");
    qIn->SQL->Add("stmnt = :stmnt09 OR stmnt = :stmnt10 OR");
    qIn->SQL->Add("stmnt = :stmnt11 OR stmnt = :stmnt12 OR");
    qIn->SQL->Add("stmnt = :stmnt13 OR");
    qIn->SQL->Add("stmnt = :stmnt14 OR");
    qIn->SQL->Add("stmnt = :stmnt15 OR stmnt = :stmnt16 OR stmnt = :stmnt17 OR");
    qIn->SQL->Add("stmnt = :stmnt18)");
    qIn->ParamByName("form_id")->AsInteger() = form_id; // Added () after AsInteger
    qIn->ParamByName("stmnt01")->AsString() = "ЗНАЧЕНИЕСВ"; // Added () after AsString
    qIn->ParamByName("stmnt02")->AsString() = "ИСТЗНАЧВВОДПК"; // Added () after AsString
    qIn->ParamByName("stmnt03")->AsString() = "ПРОЦЕНТСВ"; // Added () after AsString
    qIn->ParamByName("stmnt04")->AsString() = "ОТКЛОНЕНИЕСВ"; // Added () after AsString
    qIn->ParamByName("stmnt05")->AsString() = "ЗНАЧЕНИЕСВНИ"; // Added () after AsString
    qIn->ParamByName("stmnt06")->AsString() = "ПРОЦЕНТСВНИ"; // Added () after AsString
    qIn->ParamByName("stmnt07")->AsString() = "КБК-Дгвр:Сумма№БО"; // Added () after AsString
    qIn->ParamByName("stmnt08")->AsString() = "ЗНАЧЕНИЕСВИР"; // Added () after AsString
    qIn->ParamByName("stmnt09")->AsString() = "ПРОЦЕНТНВ"; // Added () after AsString
    qIn->ParamByName("stmnt10")->AsString() = "ОТКЛОНЕНИЕНВ"; // Added () after AsString
    qIn->ParamByName("stmnt11")->AsString() = "ЗНАЧЕНИЕНВНИ"; // Added () after AsString
    qIn->ParamByName("stmnt12")->AsString() = "ПРОЦЕНТНВНИ"; // Added () after AsString
    qIn->ParamByName("stmnt13")->AsString() = "КБК-Дгвр:СуммаБО"; // Added () after AsString
    qIn->ParamByName("stmnt14")->AsString() = "ЗНАЧЕНИЕНВИР"; // Added () after AsString
    qIn->ParamByName("stmnt15")->AsString() = "КОДАУ"; // Added () after AsString
    qIn->ParamByName("stmnt16")->AsString() = "КОДБК"; // Added () after AsString
    qIn->ParamByName("stmnt17")->AsString() = "ЕДИЗМ"; // Added () after AsString
    qIn->ParamByName("stmnt18")->AsString() = "ЕДИЗМНВ"; // Added () after AsString
    qIn->Open();
}

std::string TDB_FormStr::getName_svod(int form_id, int form_str_n) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";

    q->SQL->Clear();
    q->SQL->Add("SELECT name_svod");
    q->SQL->Add("FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id; // Added () after AsInteger
    q->ParamByName("form_str_n")->AsInteger() = form_str_n; // Added () after AsInteger
    q->Open();
    
    if (!q->EoF()) {
        result = q->FieldByName("name_svod")->AsString(); // Added () after AsString
    }
    
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::setName_svod(int form_id, int form_str_n, const std::string& name_svod) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Form_str");
    q->SQL->Add("SET name_svod = :name_svod");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n = :form_str_n");
    q->ParamByName("form_id")->AsInteger() = form_id; // Added () after AsInteger
    q->ParamByName("form_str_n")->AsInteger() = form_str_n; // Added () after AsInteger
    q->ParamByName("name_svod")->AsString() = name_svod; // Added () after AsString
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_FormStr::SetRangeMerged(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, bool merged) {
    if (merged && RangeExists(formId, formStrNFrom, formStrNTo, row1, col1, row2, col2))
        return;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n BETWEEN :form_str_n_from AND :form_str_n_to");
    q->SQL->Add("AND (attr_1 BETWEEN :row1 AND :row2 OR attr_3 BETWEEN :row1 AND :row2");
    q->SQL->Add("OR :row1 BETWEEN attr_1 AND attr_3 OR :row2 BETWEEN attr_1 AND attr_3)");
    q->SQL->Add("AND (attr_2 BETWEEN :col1 AND :col2 OR attr_4 BETWEEN :col1 AND :col2");
    q->SQL->Add("OR :col1 BETWEEN attr_2 AND attr_4 OR :col2 BETWEEN attr_2 AND attr_4)");

    q->ParamByName("form_id")->AsInteger() = formId; // Added () after AsInteger
    q->ParamByName("form_str_n_from")->AsInteger() = formStrNFrom; // Added () after AsInteger
    q->ParamByName("form_str_n_to")->AsInteger() = formStrNTo; // Added () after AsInteger
    q->ParamByName("row1")->AsInteger() = row1; // Added () after AsInteger
    q->ParamByName("col1")->AsInteger() = col1; // Added () after AsInteger
    q->ParamByName("row2")->AsInteger() = row2; // Added () after AsInteger
    q->ParamByName("col2")->AsInteger() = col2; // Added () after AsInteger

    q->ExecSQL();

    if (merged) {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO Form_str (form_id, form_str_n, attr_1, attr_2, attr_3, attr_4)");
        q->SQL->Add("SELECT :form_id,");
        q->SQL->Add("(SELECT COALESCE(MAX(form_str_n),:form_str_n_from)+1 FROM Form_str WHERE form_id = :form_id AND form_str_n >= :form_str_n_from),");
        q->SQL->Add(":row1,");
        q->SQL->Add(":col1,");
        q->SQL->Add(":row2,");
        q->SQL->Add(":col2");

        q->ParamByName("form_id")->AsInteger() = formId; // Added () after AsInteger
        q->ParamByName("form_str_n_from")->AsInteger() = formStrNFrom; // Added () after AsInteger
        q->ParamByName("row1")->AsInteger() = row1; // Added () after AsInteger
        q->ParamByName("col1")->AsInteger() = col1; // Added () after AsInteger
        q->ParamByName("row2")->AsInteger() = row2; // Added () after AsInteger
        q->ParamByName("col2")->AsInteger() = col2; // Added () after AsInteger

        q->ExecSQL();
    }

   //MAK q->Free();
delete q;
}

TQueryUni* TDB_FormStr::GetRangeField(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n BETWEEN :form_str_n_from AND :form_str_n_to");
    q->SQL->Add("AND attr_1 = :row1");
    q->SQL->Add("AND attr_2 = :col1");
    q->SQL->Add("AND attr_3 = :row2");
    q->SQL->Add("AND attr_4 = :col2");

    q->ParamByName("form_id")->AsInteger() = formId; // Added () after AsInteger
    q->ParamByName("form_str_n_from")->AsInteger() = formStrNFrom; // Added () after AsInteger
    q->ParamByName("form_str_n_to")->AsInteger() = formStrNTo; // Added () after AsInteger
    q->ParamByName("row1")->AsInteger() = row1; // Added () after AsInteger
    q->ParamByName("col1")->AsInteger() = col1; // Added () after AsInteger
    q->ParamByName("row2")->AsInteger() = row2; // Added () after AsInteger
    q->ParamByName("col2")->AsInteger() = col2; // Added () after AsInteger

    q->Open();
    return q;
}

bool TDB_FormStr::RangeExists(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT Form_id FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND form_str_n BETWEEN :form_str_n_from AND :form_str_n_to");
    q->SQL->Add("AND attr_1 = :row1");
    q->SQL->Add("AND attr_2 = :col1");
    q->SQL->Add("AND attr_3 = :row2");
    q->SQL->Add("AND attr_4 = :col2");

    q->ParamByName("form_id")->AsInteger() = formId; // Added () after AsInteger
    q->ParamByName("form_str_n_from")->AsInteger() = formStrNFrom; // Added () after AsInteger
    q->ParamByName("form_str_n_to")->AsInteger() = formStrNTo; // Added () after AsInteger
    q->ParamByName("row1")->AsInteger() = row1; // Added () after AsInteger
    q->ParamByName("col1")->AsInteger() = col1; // Added () after AsInteger
    q->ParamByName("row2")->AsInteger() = row2; // Added () after AsInteger
    q->ParamByName("col2")->AsInteger() = col2; // Added () after AsInteger

    q->Open();
    bool result = !q->EoF();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::SetRangeText(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& stmnt, const std::string& text) {
    bool exists = RangeExists(formId, formStrNFrom, formStrNTo, row1, col1, row2, col2);

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    if (exists) {
        q->SQL->Add("UPDATE Form_str SET stmnt = :stmnt, note1 = :text");
        q->SQL->Add("WHERE form_id = :form_id");
        q->SQL->Add("AND form_str_n BETWEEN :form_str_n_from AND :form_str_n_to");
        q->SQL->Add("AND attr_1 = :row1");
        q->SQL->Add("AND attr_2 = :col1");
        q->SQL->Add("AND attr_3 = :row2");
        q->SQL->Add("AND attr_4 = :col2");

        q->ParamByName("form_str_n_to")->AsInteger() = formStrNTo; // Added () after AsInteger
    } else {
        q->SQL->Add("INSERT INTO Form_str (form_id, form_str_n, attr_1, attr_2, attr_3, attr_4, stmnt, note1)");
        q->SQL->Add("SELECT :form_id,");
        q->SQL->Add("(SELECT COALESCE(MAX(form_str_n),:form_str_n_from)+1 FROM Form_str WHERE form_id = :form_id AND form_str_n >= :form_str_n_from),");
        q->SQL->Add(":row1,");
        q->SQL->Add(":col1,");
        q->SQL->Add(":row2,");
        q->SQL->Add(":col2,");
        q->SQL->Add(":stmnt,");
        q->SQL->Add(":text");
    }

    q->ParamByName("form_str_n_from")->AsInteger() = formStrNFrom; // Added () after AsInteger
    q->ParamByName("form_id")->AsInteger() = formId; // Added () after AsInteger
    q->ParamByName("row1")->AsInteger() = row1; // Added () after AsInteger
    q->ParamByName("col1")->AsInteger() = col1; // Added () after AsInteger
    q->ParamByName("row2")->AsInteger() = row2; // Added () after AsInteger
    q->ParamByName("col2")->AsInteger() = col2; // Added () after AsInteger
    q->ParamByName("stmnt")->AsString() = stmnt; // Added () after AsString
    q->ParamByName("text")->AsString() = text; // Added () after AsString

    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::SetRangeCellFormat(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& fontName, int fontSize, int fontStyle, int fontColor, int backgroundColor, int hAlign, int vAlign, int borderStyle, int borderColor) {
    bool exists = RangeExists(formId, formStrNFrom, formStrNTo, row1, col1, row2, col2);

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    if (exists) {
        q->SQL->Add("UPDATE Form_str SET font = :fontName, h = :fontSize, is_bold = :fontStyle, fontColor = :fontColor, patternBGcolor = :backgroundColor, align_vert = :vAlign, align_gor = :hAlign, borderStyle = :borderStyle, borderColor = :borderColor");
        q->SQL->Add("WHERE form_id = :form_id");
        q->SQL->Add("AND form_str_n BETWEEN :form_str_n_from AND :form_str_n_to");
        q->SQL->Add("AND attr_1 = :row1");
        q->SQL->Add("AND attr_2 = :col1");
        q->SQL->Add("AND attr_3 = :row2");
        q->SQL->Add("AND attr_4 = :col2");

        q->ParamByName("form_str_n_to")->AsInteger() = formStrNTo; // Added () after AsInteger
    } else {
        q->SQL->Add("INSERT INTO Form_str (form_id, form_str_n, attr_1, attr_2, attr_3, attr_4, font, h, is_bold, fontColor, patternBGcolor, align_vert, align_gor, borderStyle, borderColor)");
        q->SQL->Add("SELECT :form_id,");
        q->SQL->Add("(SELECT COALESCE(MAX(form_str_n),:form_str_n_from)+1 FROM Form_str WHERE form_id = :form_id AND form_str_n >= :form_str_n_from),");
        q->SQL->Add(":row1,");
        q->SQL->Add(":col1,");
        q->SQL->Add(":row2,");
        q->SQL->Add(":col2,");
        q->SQL->Add(":fontName,");
        q->SQL->Add(":fontSize,");
        q->SQL->Add(":fontStyle,");
        q->SQL->Add(":fontColor,");
        q->SQL->Add(":backgroundColor,");
        q->SQL->Add(":vAlign,");
        q->SQL->Add(":hAlign,");
        q->SQL->Add(":borderStyle,");
        q->SQL->Add(":borderColor");
    }

    q->ParamByName("form_str_n_from")->AsInteger() = formStrNFrom; // Added () after AsInteger
    q->ParamByName("form_id")->AsInteger() = formId; // Added () after AsInteger
    q->ParamByName("row1")->AsInteger() = row1; // Added () after AsInteger
    q->ParamByName("col1")->AsInteger() = col1; // Added () after AsInteger
    q->ParamByName("row2")->AsInteger() = row2; // Added () after AsInteger
    q->ParamByName("col2")->AsInteger() = col2; // Added () after AsInteger
    q->ParamByName("fontName")->AsString() = fontName; // Added () after AsString
    q->ParamByName("fontSize")->AsInteger() = fontSize; // Added () after AsInteger
    q->ParamByName("fontStyle")->AsInteger() = fontStyle; // Added () after AsInteger
    q->ParamByName("fontColor")->AsInteger() = fontColor; // Added () after AsInteger
    q->ParamByName("backgroundColor")->AsInteger() = backgroundColor; // Added () after AsInteger
    q->ParamByName("vAlign")->AsInteger() = vAlign; // Added () after AsInteger
    q->ParamByName("hAlign")->AsInteger() = hAlign; // Added () after AsInteger
    q->ParamByName("borderStyle")->AsInteger() = borderStyle; // Added () after AsInteger
    q->ParamByName("borderColor")->AsInteger() = borderColor; // Added () after AsInteger

    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::SetRangeField(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& stmnt, int fieldId, int readonly, int tocnost, int isIstFin, int istFinGradId, int period, int formStrNSub, int vidOtch, const std::string& dinIzm, int isSetGod, int godPlus, int godRavno, int planId, int coefPlus1, int valuePeriod, int checkOnClose, int checkValidator, int checkConditionType, double checkValue1, double checkValue2, const std::string& checkShablon, const std::string& checkMsg, const std::string& additionalCellStyle) {
    bool exists = RangeExists(formId, formStrNFrom, formStrNTo, row1, col1, row2, col2);

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    if (exists) {
        q->SQL->Add("UPDATE Form_str SET stmnt = :stmnt, field_id = :fieldId,");
        q->SQL->Add("readonly = :readonly, tocnost = :tocnost, is_ist_fin = :isIstFin, ist_fin_grad_id = :istFinGradId,");
        q->SQL->Add("period = :period, form_str_nSub = :formStrNSub, vid_otch = :vidOtch, din_izm = :dinIzm,");
        q->SQL->Add("is_set_god = :isSetGod, god_plus = :godPlus, god_ravno = :godRavno, plan_id = :planId,");
        q->SQL->Add("CoefPlus1 = :coefPlus1, ValuePeriod = :valuePeriod, is_Check = :checkOnClose, nValidator = :checkValidator,");
        q->SQL->Add("vid_uslovij = :checkConditionType, value1 = :checkValue1, value2 = :checkValue2, shablon = :checkShablon, Msg = :checkMsg, note2 = :additionalCellStyle");
        q->SQL->Add("WHERE form_id = :form_id");
        q->SQL->Add("AND form_str_n BETWEEN :form_str_n_from AND :form_str_n_to");
        q->SQL->Add("AND attr_1 = :row1");
        q->SQL->Add("AND attr_2 = :col1");
        q->SQL->Add("AND attr_3 = :row2");
        q->SQL->Add("AND attr_4 = :col2");

        q->ParamByName("form_str_n_to")->AsInteger() = formStrNTo; // Added () after AsInteger
    } else {
        q->SQL->Add("INSERT INTO Form_str (form_id, form_str_n, attr_1, attr_2, attr_3, attr_4, stmnt, field_id,");
        q->SQL->Add("readonly, tocnost, is_ist_fin, ist_fin_grad_id,");
        q->SQL->Add("period, form_str_nSub, vid_otch, din_izm,");
        q->SQL->Add("is_set_god, god_plus, god_ravno, plan_id,");
        q->SQL->Add("CoefPlus1, ValuePeriod, is_Check, nValidator, vid_uslovij,");
        q->SQL->Add("value1, value2, shablon, Msg, note2)");
        q->SQL->Add("SELECT :form_id,");
        q->SQL->Add("(SELECT COALESCE(MAX(form_str_n),:form_str_n_from)+1 FROM Form_str WHERE form_id = :form_id AND form_str_n >= :form_str_n_from),");
        q->SQL->Add(":row1,");
        q->SQL->Add(":col1,");
        q->SQL->Add(":row2,");
        q->SQL->Add(":col2,");
        q->SQL->Add(":stmnt,");
        q->SQL->Add(":fieldId,");
        q->SQL->Add(":readonly,");
        q->SQL->Add(":tocnost,");
        q->SQL->Add(":isIstFin,");
        q->SQL->Add(":istFinGradId,");
        q->SQL->Add(":period,");
        q->SQL->Add(":formStrNSub,");
        q->SQL->Add(":vidOtch,");
        q->SQL->Add(":dinIzm,");
        q->SQL->Add(":isSetGod,");
        q->SQL->Add(":godPlus,");
        q->SQL->Add(":godRavno,");
        q->SQL->Add(":planId,");
        q->SQL->Add(":coefPlus1,");
        q->SQL->Add(":valuePeriod,");
        q->SQL->Add(":checkOnClose,");
        q->SQL->Add(":checkValidator,");
        q->SQL->Add(":checkConditionType,");
        q->SQL->Add(":checkValue1,");
        q->SQL->Add(":checkValue2,");
        q->SQL->Add(":checkShablon,");
        q->SQL->Add(":checkMsg,");
        q->SQL->Add(":additionalCellStyle");
    }

    q->ParamByName("form_str_n_from")->AsInteger() = formStrNFrom; // Added () after AsInteger
    q->ParamByName("form_id")->AsInteger() = formId; // Added () after AsInteger
    q->ParamByName("row1")->AsInteger() = row1; // Added () after AsInteger
    q->ParamByName("col1")->AsInteger() = col1; // Added () after AsInteger
    q->ParamByName("row2")->AsInteger() = row2; // Added () after AsInteger
    q->ParamByName("col2")->AsInteger() = col2; // Added () after AsInteger
    q->ParamByName("stmnt")->AsString() = stmnt; // Added () after AsString
    q->ParamByName("fieldId")->AsInteger() = fieldId; // Added () after AsInteger
    q->ParamByName("readonly")->AsInteger() = readonly; // Added () after AsInteger
    q->ParamByName("tocnost")->AsInteger() = tocnost; // Added () after AsInteger
    q->ParamByName("isIstFin")->AsInteger() = isIstFin; // Added () after AsInteger
    q->ParamByName("istFinGradId")->AsInteger() = istFinGradId; // Added () after AsInteger
    q->ParamByName("period")->AsInteger() = period; // Added () after AsInteger
    q->ParamByName("formStrNSub")->AsInteger() = formStrNSub; // Added () after AsInteger
    q->ParamByName("vidOtch")->AsInteger() = vidOtch; // Added () after AsInteger
    q->ParamByName("dinIzm")->AsString() = dinIzm; // Added () after AsString
    q->ParamByName("isSetGod")->AsInteger() = isSetGod; // Added () after AsInteger
    q->ParamByName("godPlus")->AsInteger() = godPlus; // Added () after AsInteger
    q->ParamByName("godRavno")->AsInteger() = godRavno; // Added () after AsInteger
    q->ParamByName("planId")->AsInteger() = planId; // Added () after AsInteger
    q->ParamByName("coefPlus1")->AsInteger() = coefPlus1; // Added () after AsInteger
    q->ParamByName("valuePeriod")->AsInteger() = valuePeriod; // Added () after AsInteger
    q->ParamByName("checkOnClose")->AsInteger() = checkOnClose; // Added () after AsInteger
    q->ParamByName("checkValidator")->AsInteger() = checkValidator; // Added () after AsInteger
    q->ParamByName("checkConditionType")->AsInteger() = checkConditionType; // Added () after AsInteger
    q->ParamByName("checkValue1")->AsFloat() = checkValue1; // Added () after AsFloat
    q->ParamByName("checkValue2")->AsFloat() = checkValue2; // Added () after AsFloat
    q->ParamByName("checkShablon")->AsString() = checkShablon; // Added () after AsString
    q->ParamByName("checkMsg")->AsString() = checkMsg; // Added () after AsString
    q->ParamByName("additionalCellStyle")->AsString() = additionalCellStyle; // Added () after AsString

    q->ExecSQL();
   //MAK q->Free();
delete q;
}

bool TDB_FormStr::IntersectRangeExists(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& stmntText) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT Form_id FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND stmnt = :stmntText");
    q->SQL->Add("AND form_str_n BETWEEN :form_str_n_from AND :form_str_n_to");
    q->SQL->Add("AND (attr_1 BETWEEN :row1 AND :row2 OR attr_3 BETWEEN :row1 AND :row2");
    q->SQL->Add("OR :row1 BETWEEN attr_1 AND attr_3 OR :row2 BETWEEN attr_1 AND attr_3)");
    q->SQL->Add("AND (attr_2 BETWEEN :col1 AND :col2 OR attr_4 BETWEEN :col1 AND :col2");
    q->SQL->Add("OR :col1 BETWEEN attr_2 AND attr_4 OR :col2 BETWEEN attr_2 AND attr_4)");
    q->SQL->Add("AND(attr_1 <> :row1 OR attr_3 <> :row2 OR attr_2 <> :col1 OR attr_4 <> :col2)");
    q->SQL->Add("AND NOT(stmnt LIKE '%_TOP%') AND NOT(stmnt LIKE '%_BOTTOM%')");

    q->ParamByName("form_id")->AsInteger() = formId; // Added () after AsInteger
    q->ParamByName("form_str_n_from")->AsInteger() = formStrNFrom; // Added () after AsInteger
    q->ParamByName("form_str_n_to")->AsInteger() = formStrNTo; // Added () after AsInteger
    q->ParamByName("row1")->AsInteger() = row1; // Added () after AsInteger
    q->ParamByName("col1")->AsInteger() = col1; // Added () after AsInteger
    q->ParamByName("row2")->AsInteger() = row2; // Added () after AsInteger
    q->ParamByName("col2")->AsInteger() = col2; // Added () after AsInteger
    q->ParamByName("stmntText")->AsString() = stmntText; // Added () after AsString

    q->Open();
    bool result = !q->EoF();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_FormStr::IntersectRangeDelete(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& stmntText) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND stmnt = :stmntText");
    q->SQL->Add("AND form_str_n BETWEEN :form_str_n_from AND :form_str_n_to");
    q->SQL->Add("AND (attr_1 BETWEEN :row1 AND :row2 OR attr_3 BETWEEN :row1 AND :row2");
    q->SQL->Add("OR :row1 BETWEEN attr_1 AND attr_3 OR :row2 BETWEEN attr_1 AND attr_3)");
    q->SQL->Add("AND (attr_2 BETWEEN :col1 AND :col2 OR attr_4 BETWEEN :col1 AND :col2");
    q->SQL->Add("OR :col1 BETWEEN attr_2 AND attr_4 OR :col2 BETWEEN attr_2 AND attr_4)");
    q->SQL->Add("AND(attr_1 <> :row1 OR attr_3 <> :row2 OR attr_2 <> :col1 OR attr_4 <> :col2)");
    q->SQL->Add("AND NOT(stmnt LIKE '%_TOP%') AND NOT(stmnt LIKE '%_BOTTOM%')");

    q->ParamByName("form_id")->AsInteger() = formId; // Added () after AsInteger
    q->ParamByName("form_str_n_from")->AsInteger() = formStrNFrom; // Added () after AsInteger
    q->ParamByName("form_str_n_to")->AsInteger() = formStrNTo; // Added () after AsInteger
    q->ParamByName("row1")->AsInteger() = row1; // Added () after AsInteger
    q->ParamByName("col1")->AsInteger() = col1; // Added () after AsInteger
    q->ParamByName("row2")->AsInteger() = row2; // Added () after AsInteger
    q->ParamByName("col2")->AsInteger() = col2; // Added () after AsInteger
    q->ParamByName("stmntText")->AsString() = stmntText; // Added () after AsString

    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_FormStr::LoadTostmntList(TStringList* stmntList) {
    stmntList->Add("ЗНАЧЕНИЕСВ");
    stmntList->Add("НАИМПОКАЗальтерн");
    stmntList->Add("НАИМЕНОВАНИЕ");
    // --stmntList->Add("ЗНАЧЕНИЕНВ");
    stmntList->Add("ИСТЗНАЧВВОДПК");
    stmntList->Add("НАИМПОКАЗ");
    stmntList->Add("КОДБК");
    stmntList->Add("ЕДИЗМ");
    stmntList->Add("ЕДИЗМНВ");
    // --stmntList->Add("ПРОЦЕНТСВ");
    // --stmntList->Add("ПРОЦЕНТНВНИ");
    stmntList->Add("ЕДИЗМобщНП");
    stmntList->Add("ДатаСоздОбъекта"); // 'ОТКЛОНЕНИЕСВ'
    stmntList->Add("ЗНАЧЕНИЕСВ_ДООПРЕД");
    stmntList->Add("ЗНАЧЕНИЕСВНИ");
    stmntList->Add("ЗНАЧЕНИЕНВНИ");
    stmntList->Add("КБК-Дгвр:Сумма№БО"); // 'ОТКЛОНЕНИЕСВНИ'
    stmntList->Add("КБК-Дгвр:СуммаБО");  // 'ОТКЛОНЕНИЕНВНИ'
    stmntList->Add("КБК-Дгвр:Сумма№БОоз");  // 'ЗНАЧЕНИЕНВИР'
    stmntList->Add("ЗНАЧЕНИЕСВИР");
    stmntList->Add("ЗНАЧЕНИЕНВИР");
    stmntList->Add("РАСЧСЧЕТ");
    stmntList->Add("ПРОИЗВСТОЛБ");
    stmntList->Add("ОТКЛОНЕНИЕСВНачКВ");
    stmntList->Add("ОТКЛОНЕНИЕНВНачКВ");
    stmntList->Add("ФОРМУЛАF1");
    stmntList->Add("УТОЧНЕНИЕСВ");
    stmntList->Add("УТОЧНЕНИЕСВНИ");
    stmntList->Add("Мрпр-Дгвр:Сумма№БО");
    stmntList->Add("Мрпр-Дгвр:СуммаБО");
    stmntList->Add("ПлнОСТАТОК");
    stmntList->Add("ФктОСТАТОК");
    stmntList->Add("РАСЧСЧЕТ2");
    stmntList->Add("КОДБКРздл");
    stmntList->Add("КОДБКЦелСт");
    stmntList->Add("КОДБКВидРасх");
    stmntList->Add("КОДБКЭкКл");
    stmntList->Add("НомерМрпр");
    stmntList->Add("ПлнОСТАТОКНИ");
    stmntList->Add("ИНН");
    stmntList->Add("ЮрАдрес");
    stmntList->Add("УТОЧНЕНИЕСВПосле");
    stmntList->Add("ФактВсегоВНачале");
    stmntList->Add("ПоступлВсегоВНачале");
    stmntList->Add("СуммаПриход");
    stmntList->Add("СуммаВозврат");
    stmntList->Add("СуммаПлГрНИВНачале");
    stmntList->Add("ФактКвДату");
    stmntList->Add("СуммаПлГрНИВМесяце");
    stmntList->Add("ааЗначВырМнрг");
    stmntList->Add("ааДельтаЭтлн");
    stmntList->Add("ааИдентифТенд");
    stmntList->Add("ЛицевойСчетТБ");
    stmntList->Add("ааПрирост");
    stmntList->Add("ааПроцПриростБаза");
    stmntList->Add("УточПланВклДату");
    stmntList->Add("КОДБК1");
    stmntList->Add("КОДБКРздл1");
    stmntList->Add("КОДБКЦелСт1");
    stmntList->Add("КБК-Дгвр:Кол-во");
    stmntList->Add("КБК-Дгвр:Кол-воБО");
    stmntList->Add("КБК-Дгвр:СуммаКЗ");
    stmntList->Add("БДЖПОЛУЧТЛЬ");
    stmntList->Add("ПДГ_УТ_КассПл");
    stmntList->Add("УТВ_УТ_КассПл");
    stmntList->Add("Примечание");
    stmntList->Add("Ведомство");
    stmntList->Add("СуммаПлГрНИГод");
    stmntList->Add("ЗНАЧЕНИЕобщНП");
    stmntList->Add("БДЖПОЛУЧТЛЬ_нет()");
    stmntList->Add("ХарактСВобъекта");
    stmntList->Add("КонкатСтрок");
    stmntList->Add("КонкатСтрокСВобъек");
    stmntList->Add("№п/п");
    stmntList->Add("Экономия/Резерв");
    stmntList->Add("отчетCO_RP_015");
    stmntList->Add("СведенияОбЭкономии");
    stmntList->Add("НАИМЦелСт");
    stmntList->Add("Владелец");
    stmntList->Add("forUnikIdentif");
    stmntList->Add("ЗНАЧЕНИЕобщНПИР");
    stmntList->Add("ПолныйНомер");
    stmntList->Add("NamePlanВстроке");
    stmntList->Add("ГодВстроке");
    stmntList->Add("OKTMOPlanВстроке");
    stmntList->Add("СВОДпоСУБЪвАльб");
    stmntList->Add("СВОДпоСУБЪвА/n");
    stmntList->Add("СВОДобщНПпоСУБЪвАльб");
    stmntList->Add("СВОДобщНПпоСУБЪвА/n");
    stmntList->Add("ПризнакУчастияВСвязи");
    stmntList->Add("СистХарОбъекта");
    stmntList->Add("СистХарПоказтля");
    stmntList->Add("ВводЗначения");
    stmntList->Add("ОперПрисвоить");
    stmntList->Add("ОткрытьФормуПодчинОб");
    stmntList->Add("ОткрытьФормуТекущОб");
    stmntList->Add("TreeImgShow/Hide1");
    stmntList->Add("TreeImgShow/HideNext");
}
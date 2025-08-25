
#include "DB_Common.h"
#include "DB_Forms.h"
#include "DB_FormStr.h"
#include "unDefDBconnect.h"
#include "unDBType.h"
#include "SystFuncsLogic.h"

void TDB_Forms::GetAllFormAlbum(TQueryUni *qIn, int album_id, TDateTime byDate) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT Forms.*, form_order, form_is_main, readonly as form_is_readonly, nModulOfMetods");
    qIn->SQL->Add("FROM Album_form, Forms");
    qIn->SQL->Add("WHERE :CurrDate BETWEEN form_beg_date AND form_end_date");
    qIn->SQL->Add("AND album_id = :album_id AND Album_form.form_id = Forms.form_id");
    qIn->SQL->Add("ORDER BY form_order");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
//    qIn->ParamByName("CurrDate")->AsDateTime() = byDate.getDateTime();
    qIn->ParamByName("CurrDate")->AsString() = byDate.ToString();

    //std::string strDate = byDate.ToString();
    qIn->Open();
}

void TDB_Forms::Get1FormWithoutAlbum(TQueryUni *qIn, int album_id, TDateTime byDate) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *, 1 as form_order, 0 as form_is_main, 0 as nModulOfMetods");
    qIn->SQL->Add("FROM Forms");
    qIn->SQL->Add("WHERE :CurrDate BETWEEN form_beg_date AND form_end_date");
    qIn->SQL->Add("AND Forms.form_id = :form_id");
//    qIn->SQL->Add("WHERE Forms.form_id = :form_id");
    qIn->ParamByName("form_id")->AsInteger() = -album_id;
//    qIn->ParamByName("CurrDate")->AsDateTime() = byDate.get();
    qIn->ParamByName("CurrDate")->AsString() = byDate.ToString();
    qIn->Open();
}


int TDB_Forms::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int form_id = 0;
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as form_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Forms', 'form_id')) as form_id");
            break;
    }
    q->Open();
    if (!q->EoF())
        form_id = q->FieldByName("form_id")->AsInteger();
   //MAK q->Free();
delete q;
    return form_id;
}

//void TDB_Forms::GetAllFormAlbum(TQueryUni *qIn, int album_id, TDateTime byDate) {
//    qIn->SQL->Clear();
//    qIn->SQL->Add("SELECT Forms.*, form_order, form_is_main, readonly as form_is_readonly, nModulOfMetods");
//    qIn->SQL->Add("FROM Album_form, Forms");
//    qIn->SQL->Add("WHERE :CurrDate BETWEEN form_beg_date AND form_end_date");
//    qIn->SQL->Add("AND album_id = :album_id AND Album_form.form_id = Forms.form_id");
//    qIn->SQL->Add("ORDER BY form_order");
//    qIn->ParamByName("album_id")->AsInteger() = album_id;
//    qIn->ParamByName("CurrDate")->AsDateTime() = byDate;
//    qIn->Open();
//}

void TDB_Forms::Get1FormAlbum(TQueryUni *qIn, int album_id, int form_id, TDateTime byDate) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT Forms.*, form_order, form_is_main, readonly as form_is_readonly, nModulOfMetods");
    qIn->SQL->Add("FROM Album_form, Forms");
    qIn->SQL->Add("WHERE :CurrDate BETWEEN form_beg_date AND form_end_date");
    qIn->SQL->Add("AND album_id = :album_id AND Album_form.form_id = :form_id");
    qIn->SQL->Add("AND Forms.form_id = Album_form.form_id");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->ParamByName("CurrDate")->AsDateTime() = byDate;
    qIn->Open();
}

//void TDB_Forms::Get1FormWithoutAlbum(TQueryUni *qIn, int album_id, TDateTime byDate) {
//    qIn->SQL->Clear();
//    qIn->SQL->Add("SELECT *, 1 as form_order, 0 as form_is_main, 0 as nModulOfMetods");
//    qIn->SQL->Add("FROM Forms");
//    qIn->SQL->Add("WHERE :CurrDate BETWEEN form_beg_date AND form_end_date");
//    qIn->SQL->Add("AND Forms.form_id = :form_id");
//    qIn->ParamByName("form_id")->AsInteger() = -album_id;
//    qIn->ParamByName("CurrDate")->AsDateTime() = byDate;
//    qIn->Open();
//}

bool TDB_Forms::getNextFormAtr(int album_id, int form_idCur, int form_orderCur, int &form_idNext, int &form_orderNext, TDateTime byDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT Album_form.form_id, Album_form.form_order");
    q->SQL->Add("FROM Album_form, Forms");
    q->SQL->Add("WHERE :CurrDate BETWEEN form_beg_date AND form_end_date");
    q->SQL->Add("AND album_id = :album_id AND Album_form.form_id = Forms.form_id");
    q->SQL->Add("AND Album_form.form_order > :form_orderCur");
    q->SQL->Add("ORDER BY form_order");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_orderCur")->AsInteger() = form_orderCur;
    q->ParamByName("CurrDate")->AsDateTime() = byDate;
    q->Open();
    if (!q->EoF()) {
        result = true;
        form_idNext = q->FieldByName("form_id")->AsInteger();
        form_orderNext = q->FieldByName("form_order")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_Forms::GetForm(TQueryUni *qIn, int form_id, bool isMsg) {
    bool result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Forms");
    qIn->SQL->Add("WHERE form_id = :form_id");
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->Open();
    if (qIn->EoF()) {
        if (isMsg)
            //throw Exception("Форма по form_id не найдена");
        result = false;
    }
    return result;
}

int TDB_Forms::GetFormIdByObozn(string form_obozn, int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT F.form_id");
    q->SQL->Add("FROM Album_form AF, Forms F");
    q->SQL->Add("WHERE AF.album_id = :album_id");
    q->SQL->Add("AND AF.form_id = F.form_id");
    q->SQL->Add("AND F.form_obozn = :form_obozn");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_obozn")->AsString() = form_obozn;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("form_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_Forms::isFormExist(int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = TDB_Forms::GetForm(q, form_id, false);
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Forms::GetVidForm(int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetForm(q, form_id, true);
    int result = q->FieldByName("vid_form")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Forms::GetWorkWith(int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetForm(q, form_id, false);
    int result = q->FieldByName("work_with")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Forms::GetFormK2(int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetForm(q, form_id, false);
    int result = q->FieldByName("form_k2")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

string TDB_Forms::GetNameForm(int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool formExist = GetForm(q, form_id, false);
    string result = formExist ? q->FieldByName("form_obozn")->AsString() : "???";
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Forms::GetPlanIdForm(int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetForm(q, form_id, true);
    int result = q->FieldByName("plan_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_Forms::GetAllForm(TQueryUni *qIn, int userctgr_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Forms WHERE userctgr_id = :userctgr_id ORDER BY form_obozn");
    qIn->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qIn->Open();
}

void TDB_Forms::GetAllAlbForm(TQueryUni *qIn, int userctgr_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT Album.album_id, Album.album_obozn, Forms.form_id FROM Album, Album_form, Forms WHERE Album.userctgr_id = :userctgr_id AND Album_form.album_id = Album.album_id AND Forms.form_id = Album_form.form_id ORDER BY Album.album_obozn");
    qIn->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qIn->Open();
}

int TDB_Forms::AddForm(int plan_id, std::string form_obozn, std::string form_name, TDateTime form_beg_date,
    TDateTime form_end_date, int pokaz_id, int col_n, int form_readonly, int form_k1, int form_k2,
    int form_k_zagtop, int form_k_zagbottom, int is_dublir_itogo, int userctgr_id, int work_with,
    int class_id, int ogr_class_id, int for_class, int is_add, int is_copy, int is_sort, int vid_form,
    int is_edIzm, int NZamorog, int is_svod, int tocnost, int isLandscape, int isShowZero,
    std::string OgrUserOprcii, int isShapkaColor, int isLinesColor) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Forms (plan_id, form_obozn, form_name, form_beg_date, form_end_date, form_create_date, pokaz_id, col_n, form_readonly, form_k1, form_k2, form_k_zagtop, form_k_zagbottom, is_dublir_itogo, userctgr_id, work_with, class_id, ogr_class_id, for_class, is_add, is_copy, is_sort, vid_form, is_edIzm, is_noout_nstr, is_svod, tocnost, isLandscape, isShowZero, OgrUserOprcii, isShapkaColor, isLinesColor) VALUES (:plan_id, :form_obozn, :form_name, :form_beg_date, :form_end_date, :form_create_date, :pokaz_id, :col_n, :form_readonly, :form_k1, :form_k2, :form_k_zagtop, :form_k_zagbottom, :is_dublir_itogo, :userctgr_id, :work_with, :class_id, :ogr_class_id, :for_class, :is_add, :is_copy, :is_sort, :vid_form, :is_edIzm, :is_noout_nstr, :is_svod, :tocnost, :isLandscape, :isShowZero, :OgrUserOprcii, :isShapkaColor, :isLinesColor)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("form_obozn")->AsString() = form_obozn;
    q->ParamByName("form_name")->AsString() = form_name;
    q->ParamByName("form_beg_date")->AsDateTime() = form_beg_date;
    q->ParamByName("form_end_date")->AsDateTime() = form_end_date;
    q->ParamByName("form_create_date")->AsDateTime() = Date();
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("col_n")->AsInteger() = col_n;
    q->ParamByName("form_readonly")->AsInteger() = form_readonly;
    q->ParamByName("form_k1")->AsInteger() = form_k1;
    q->ParamByName("form_k2")->AsInteger() = form_k2;
    q->ParamByName("form_k_zagtop")->AsInteger() = form_k_zagtop;
    q->ParamByName("form_k_zagbottom")->AsInteger() = form_k_zagbottom;
    q->ParamByName("is_dublir_itogo")->AsInteger() = is_dublir_itogo;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("work_with")->AsInteger() = work_with;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ParamByName("ogr_class_id")->AsInteger() = ogr_class_id;
    q->ParamByName("for_class")->AsInteger() = for_class;
    q->ParamByName("is_add")->AsInteger() = is_add;
    q->ParamByName("is_copy")->AsInteger() = is_copy;
    q->ParamByName("is_sort")->AsInteger() = is_sort;
    q->ParamByName("vid_form")->AsInteger() = vid_form;
    q->ParamByName("is_edIzm")->AsInteger() = is_edIzm;
    q->ParamByName("is_noout_nstr")->AsInteger() = NZamorog;
    q->ParamByName("is_svod")->AsInteger() = is_svod;
    q->ParamByName("tocnost")->AsInteger() = tocnost;
    q->ParamByName("isLandscape")->AsInteger() = isLandscape;
    q->ParamByName("isShowZero")->AsInteger() = isShowZero;
    q->ParamByName("OgrUserOprcii")->AsString() = OgrUserOprcii;
    q->ParamByName("isShapkaColor")->AsInteger() = isShapkaColor;
    q->ParamByName("isLinesColor")->AsInteger() = isLinesColor;
    q->ExecSQL();

    int form_id;
    q->SQL->Clear();
    q->SQL->Add("SELECT form_id FROM Forms WHERE form_obozn = :form_obozn AND userctgr_id = :userctgr_id");
    q->ParamByName("form_obozn")->AsString() = form_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->Open();
    if (!q->EoF())
        form_id = q->FieldByName("form_id")->AsInteger();
    else
        form_id = 0;
   //MAK q->Free();
delete q;
    return form_id;
}

void TDB_Forms::UpdFormGrFin(int form_id, int work_with_gf) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    // called directly from TDB_Forms::Upd_Work_with_gf
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms SET work_with_gf = :work_with_gf WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("work_with_gf")->AsInteger() = work_with_gf;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Forms::Upd_Work_with_gf(int form_id, int work_with_gf) {
    TDB_Forms::UpdFormGrFin(form_id, work_with_gf);
}

void TDB_Forms::Upd_Work_with(int form_id, int work_with) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    // called directly from TDB_Forms::Upd_Work_with_gf
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms SET work_with = :work_with WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("work_with")->AsInteger() = work_with;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Forms::UpdForm(int form_id, string form_obozn, string form_name,
    TDateTime form_beg_date, TDateTime form_end_date, int pokaz_id, int col_n,
    int form_readonly, int is_dublir_itogo, int class_id, int ogr_class_id,
    int for_class, int is_add, int is_copy, int is_sort, int is_edIzm,
    int NZamorog, int is_svod, string name_svod, string formula_svod,
    int tocnost, int isLandscape, int isShowZero, string OgrUserOprcii,
    int isShapkaColor, int isLinesColor) {
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms SET form_obozn = :form_obozn, form_name = :form_name, form_beg_date = :form_beg_date, ");
    q->SQL->Add("form_end_date = :form_end_date, pokaz_id = :pokaz_id, col_n = :col_n, form_readonly = :form_readonly, ");
    q->SQL->Add("is_dublir_itogo = :is_dublir_itogo, class_id = :class_id, ogr_class_id = :ogr_class_id, ");
    q->SQL->Add("for_class = :for_class, is_add = :is_add, is_copy = :is_copy, is_sort = :is_sort, is_edIzm = :is_edIzm, ");
    q->SQL->Add("is_noout_nstr = :is_noout_nstr, is_svod = :is_svod, name_svod = :name_svod, formula_svod = :formula_svod, ");
    q->SQL->Add("tocnost = :tocnost, isLandscape = :isLandscape, isShowZero = :isShowZero, OgrUserOprcii = :OgrUserOprcii, ");
    q->SQL->Add("isShapkaColor = :isShapkaColor, isLinesColor = :isLinesColor WHERE form_id = :form_id");
    
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_obozn")->AsString() = form_obozn;
    q->ParamByName("form_name")->AsString() = form_name;
    q->ParamByName("form_beg_date")->AsDateTime() = form_beg_date;
    q->ParamByName("form_end_date")->AsDateTime() = form_end_date;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("col_n")->AsInteger() = col_n;
    q->ParamByName("form_readonly")->AsInteger() = form_readonly;
    q->ParamByName("is_dublir_itogo")->AsInteger() = is_dublir_itogo;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ParamByName("ogr_class_id")->AsInteger() = ogr_class_id;
    q->ParamByName("for_class")->AsInteger() = for_class;
    q->ParamByName("is_add")->AsInteger() = is_add;
    q->ParamByName("is_copy")->AsInteger() = is_copy;
    q->ParamByName("is_sort")->AsInteger() = is_sort;
    q->ParamByName("is_edIzm")->AsInteger() = is_edIzm;
    q->ParamByName("is_noout_nstr")->AsInteger() = NZamorog;
    q->ParamByName("is_svod")->AsInteger() = is_svod;
    q->ParamByName("name_svod")->AsString() = name_svod;
    q->ParamByName("formula_svod")->AsString() = formula_svod;
    q->ParamByName("tocnost")->AsInteger() = tocnost;
    q->ParamByName("isLandscape")->AsInteger() = isLandscape;
    q->ParamByName("isShowZero")->AsInteger() = isShowZero;
    q->ParamByName("OgrUserOprcii")->AsString() = OgrUserOprcii;
    q->ParamByName("isShapkaColor")->AsInteger() = isShapkaColor;
    q->ParamByName("isLinesColor")->AsInteger() = isLinesColor;

    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Forms::UpdForm1(int form_id, string name_svod, string formula_svod) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms SET name_svod = :name_svod, formula_svod = :formula_svod WHERE form_id = :form_id");
    
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("name_svod")->AsString() = name_svod;
    q->ParamByName("formula_svod")->AsString() = formula_svod;

    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Forms::UpdForm2(int form_id, int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms SET plan_id = :plan_id WHERE form_id = :form_id");
    
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;

    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Forms::DelForm(int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    TDB_FormStr::DelAllItems(form_id);
    
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Forms WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Album_form WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Forms::DelFormAll(int userctgr_id) {
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    
    qW->SQL->Clear();
    qW->SQL->Add("SELECT * FROM Forms WHERE userctgr_id = :userctgr_id");
    qW->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qW->Open();
    
    while (!qW->EoF()) {
        int form_id = qW->FieldByName("form_id")->AsInteger();
        TDB_Forms::DelForm(form_id);
        qW->Next();
    }
    
    delete qW;
}

void TDB_Forms::UpdUserCtgr(int form_id, int userctgr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms SET userctgr_id = :userctgr_id WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

bool TDB_Forms::UnikName(std::string form_obozn, int userctgr_id, int &form_id){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    bool result = true;
    
    q->SQL->Clear();
    q->SQL->Add("SELECT form_id FROM Forms WHERE form_obozn = :form_obozn AND userctgr_id = :userctgr_id");
    q->ParamByName("form_obozn")->AsString() = form_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->Open();
    
    if (!q->EoF()) {
        result = false;
        form_id = q->FieldByName("form_id")->AsInteger();
    }
    
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_Forms::UnikNameIzm(std::string form_obozn, int userctgr_id, int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = true;

    q->SQL->Clear();
    q->SQL->Add("SELECT form_id FROM Forms");
    q->SQL->Add("WHERE form_obozn = :form_obozn");
    q->SQL->Add("AND userctgr_id = :userctgr_id");
    q->SQL->Add("AND form_id <> :form_id");
    q->ParamByName("form_obozn")->AsString() = form_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->Open();
    if (!q->EoF()) {
        Result = false;
    }
   //MAK q->Free();
delete q;
    return Result;
}

void TDB_Forms::UpdForm_vid_period(int form_id, int form_vid_period) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms");
    q->SQL->Add("SET form_vid_period = :form_vid_period");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_vid_period")->AsInteger() = form_vid_period;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Forms::UpdForm_k2(int form_id, int d) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms");
    q->SQL->Add("SET form_k2 = form_k2 + :d");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("d")->AsInteger() = d;
    q->ExecSQL();
}

void TDB_Forms::UpdForm_k1(int form_id, int d) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms");
    q->SQL->Add("SET form_k1 = form_k1 + :d");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("d")->AsInteger() = d;
    q->ExecSQL();
}

void TDB_Forms::UpdForm_k_zagtop(int form_id, int d) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms");
    q->SQL->Add("SET form_k_zagtop = form_k_zagtop + :d");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("d")->AsInteger() = d;
    q->ExecSQL();
}

void TDB_Forms::UpdForm_k_zagbottom(int form_id, int d) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms");
    q->SQL->Add("SET form_k_zagbottom = form_k_zagbottom + :d");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("d")->AsInteger() = d;
    q->ExecSQL();
}

void TDB_Forms::UpdSheetObozn(int form_id, string SheetObozn) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms");
    q->SQL->Add("SET SheetObozn = :SheetObozn");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("SheetObozn")->AsString() = SheetObozn;
    try {
        q->ExecSQL();
    } catch (std::exception& e) {

        q->SQL->Clear();
        q->SQL->Add("ALTER TABLE Forms");
        q->SQL->Add("ADD SheetObozn VARCHAR(100) NULL");
        q->ExecSQL();

        q->SQL->Clear();
        q->SQL->Add("UPDATE Forms");
        q->SQL->Add("SET SheetObozn = :SheetObozn");
        q->SQL->Add("WHERE form_id = :form_id");
        q->ParamByName("form_id")->AsInteger() = form_id;
        q->ParamByName("SheetObozn")->AsString() = SheetObozn;
        q->ExecSQL();
    }
}

void TDB_Forms::UpdColorLockedCell(int form_id, int ColorLockedCell) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms");
    q->SQL->Add("SET ColorLockedCell = :ColorLockedCell");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("ColorLockedCell")->AsInteger() = ColorLockedCell;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Forms::getInfoNamePokazClmn(int form_id, int &aNamePokazClmn, bool &isHidden) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT col_n, is_edIzm");
    q->SQL->Add("FROM Forms");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->Open();
    aNamePokazClmn = q->FieldByName("col_n")->AsInteger();
    isHidden = q->FieldByName("is_edIzm")->AsInteger() == 2 || q->FieldByName("is_edIzm")->AsInteger() == 3;
   //MAK q->Free();
delete q;
}

void TDB_Forms::SetK1(int form_id, int k1) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms");
    q->SQL->Add("SET form_k1 = :form_k1");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_k1")->AsInteger() = k1;
    q->ExecSQL();
}

void TDB_Forms::SetK2(int form_id, int k2) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Forms");
    q->SQL->Add("SET form_k2 = :form_k2");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("form_k2")->AsInteger() = k2;
    q->ExecSQL();
}

void TDB_Forms::GetK1K2(int form_id, int &k1, int &k2) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (GetForm(q, form_id, false)) {
        k1 = q->FieldByName("form_k1")->AsInteger();
        k2 = q->FieldByName("form_k2")->AsInteger();
    } else {
        k1 = 0;
        k2 = 0;
    }
}



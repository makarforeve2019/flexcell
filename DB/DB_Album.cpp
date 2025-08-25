#include "unDefDBconnect.h"
#include "DB_Userctgr.h"
#include "DB_Album.h"
#include "SystFuncsLogic.h"
// получает всю информацию про альбом album_id и периодичность подчиненных форм
//  YearFixed - вычисленный год, если для альбома задана установка года
void TDB_Album::GetAlbumInfoYQM(int album_id, int& album_vid_func, int& album_isbp, int& readOnly,
                            std::string& album_obozn, int& plan_id_fiks, int& vid_deistv, int& album_typ,
                            int& isYearQuartMonth, int& YearFixed, std::string& JSONFiltr)

//TDB_Album::GetAlbumInfoYQM(int album_id, int &album_vid_func, int &album_isbp, int &readOnly, std::string &album_obozn,
//                            int &plan_id_fiks, int &vid_deistv, int &album_typ, int &isYearQuartMonth, int &YearFixed,
//                            std::string &JSONFiltr)
                            {
    json result; // Создаем JSON-объект для хранения данных

    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_Album::GetItem(&qTmp, album_id); // Выполняем запрос и получаем данные

        if (!qTmp.IsEmpty()) {
            album_vid_func = qTmp.FieldByName("album_vid_func")->AsInteger();
            album_isbp = qTmp.FieldByName("album_isbp")->AsInteger();
            readOnly = qTmp.FieldByName("readonly")->AsInteger();
            album_obozn = qTmp.FieldByName("album_obozn")->AsString();
            plan_id_fiks = qTmp.FieldByName("plan_id_fiks")->AsInteger();
            vid_deistv = qTmp.FieldByName("vid_deistv")->AsInteger();
            album_typ = qTmp.FieldByName("album_typ")->AsInteger();
            JSONFiltr = qTmp.FieldByName("ifopenbeginsost")->AsString();
            YearFixed = 0;

            // Получение текущего времени и даты
            auto now = std::chrono::system_clock::now();
            auto now_time = std::chrono::system_clock::to_time_t(now);
            std::tm local_time = *std::localtime(&now_time);

            // Получение года из текущей даты
            int currentYear = local_time.tm_year + 1900;

            if (!qTmp.FieldByName("is_set_god")->IsNull()) {
                if (qTmp.FieldByName("is_set_god")->AsInteger() == 0) {
                    YearFixed = currentYear + qTmp.FieldByName("god_plus")->AsInteger();
                } else {
                    YearFixed = qTmp.FieldByName("god_ravno")->AsInteger();
                }
            }

            // Переключение на запрос для получения информации о периодичности подчиненных форм
            qTmp.SQL->Clear();
            qTmp.SQL->Add("SELECT Forms.form_id, Forms.form_vid_period FROM Forms INNER JOIN Album_form ON Forms.form_id = Album_form.form_id WHERE Forms.form_vid_period IS NOT NULL AND Album_form.album_id = :album_id");
            qTmp.ParamByName("album_id")->AsInteger() = album_id;
            qTmp.Open();

            isYearQuartMonth = 1;
            if (qTmp.Locate("form_vid_period", 2)) {
                isYearQuartMonth = 4;
            } else if (qTmp.Locate("form_vid_period", 1)) {
                isYearQuartMonth = 2;
            } else if (qTmp.Locate("form_vid_period", 3)) {
                isYearQuartMonth = 3;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
}

void TDB_Album::GetAlbumInfo2(int album_id, int &isUpdDateSave) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (TDB_Album::GetItem(q, album_id)) {
        isUpdDateSave = q->FieldByName("isUpdDateSave")->AsInteger();
    }
   //MAK q->Free();
delete q;
}

// выбрать все формы альбома
void TDB_Album::GetAlbForms(TQueryUni *qIn, int album_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * ");
    qIn->SQL->Add("FROM Album_form, Forms ");
    qIn->SQL->Add("WHERE album_id = :album_id ");
    qIn->SQL->Add("AND Forms.form_id = Album_form.form_id ");
    qIn->SQL->Add("ORDER BY form_order");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->Open();
}

// получает информацию про актуальные по времени формы альбом - следует ли задавать дату за период
bool TDB_Album::GetAlbumZaPeriod(TQueryUni *qIn, int album_id, boost::posix_time::ptime& byDate) {
    std::stringstream ss;
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S");
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << byDate;
    std::string strTime = ss.str();


    qIn->SQL->Add("SELECT Form_str.form_id");
    qIn->SQL->Add("FROM Album_form, Forms, Form_str");
    qIn->SQL->Add("WHERE album_id = :album_id");
    qIn->SQL->Add("AND Forms.form_id = Album_form.form_id");
    qIn->SQL->Add("AND :CurrDate BETWEEN form_beg_date AND form_end_date");
    qIn->SQL->Add("AND Form_str.form_id = Forms.form_id");
    qIn->SQL->Add("AND ((Form_str.stmnt = :stmnt1 AND Form_str.period = 7) OR");
    qIn->SQL->Add("(Form_str.stmnt = :stmnt2 AND Form_str.period = 7) OR");
    qIn->SQL->Add("(Form_str.stmnt = :stmnt3 AND Form_str.period = 7) OR");
    qIn->SQL->Add("(Form_str.stmnt = :stmnt4 AND Form_str.period = 7) OR");
    qIn->SQL->Add("(Form_str.stmnt = :stmnt5 AND Form_str.period = 7) OR");
    qIn->SQL->Add("(Form_str.stmnt = :stmnt6 AND Form_str.period = 7))");

    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->ParamByName("CurrDate")->AsDateTime() = byDate;
    qIn->ParamByName("stmnt1")->AsString() = "ФактВсегоВНачале";
    qIn->ParamByName("stmnt2")->AsString() = "СуммаПлГрНИВНачале";
    qIn->ParamByName("stmnt3")->AsString() = "ЗНАЧЕНИЕСВНИ";
    qIn->ParamByName("stmnt4")->AsString() = "УточПланВклДату";
    qIn->ParamByName("stmnt5")->AsString() = "ЗНАЧЕНИЕСВИР";
    qIn->ParamByName("stmnt6")->AsString() = "ПоступлВсегоВНачале";

    qIn->Open();

    bool result = !qIn->EoF(); // True, если запись найдена
    return result;
}

bool TDB_Album::GetAlbumZaPeriod(int album_id, TDateTime byDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    // есть ли в альбоме форма со столбцом с операцией За Период
    // isForPeriod = true, значит в формах альбома есть столбец На Дату
    // = false, нет такого столбца
    bool Result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT Form_str.form_id");
    q->SQL->Add("FROM Album_form, Forms, Form_str");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND Forms.form_id = Album_form.form_id");
    q->SQL->Add("AND :CurrDate BETWEEN form_beg_date AND form_end_date");
    q->SQL->Add("AND Form_str.form_id = Forms.form_id");
    q->SQL->Add("AND ((Form_str.stmnt = :stmnt1 AND Form_str.period = 7) OR ");
    q->SQL->Add("     (Form_str.stmnt = :stmnt2 AND Form_str.period = 7) OR ");
    q->SQL->Add("     (Form_str.stmnt = :stmnt3 AND Form_str.period = 7) OR ");
    q->SQL->Add("     (Form_str.stmnt = :stmnt4 AND Form_str.period = 7) OR ");
    q->SQL->Add("     (Form_str.stmnt = :stmnt5 AND Form_str.period = 7) OR ");
    q->SQL->Add("     (Form_str.stmnt = :stmnt6 AND Form_str.period = 7))");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("stmnt1")->AsString() = "ФактВсегоВНачале";
    q->ParamByName("stmnt2")->AsString() = "СуммаПлГрНИВНачале";
    q->ParamByName("stmnt3")->AsString() = "ЗНАЧЕНИЕСВНИ";
    q->ParamByName("stmnt4")->AsString() = "УточПланВклДату";
    q->ParamByName("stmnt5")->AsString() = "ЗНАЧЕНИЕСВИР";
    q->ParamByName("stmnt6")->AsString() = "ПоступлВсегоВНачале";
    q->ParamByName("CurrDate")->AsDateTime() = byDate;
    q->Open();
    if (!q->EoF()) Result = true;
   //MAK q->Free();
delete q;
    return Result;
}


// получает всю информацию про альбом album_id
//  YearFixed - вычисленный год, если для альбома задана установка года
void TDB_Album::GetAlbumInfo(int album_id, int& album_vid_func, int& album_isbp, int& readOnly,
                              std::string& album_obozn, int& plan_id_fiks, int& vid_deistv, int& album_typ,
                              int& vid_deistv_Versij, int& YearFixed) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Album::GetItem(q, album_id);
    album_vid_func = q->FieldByName("album_vid_func")->AsInteger();
    album_isbp = q->FieldByName("album_isbp")->AsInteger();
    readOnly = q->FieldByName("readonly")->AsInteger();
    album_obozn = q->FieldByName("album_obozn")->AsString();
    plan_id_fiks = q->FieldByName("plan_id_fiks")->AsInteger();
    vid_deistv = q->FieldByName("vid_deistv")->AsInteger();
    album_typ = q->FieldByName("album_typ")->AsInteger();
    vid_deistv_Versij = q->FieldByName("vid_deistv_Versij")->AsInteger();
    YearFixed = 0;
    if (!q->FieldByName("is_set_god")->IsNull()) {
        if (q->FieldByName("is_set_god")->AsInteger() == 0) {
             YearFixed = YearOf(Now()) + q->FieldByName("god_plus")->AsInteger();
        } else {
            YearFixed = q->FieldByName("god_ravno")->AsInteger();
        }
    }
   //MAK q->Free();
delete q;
}

std::string TDB_Album::getAlbumName(int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Album::GetItem(q, album_id);
    std::string Result = q->FieldByName("album_obozn")->AsString();
   //MAK q->Free();
delete q;
    return Result;
}

std::string TDB_Album::GetDinIzmIds(int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT din_izm_ids FROM Album");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->Open();
    if (!q->EoF()) Result = q->FieldByName("din_izm_ids")->AsString();
   //MAK q->Free();
delete q;
    return Result;
}

int TDB_Album::GetVid_deistv(int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT vid_deistv FROM Album");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->Open();
    if (!q->EoF()) Result = q->FieldByName("vid_deistv")->AsInteger();
   //MAK q->Free();
delete q;
    return Result;
}

// возвращает id последней добавленной записи
int TDB_Album::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    int Result;
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as album_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Album', 'album_id')) as album_id");
            break;
    }
    q->SQL->Add("SELECT currval(pg_get_serial_sequence('Album', 'album_id')) as album_id");
    q->Open();
    Result = q->FieldByName("album_id")->AsInteger();
   //MAK q->Free();
delete q;
    return Result;
}

  // добавить разделитель
int TDB_Album::AddRasdel(int userctgr_id, int owner_id, int album_vid_func, unsigned char album_isbp) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int MaxOrder = TDB_Album::GetMaxOrder(owner_id, userctgr_id);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Album");
    q->SQL->Add("(album_obozn, userctgr_id, owner_id, order_nr, album_typ, album_vid_func, album_isbp)");
    q->SQL->Add("values (:album_obozn, :userctgr_id, :owner_id, :order_nr, 3, :album_vid_func, :album_isbp)");
    q->ParamByName("album_obozn")->AsString() = "-";
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("order_nr")->AsInteger() = MaxOrder + 1;
    q->ParamByName("album_vid_func")->AsInteger() = album_vid_func;
    q->ParamByName("album_isbp")->AsInteger() = album_isbp;
    q->ExecSQL();
    int Result = TDB_Album::GetId();
   //MAK q->Free();
delete q;
    return Result;
}

// удалить разделитель
void TDB_Album::DelRasdel(int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Album");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// получает максимальный порядковый номер
int TDB_Album::GetMaxOrder(int owner_id, int userctgr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int old_id;
    bool isB = TDB_Album::NewToOld(owner_id, old_id);
    if (isB) {
        q->SQL->Clear();
        q->SQL->Add("SELECT MAX (order_nr) FROM Album ");
        q->SQL->Add("WHERE (owner_id = :owner_id");
        q->SQL->Add("OR owner_id = " + IntToStr(old_id));
        q->SQL->Add(")");
        q->SQL->Add("AND userctgr_id = :userctgr_id");
        q->ParamByName("owner_id")->AsInteger() = owner_id;
        q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
        q->Open();
        int result = 0;
        if (q->EoF()) {
            result = 0;
        } else {
            result = q->Fields[0].AsInteger();
        }
       //MAK q->Free();
delete q;
        return result;
    }
    q->SQL->Clear();
    q->SQL->Add("SELECT MAX (order_nr) FROM Album ");
    q->SQL->Add("WHERE owner_id = :owner_id");
    q->SQL->Add("AND userctgr_id = :userctgr_id");
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->Open();
    int result = 0;
    if (q->EoF()) {
        result = 0;
    } else {
        result = q->Fields[0].AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

//функция проверки уникальности имени при добавлении
bool TDB_Album::UnikName(std::string album_obozn, int owner_id, int album_vid_func, int userctgr_id) {
    if (owner_id > 0) {
        return UnikName0(album_obozn, owner_id, userctgr_id);
    }
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT album_id FROM Album");
    q->SQL->Add("WHERE album_obozn = :album_obozn");
    q->SQL->Add("AND userctgr_id = :userctgr_id");
    q->SQL->Add("AND owner_id = :owner_id");
    q->SQL->Add("AND album_vid_func = :album_vid_func");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("album_vid_func")->AsInteger() = album_vid_func;
    q->Open();
    if (!q->EoF()) {
        result = false;
    }
   //MAK q->Free();
delete q;
    return result;
}

//функция проверки уникальности имени при добавлении
bool TDB_Album::UnikName0(std::string album_obozn, int owner_id, int userctgr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT album_id FROM Album");
    q->SQL->Add("WHERE album_obozn = :album_obozn");
    q->SQL->Add("AND userctgr_id = :userctgr_id");
    q->SQL->Add("AND owner_id = :owner_id");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->Open();
    if (!q->EoF()) {
        result = false;
    }
   //MAK q->Free();
delete q;
    return result;
}

// добавить группу
int TDB_Album::AddGrp(std::string album_obozn, int userctgr_id, int owner_id, int album_vid_func, unsigned char album_isbp,
                      int begGod, int endGod) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int MaxOrder = TDB_Album::GetMaxOrder(owner_id, userctgr_id);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Album");
    q->SQL->Add("(album_obozn, userctgr_id, owner_id, order_nr, album_typ, album_vid_func, album_isbp, alb_beg_date, alb_end_date, alb_create_date)");
    q->SQL->Add("values (:album_obozn, :userctgr_id, :owner_id, :order_nr, 2, :album_vid_func, :album_isbp, :alb_beg_date, :alb_end_date, :alb_create_date)");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("order_nr")->AsInteger() = MaxOrder + 1;
    q->ParamByName("album_vid_func")->AsInteger() = album_vid_func;
    q->ParamByName("album_isbp")->AsInteger() = album_isbp;
    q->ParamByName("alb_beg_date")->AsDateTime() = EncodeDate(begGod, 1, 1);
    q->ParamByName("alb_end_date")->AsDateTime() = EncodeDate(endGod, 12, 31);
    q->ParamByName("alb_create_date")->AsDateTime() = Date();
    q->ExecSQL();
    int Result = TDB_Album::GetId();
   //MAK q->Free();
delete q;
    return Result;
}

// добавить блок действий
int TDB_Album::AddBlockDeistv(std::string album_obozn, int userctgr_id, int owner_id, int album_vid_func, unsigned char album_isbp,
                              int begGod, int endGod) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int MaxOrder = TDB_Album::GetMaxOrder(owner_id, userctgr_id);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Album");
    q->SQL->Add("(album_obozn, userctgr_id, owner_id, order_nr, album_typ, album_vid_func, album_isbp, alb_beg_date, alb_end_date, alb_create_date)");
    q->SQL->Add("values (:album_obozn, :userctgr_id, :owner_id, :order_nr, 6, :album_vid_func, :album_isbp, :alb_beg_date, :alb_end_date, :alb_create_date)");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("order_nr")->AsInteger() = MaxOrder + 1;
    q->ParamByName("album_vid_func")->AsInteger() = album_vid_func;
    q->ParamByName("album_isbp")->AsInteger() = album_isbp;
    q->ParamByName("alb_beg_date")->AsDateTime() = EncodeDate(begGod, 1, 1);
    q->ParamByName("alb_end_date")->AsDateTime() = EncodeDate(endGod, 12, 31);
    q->ParamByName("alb_create_date")->AsDateTime() = Date();
    q->ExecSQL();
    int Result = TDB_Album::GetId();
   //MAK q->Free();
delete q;
    return Result;
}

// добавить действие
int TDB_Album::AddDeistv(std::string album_obozn, int userctgr_id, int owner_id,
                         int vid_deistv, int vid_deistv_Versij, int album_vid_func, unsigned char album_isbp,
                         int begGod, int endGod, std::string subNm) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int MaxOrder = TDB_Album::GetMaxOrder(owner_id, userctgr_id);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Album");
    q->SQL->Add("(album_obozn, userctgr_id, owner_id, order_nr, album_typ, vid_deistv, vid_deistv_Versij, album_vid_func, album_isbp, alb_beg_date, alb_end_date, alb_create_date, din_izm_ids)");
    q->SQL->Add("values (:album_obozn, :userctgr_id, :owner_id, :order_nr, 5, :vid_deistv, :vid_deistv_Versij, :album_vid_func, :album_isbp, :alb_beg_date, :alb_end_date, :alb_create_date, :din_izm_ids)");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("order_nr")->AsInteger() = MaxOrder + 1;
    q->ParamByName("vid_deistv")->AsInteger() = vid_deistv;
    q->ParamByName("vid_deistv_Versij")->AsInteger() = vid_deistv_Versij;
    q->ParamByName("album_vid_func")->AsInteger() = album_vid_func;
    q->ParamByName("album_isbp")->AsInteger() = album_isbp;
    q->ParamByName("alb_beg_date")->AsDateTime() = EncodeDate(begGod, 1, 1);
    q->ParamByName("alb_end_date")->AsDateTime() = EncodeDate(endGod, 12, 31);
    q->ParamByName("din_izm_ids")->AsString() = subNm;
    q->ParamByName("alb_create_date")->AsDateTime() = Date();
    q->ExecSQL();
    int Result = TDB_Album::GetId();
   //MAK q->Free();
delete q;
    return Result;
}

// добавить действие с параметром
// Для действия с параметром album_typ оставляем равным 5, КАК ЭТО ЕСТЬ ДЛЯ ДЕЙСТВИЯ
//   то, что это действие с параметром, можно понять по полю vid_deistv, который в данном случае >= 500
// userctgr_idParam кладем в pokaz_id, album_idParam кладем в ist_fin_id
int TDB_Album::AddDeistvParam(std::string album_obozn, int userctgr_id, int owner_id,
                              int vid_deistv, int vid_deistv_Versij, int album_vid_func, unsigned char album_isbp,
                              int begGod, int endGod, std::string subNm, int userctgr_idParam, int album_idParam) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int MaxOrder = TDB_Album::GetMaxOrder(owner_id, userctgr_id);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Album");
    q->SQL->Add("(album_obozn, userctgr_id, owner_id, order_nr, album_typ, vid_deistv, vid_deistv_Versij, album_vid_func, album_isbp, alb_beg_date, alb_end_date, alb_create_date, din_izm_ids, pokaz_id, ist_fin_id)");
    q->SQL->Add("values (:album_obozn, :userctgr_id, :owner_id, :order_nr, 5, :vid_deistv, :vid_deistv_Versij, :album_vid_func, :album_isbp, :alb_beg_date, :alb_end_date, :alb_create_date, :din_izm_ids, :userctgr_idParam, :album_idParam)");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("order_nr")->AsInteger() = MaxOrder + 1;
    q->ParamByName("vid_deistv")->AsInteger() = vid_deistv;
    q->ParamByName("vid_deistv_Versij")->AsInteger() = vid_deistv_Versij;
    q->ParamByName("album_vid_func")->AsInteger() = album_vid_func;
    q->ParamByName("album_isbp")->AsInteger() = album_isbp;
    q->ParamByName("alb_beg_date")->AsDateTime() = EncodeDate(begGod, 1, 1);
    q->ParamByName("alb_end_date")->AsDateTime() = EncodeDate(endGod, 12, 31);
    q->ParamByName("din_izm_ids")->AsString() = subNm;
    q->ParamByName("alb_create_date")->AsDateTime() = Date();
    q->ParamByName("userctgr_idParam")->AsInteger() = userctgr_idParam;
    q->ParamByName("album_idParam")->AsInteger() = album_idParam;
    q->ExecSQL();
    int Result = TDB_Album::GetId();
   //MAK q->Free();
delete q;
    return Result;
}

// удалить группу
bool TDB_Album::DelGrp(int album_id) {
    int userctgr_id;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;
    GetItem(q, album_id);
    userctgr_id = q->FieldByName("userctgr_id")->AsInteger();
    album_id = q->FieldByName("album_id")->AsInteger();
    q->Close();
    GetSubItemAll(q, userctgr_id, album_id);
    if (!q->EoF()) {
       //MAK q->Free();
delete q;
        result = false;
        return result;
    }
    q->Close();
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Album");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
    return result;
}

// добавить альбом
int TDB_Album::AddAlbum(std::string album_obozn, int userctgr_id, int owner_id,
                        int album_vid_func, unsigned char album_isbp, int plan_id, int readonly, int begGod,
                        int endGod, int plan_id_fiks, int vid_deistv) {
    int MaxOrder;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    MaxOrder = TDB_Album::GetMaxOrder(owner_id, userctgr_id);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Album");
    q->SQL->Add("(album_obozn, userctgr_id, owner_id, order_nr, album_typ, album_vid_func, album_isbp, plan_id, readonly, ");
    q->SQL->Add("alb_beg_date, alb_end_date, alb_create_date, plan_id_fiks, vid_deistv)");
    q->SQL->Add("VALUES (:album_obozn, :userctgr_id, :owner_id, :order_nr, 1, :album_vid_func, :album_isbp, :plan_id, :readonly, ");
    q->SQL->Add(":alb_beg_date, :alb_end_date, :alb_create_date, :plan_id_fiks, :vid_deistv)");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("order_nr")->AsInteger() = MaxOrder + 1;
    q->ParamByName("album_vid_func")->AsInteger() = album_vid_func;
    q->ParamByName("album_isbp")->AsInteger() = album_isbp;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("alb_beg_date")->AsDateTime() = EncodeDate(begGod, 1, 1);
    q->ParamByName("alb_end_date")->AsDateTime() = EncodeDate(endGod, 12, 31);
    q->ParamByName("alb_create_date")->AsDateTime() = Date();
    q->ParamByName("plan_id_fiks")->AsInteger() = plan_id_fiks;
    q->ParamByName("vid_deistv")->AsInteger() = vid_deistv;
    q->ExecSQL();
   //MAK q->Free();
delete q;
    return TDB_Album::GetId();
}

// выбрать все подчиненные элементы
void TDB_Album::GetSubItemAll(TQueryUni *qIn, int userctgr_id, int owner_id) {
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Album");
    qIn->SQL->Add("WHERE (owner_id = :owner_id");
    if (owner_id == 0)
        qIn->SQL->Add("OR owner_id IS NULL)");
    else
        qIn->SQL->Add(")");
    qIn->SQL->Add("AND userctgr_id = :userctgr_id");
    qIn->SQL->Add("ORDER BY order_nr");
    qIn->ParamByName("owner_id")->AsInteger() = owner_id;
    qIn->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qIn->Open();
}

// выбрать все подчиненные элементы
void TDB_Album::GetSubItemAll_1(TQueryUni *qIn, int userctgr_id, int owner_id) {
    bool isB;
    int old_id;
    isB = TDB_Album::NewToOld(owner_id, old_id);
    qIn->SQL->Clear();
  
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Album");
    qIn->SQL->Add("WHERE (owner_id = :owner_id");
    if (isB) {
        qIn->SQL->Add("OR owner_id = " + IntToStr(old_id));
        if (old_id == 0)
            qIn->SQL->Add("OR owner_id IS NULL");
    }
    qIn->SQL->Add(")");
    qIn->SQL->Add("AND userctgr_id = :userctgr_id");
    qIn->SQL->Add("ORDER BY order_nr");
    qIn->ParamByName("owner_id")->AsInteger() = owner_id;
    qIn->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qIn->Open();
}

// получает всю информацию про элемент по album_id
bool TDB_Album::GetItem(TQueryUni* qIn, int album_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Album");
    qIn->SQL->Add("WHERE album_id = :album_id");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->Open();

    if (qIn->EoF()) {
        return false; // Нет записей, соответствующих album_id
    }
    return true; // Запись найдена
}





//функция проверки уникальности имени при изменении
bool TDB_Album::UnikName_Izm(string album_obozn, int owner_id, int album_vid_func, int userctgr_id, int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (owner_id > 0) {
        return UnikName_Izm0(album_obozn, owner_id, userctgr_id, album_id);
    }
  
    bool result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT album_id FROM Album");
    q->SQL->Add("WHERE album_obozn = :album_obozn");
    q->SQL->Add("AND userctgr_id = :userctgr_id");
    q->SQL->Add("AND owner_id = :owner_id");
    q->SQL->Add("AND album_vid_func = :album_vid_func");
    q->SQL->Add("AND album_id <> :album_id");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("album_vid_func")->AsInteger() = album_vid_func;
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->Open();
    if (!q->EoF())
        result = false;
   //MAK q->Free();
delete q;
    return result;
}

//функция проверки уникальности имени при изменении
bool TDB_Album::UnikName_Izm0(string album_obozn, int owner_id, int userctgr_id, int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT album_id FROM Album");
    q->SQL->Add("WHERE album_obozn = :album_obozn");
    q->SQL->Add("AND userctgr_id = :userctgr_id");
    q->SQL->Add("AND owner_id = :owner_id");
    q->SQL->Add("AND album_id <> :album_id");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->Open();
    if (!q->EoF())
        result = false;
   //MAK q->Free();
delete q;
    return result;
}

// изменить альбом
void TDB_Album::UpdAlbum(int album_id, std::string album_obozn, int album_vid_func, unsigned char album_isbp, int readonly,
                         int begGod, int endGod, int plan_id_fiks, int vid_deistv) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album");
    q->SQL->Add("SET album_obozn = :album_obozn, album_vid_func = :album_vid_func, album_isbp = :album_isbp, readonly = :readonly, ");
    q->SQL->Add("alb_beg_date = :alb_beg_date, alb_end_date = :alb_end_date, plan_id_fiks = :plan_id_fiks, vid_deistv = :vid_deistv");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("album_vid_func")->AsInteger() = album_vid_func;
    q->ParamByName("album_isbp")->AsInteger() = album_isbp;
    q->ParamByName("readonly")->AsInteger() = readonly;
    q->ParamByName("alb_beg_date")->AsDateTime() = EncodeDate(begGod, 1, 1);
    q->ParamByName("alb_end_date")->AsDateTime() = EncodeDate(endGod, 12, 31);
    q->ParamByName("plan_id_fiks")->AsInteger() = plan_id_fiks;
    q->ParamByName("vid_deistv")->AsInteger() = vid_deistv;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// изменить действе с параметром
void TDB_Album::UpdDeistvParam(int album_id, string album_obozn, int vid_deistv, int begGod, int endGod,
                               int userctgr_idParam, int album_idParam, int vid_deistv_Versij) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album");
    q->SQL->Add("SET album_obozn = :album_obozn, vid_deistv = :vid_deistv, alb_beg_date = :alb_beg_date,");
    q->SQL->Add("alb_end_date = :alb_end_date, pokaz_id = :userctgr_idParam, ist_fin_id = :album_idParam, vid_deistv_Versij = :vid_deistv_Versij");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("vid_deistv")->AsInteger() = vid_deistv;
    q->ParamByName("alb_beg_date")->AsDateTime() = EncodeDate(begGod, 1, 1);
    q->ParamByName("alb_end_date")->AsDateTime() = EncodeDate(endGod, 12, 31);
    q->ParamByName("userctgr_idParam")->AsInteger() = userctgr_idParam;
    q->ParamByName("album_idParam")->AsInteger() = album_idParam;
    q->ParamByName("vid_deistv_Versij")->AsInteger() = vid_deistv_Versij;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// изменить обозначение альбом
void TDB_Album::UpdItem(int album_id, string album_obozn, int begGod, int endGod) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album");
    q->SQL->Add("SET album_obozn = :album_obozn, alb_beg_date = :alb_beg_date, alb_end_date = :alb_end_date");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("alb_beg_date")->AsDateTime() = EncodeDate(begGod, 1, 1);
    q->ParamByName("alb_end_date")->AsDateTime() = EncodeDate(endGod, 12, 31);
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// изменить действие
int TDB_Album::UpdDeistv(int album_id, std::string album_obozn, int vid_deistv, int vid_deistv_Versij,
                          int begGod, int endGod, std::string subNm) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album");
    q->SQL->Add("SET album_obozn = :album_obozn, vid_deistv = :vid_deistv, vid_deistv_Versij = :vid_deistv_Versij, ");
    q->SQL->Add("alb_beg_date = :alb_beg_date, alb_end_date = :alb_end_date, din_izm_ids = :din_izm_ids");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_obozn")->AsString() = album_obozn;
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("vid_deistv")->AsInteger() = vid_deistv;
    q->ParamByName("vid_deistv_Versij")->AsInteger() = vid_deistv_Versij;
    q->ParamByName("alb_beg_date")->AsDateTime() = EncodeDate(begGod, 1, 1);
    q->ParamByName("alb_end_date")->AsDateTime() = EncodeDate(endGod, 12, 31);
    q->ParamByName("din_izm_ids")->AsString() = subNm;
    q->ExecSQL();
   //MAK q->Free();
delete q;
    return 0;
}

// изменить владельца альбома или группы
void TDB_Album::UpdOwnerId(int userctgr_id, int album_id, int owner_id) {
    int order_nr;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    order_nr = TDB_Album::GetMaxOrder(owner_id, userctgr_id) + 1;
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album");
    q->SQL->Add("SET owner_id = :owner_id, order_nr = :order_nr");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("order_nr")->AsInteger() = order_nr;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// изменить пояснение к альбом
void TDB_Album::UpdPojsnenie(int album_id, string Pojsnenie) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album");
    q->SQL->Add("SET Pojsnenie = :Pojsnenie");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("Pojsnenie")->AsString() = Pojsnenie;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// прочитать пояснение к альбому
string TDB_Album::GetPojsnenie(int album_id) {
    string Result;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Album::GetItem(q, album_id);
    Result = q->FieldByName("Pojsnenie")->AsString();
   //MAK q->Free();
delete q;
    return Result;
}

// поменять порядковыми номерами
void TDB_Album::UpdOrders(int album_id_1, int album_id_2) {
    int order_nr_1, order_nr_2;
    bool bInTransaction = false;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Album::GetItem(q, album_id_1);
    order_nr_1 = q->FieldByName("order_nr")->AsInteger();
    q->Close();
    TDB_Album::GetItem(q, album_id_2);
    order_nr_2 = q->FieldByName("order_nr")->AsInteger();
    q->Close();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album");
    q->SQL->Add("SET order_nr = :order_nr_2");
    q->SQL->Add("WHERE album_id = :album_id_1");
    q->ParamByName("order_nr_2")->AsInteger() = order_nr_2;
    q->ParamByName("album_id_1")->AsInteger() = album_id_1;
    q->ExecSQL();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album");
    q->SQL->Add("SET order_nr = :order_nr_1");
    q->SQL->Add("WHERE album_id = :album_id_2");
    q->ParamByName("order_nr_1")->AsInteger() = order_nr_1;
    q->ParamByName("album_id_2")->AsInteger() = album_id_2;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// удалить альбом
bool TDB_Album::DelAlbum(int album_id) {
    bool Result = true;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Album::GetAlbForms(q, album_id);
    if (!q->EoF()) {
       //MAK q->Free();
delete q;
        Result = false;
        return Result;
    }
    q->Close();
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Album");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
    return Result;
}

// удалить все альбомы польз категории userctgr_id
void TDB_Album::DelAlbumAll(int userctgr_id) {
    int album_id;
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    qW->SQL->Add("SELECT * FROM Album");
    qW->SQL->Add("WHERE userctgr_id = :userctgr_id");
    qW->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qW->Open();
    while (!qW->EoF()) {
        album_id = qW->FieldByName("album_id")->AsInteger();
        TDB_Album::ExclFormAll(album_id); // исключение все формы из альбома
        qW->Next();
    }
    delete qW;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Album");
    q->SQL->Add("WHERE userctgr_id = :userctgr_id");
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// выбрать конкретную форму альбома
void TDB_Album::GetAlbForm(TQueryUni *qIn, int album_id, int form_id) {
    qIn->SQL->Clear();
  
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Album_form");
    qIn->SQL->Add("WHERE album_id = :album_id");
    qIn->SQL->Add("AND Album_form.form_id = :form_id");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->ParamByName("form_id")->AsInteger() = form_id;
    qIn->Open();
}

// получить form_order формы альбома
int TDB_Album::GetAlbFormOrder(int album_id, int form_id) {
    int Result;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Album::GetAlbForm(q, album_id, form_id);
    Result = q->FieldByName("form_order")->AsInteger();
   //MAK q->Free();
delete q;
    return Result;
}

// изменить nModulOfMetods
void TDB_Album::Upd_nModulOfMetods(int album_id, int form_id, int nModulOfMetods) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album_form");
    q->SQL->Add("SET nModulOfMetods = :nModulOfMetods");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("nModulOfMetods")->AsInteger() = nModulOfMetods;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// добавить форму к альбому
void TDB_Album::AddAlbForm(int album_id, int form_id) {
    int MaxOrder;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(form_order) FROM Album_form");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->Open();
    if (q->EoF())
        MaxOrder = 0;
    else
        MaxOrder = q->Fields[0].AsInteger();
    q->Close();
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Album_form");
    q->SQL->Add("(album_id, form_order, form_id)");
    q->SQL->Add("values (:album_id, :form_order, :form_id)");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_order")->AsInteger() = MaxOrder + 1;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// поменять номерами формы
void TDB_Album::UpdOrderForms(int album_id, int form_id_1, int form_id_2) {
    int order_nr_1, order_nr_2;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Album::GetAlbForm(q, album_id, form_id_1);
    order_nr_1 = q->FieldByName("form_order")->AsInteger();
    q->Close();
    TDB_Album::GetAlbForm(q, album_id, form_id_2);
    order_nr_2 = q->FieldByName("form_order")->AsInteger();
    q->Close();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album_form");
    q->SQL->Add("SET form_order = 10000");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id_1");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id_1")->AsInteger() = form_id_1;
    q->ExecSQL();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album_form");
    q->SQL->Add("SET form_order = :order_nr_1");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id_2");
    q->ParamByName("order_nr_1")->AsInteger() = order_nr_1;
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id_2")->AsInteger() = form_id_2;
    q->ExecSQL();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album_form");
    q->SQL->Add("SET form_order = :order_nr_2");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id_1");
    q->ParamByName("order_nr_2")->AsInteger() = order_nr_2;
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id_1")->AsInteger() = form_id_1;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

// поменять категорию альбома и его форм
void TDB_Album::UpdUserCtgr(int album_id, int userctgr_id, bool UpdTypNr) {
    bool bInTransaction = false;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TQueryUni *qTmp = new TQueryUni(DefDBconnect.Conn);
    try {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Album");
        q->SQL->Add("SET userctgr_id = :userctgr_id");
        q->SQL->Add("WHERE album_id = :album_id");
        q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->ExecSQL();

        q->SQL->Clear();
        q->SQL->Add("SELECT * FROM Album");
        q->SQL->Add("WHERE album_id = :album_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->Open();
        int album_typ = q->FieldByName("album_typ")->AsInteger();
        int owner_id = q->FieldByName("owner_id")->AsInteger();
        q->Close();

        int MaxOrder = GetMaxOrder(owner_id, userctgr_id);
        if (UpdTypNr) {
            if (album_typ != 1) {
                q->SQL->Clear();
                q->SQL->Add("UPDATE Album");
                q->SQL->Add("SET album_typ = 1, order_nr = :MaxOrder, owner_id = 0");
                q->SQL->Add("WHERE album_id = :album_id");
                q->ParamByName("album_id")->AsInteger() = album_id;
                q->ParamByName("MaxOrder")->AsInteger() = MaxOrder + 1;
                q->ExecSQL();
            }
        } else {
            q->SQL->Clear();
            q->SQL->Add("UPDATE Album");
            q->SQL->Add("SET order_nr = :MaxOrder");
            q->SQL->Add("WHERE album_id = :album_id");
            q->ParamByName("album_id")->AsInteger() = album_id;
            q->ParamByName("MaxOrder")->AsInteger() = MaxOrder + 1;
            q->ExecSQL();
        }

        qTmp->SQL->Clear();
        qTmp->SQL->Add("SELECT * FROM AlbForms");
        qTmp->SQL->Add("WHERE album_id = :album_id");
        qTmp->ParamByName("album_id")->AsInteger() = album_id;
        qTmp->Open();
        while (!qTmp->EoF()) {
            int form_id = qTmp->FieldByName("form_id")->AsInteger();
            q->SQL->Clear();
            q->SQL->Add("UPDATE Forms");
            q->SQL->Add("SET userctgr_id = :userctgr_id");
            q->SQL->Add("WHERE form_id = :form_id");
            q->ParamByName("form_id")->AsInteger() = form_id;
            q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
            q->ExecSQL();
            qTmp->Next();
        }
    } catch (...) {
        // Re-throwing to ensure any caller is aware an error occurred.
    }
        
    q->Close();
    qTmp->Close();
}


// исключение форму из альбома
void TDB_Album::ExclForm(int album_id, int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Album_form");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND album_id = :album_id");
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ExecSQL();
}

// исключение все формы из альбом
void TDB_Album::ExclFormAll(int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Album_form");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ExecSQL();
}

// сделать или отменить главную форму
void TDB_Album::MainForm(int album_id, int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Album_form");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->Open();
    if (q->FieldByName("form_is_main")->AsInteger() == 1) {
        q->Close();
        q->SQL->Clear();
        q->SQL->Add("UPDATE Album_form");
        q->SQL->Add("SET form_is_main = 0");
        q->SQL->Add("WHERE album_id = :album_id");
        q->SQL->Add("AND form_id = :form_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->ParamByName("form_id")->AsInteger() = form_id;
        q->ExecSQL();
        return;
    }
    q->Close();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album_form");
    q->SQL->Add("SET form_is_main = 0");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ExecSQL();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album_form");
    q->SQL->Add("SET form_is_main = 1");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ExecSQL();
}

// данную форму в альбоме сделать вспомогательной или отменить
void TDB_Album::VspomogForm(int album_id, int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Album_form");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->Open();
    if (q->FieldByName("form_is_main")->AsInteger() == 2) {
        q->Close();
        q->SQL->Clear();
        q->SQL->Add("UPDATE Album_form");
        q->SQL->Add("SET form_is_main = 0"); // отменили
        q->SQL->Add("WHERE album_id = :album_id");
        q->SQL->Add("AND form_id = :form_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->ParamByName("form_id")->AsInteger() = form_id;
        q->ExecSQL();
        return;
    }
    q->Close();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album_form");
    q->SQL->Add("SET form_is_main = 2");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ExecSQL();
}

// данную форму в альбоме сделать ReadOnly или отменить
void TDB_Album::ReadOnlyForm(int album_id, int form_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Album_form");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->Open();
    if (q->FieldByName("readonly")->AsInteger() == 1) {
        q->Close();
        q->SQL->Clear();
        q->SQL->Add("UPDATE Album_form");
        q->SQL->Add("SET readonly = 0"); // отменили
        q->SQL->Add("WHERE album_id = :album_id");
        q->SQL->Add("AND form_id = :form_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->ParamByName("form_id")->AsInteger() = form_id;
        q->ExecSQL();
        return;
    }
    q->Close();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album_form");
    q->SQL->Add("SET readonly = 1");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND form_id = :form_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ParamByName("form_id")->AsInteger() = form_id;
    q->ExecSQL();
}

// преобразование id при новом подход
bool TDB_Album::NewToOld(int new_id, int& old_id) {
    if (new_id == -50) {
        old_id = 0;
        return true;
    }
    if (new_id == -60) {
        old_id = 0;
        return true;
    }
    if (new_id == -40) {
        old_id = 0;
        return true;
    }
    if (new_id == -30) {
        old_id = 0;
        return true;
    }
    if (new_id == -20) {
        old_id = 0;
        return true;
    }
    if (new_id == -10) {
        old_id = 0;
        return true;
    }
    if (new_id == -12) {
        old_id = 1;
        return true;
    }
    if (new_id == -22) {
        old_id = 2;
        return true;
    }
    if (new_id == -32) {
        old_id = 3;
        return true;
    }
    if (new_id == -42) {
        old_id = 4;
        return true;
    }
    return false;
}

// есть ли в формах альбома (в описании столбца) ссылка на Выбор принятой редакции Плана
bool TDB_Album::isCol_SelectPrinjtPlans(int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT Form_str.form_id");
    q->SQL->Add("FROM Album_form, Form_str");
    q->SQL->Add("WHERE album_id = :album_id");
    q->SQL->Add("AND Form_str.form_id = Album_form.form_id");
    q->SQL->Add("AND vid_otch = 23");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->Open();
    bool result = !q->EoF();
    q->Close();
    return result;
}

void TDB_Album::UpdUstGod(int album_id, int is_set_god, int god_plus, int god_ravno) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

        q->SQL->Clear();
        q->SQL->Add("UPDATE Album");
        q->SQL->Add("SET is_set_god = :is_set_god, god_plus = :god_plus, god_ravno = :god_ravno");
        q->SQL->Add("WHERE album_id = :album_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->ParamByName("is_set_god")->AsInteger() = is_set_god;
        q->ParamByName("god_plus")->AsInteger() = god_plus;
        q->ParamByName("god_ravno")->AsInteger() = god_ravno;
        q->ExecSQL();
    
}

void TDB_Album::UpdPlanId(int album_id, int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

        q->SQL->Clear();
        q->SQL->Add("UPDATE Album");
        q->SQL->Add("SET plan_id = :plan_id");
        q->SQL->Add("WHERE album_id = :album_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ExecSQL();
    
}

// изменить порядковый номер
void TDB_Album::UpdOrderN(int album_id, int order_nr) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Album");
    q->SQL->Add("SET order_nr = :order_nr");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
   q->ParamByName("order_nr")->AsInteger() = order_nr;
    q->ExecSQL();
}

// изменить isUpdDateSave
void TDB_Album::UpdIsUpdDateSave(int album_id, int isUpdDateSave) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

        q->SQL->Clear();
        q->SQL->Add("UPDATE Album");
        q->SQL->Add("SET isUpdDateSave = :isUpdDateSave");
        q->SQL->Add("WHERE album_id = :album_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->ParamByName("isUpdDateSave")->AsInteger() = isUpdDateSave;
        q->ExecSQL();
    
}

//Получить информацию о фильтре
std::string TDB_Album::getOpenBeginSost(int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";

    q->SQL->Add("SELECT ifopenbeginsost FROM Album");
    q->SQL->Add("WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->Open();
    
    if (!q->EoF())
        result = q->FieldByName("ifopenbeginsost")->AsString();
    return result;
}

//Обновить информацию о фильтре
void TDB_Album::updOpenBeginSost(int album_id, std::string JSONString) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

        q->SQL->Add("UPDATE Album");
        q->SQL->Add("SET ifopenbeginsost = :ifopenbeginsost");
        q->SQL->Add("WHERE album_id = :album_id");
        q->ParamByName("ifopenbeginsost")->AsString() = JSONString;
       q->ParamByName("album_id")->AsInteger() = album_id;
        q->ExecSQL();
    
       //MAK q->Free();
delete q;
}

//Получить информацию о фильтре и id связанного альбома
bool TDB_Album::getOpenbeginsostAll(int album_id, std::string& JSONString, int& toAlbum_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    toAlbum_id = 0;
    JSONString = "";

        q->SQL->Add("SELECT ifopenbeginsost, ifopenbeginsost_albumid FROM Album");
        q->SQL->Add("WHERE album_id = :album_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->Open();
    if (!q->EoF()) {
        toAlbum_id = q->FieldByName("ifopenbeginsost_albumid")->AsInteger();
        JSONString = q->FieldByName("ifopenbeginsost")->AsString();
    }
       //MAK q->Free();
delete q;
    return (toAlbum_id != 0) || (!JSONString.empty());
}

//Получить фиксированный год, если такой задан
bool TDB_Album::getGod_ravno(int album_id, int& god_ravno) {
    bool Result = false;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

        q->SQL->Add("SELECT god_ravno FROM Album");
        q->SQL->Add("WHERE album_id = :album_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->Open();
    if (q->FieldByName("god_ravno")->AsInteger() != 0) {
        god_ravno = q->FieldByName("god_ravno")->AsInteger();
        Result = true;
    }
   //MAK q->Free();
delete q;
    return Result;

}

// найти в указанной пользовательской категории альбом ГГГГ: План мероприятий
// userctgr_id может быть равен 0. В этом случае ищем везде
bool TDB_Album::findAlbumPlMerpopr(int userctgr_id, std::string albumName, int& album_idPlMerpopr){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;

    if (userctgr_id == 0) {
            q->SQL->Clear();
            q->SQL->Add("SELECT album_id FROM Album");
            q->SQL->Add("WHERE album_obozn = :album_obozn");
            q->ParamByName("album_obozn")->AsString() = albumName;
            q->Open();
            if (!q->EoF()) {
                Result = true;
                album_idPlMerpopr = q->FieldByName("album_id")->AsInteger();
            }
        return Result;
    }

    TDB_Userctgr::GetAlbumUserctgr_All(q, userctgr_id);
    while (!q->EoF()) {
        if (q->FieldByName("album_obozn")->AsString() == albumName) {
            Result = true;
            album_idPlMerpopr = q->FieldByName("album_id")->AsInteger();
            break;
        }
        q->Next();
    }
    if (!Result) {
        //-=-SystFuncsLogic.MessageBoxWarning('Отсутствует альбом ' + albumName, MsgVnim);
    }
   //MAK q->Free();
delete q;
    return Result;
}








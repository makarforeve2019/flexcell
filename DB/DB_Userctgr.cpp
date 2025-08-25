#include "DB_Userctgr.h" 
#include "unDefDBconnect.h" 
#include "DB_GurnProtokol.h"
#include "../SystFuncs/SystFuncsLogic.h"


#include "../SystFuncs/SystFuncsLogic.h"

// чтение всех категорий для пользователя user_id - сортировано по наименованию категории
void TDB_Userctgr::GetUserctgrUser_All_sort(TQueryUni *qIn, int user_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM User_userctgr, Userctgr");
    qIn->SQL->Add("WHERE user_id = :user_id");
    qIn->SQL->Add("AND abstract_ctgr = :abstract_ctgr");
    qIn->SQL->Add("AND Userctgr.userctgr_id = User_userctgr.userctgr_id");
    qIn->SQL->Add("ORDER BY userctgr_obozn");
    qIn->ParamByName("user_id")->AsInteger() = user_id;
    qIn->ParamByName("abstract_ctgr")->AsInteger() = 0;
    qIn->Open();
}

// Получение содержания пользовательской категории
void TDB_Userctgr::GetAlbumUserctgr_All_byGog(TQueryUni *qIn, int userctgr_id, int God) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Album");
    qIn->SQL->Add("WHERE userctgr_id = :userctgr_id");
    qIn->SQL->Add("AND ((:God BETWEEN EXTRACT(YEAR FROM alb_beg_date) AND EXTRACT(YEAR FROM alb_end_date))");
    qIn->SQL->Add("     OR (alb_beg_date IS NULL))");
    qIn->SQL->Add("ORDER BY CASE");
    qIn->SQL->Add("WHEN owner_id IN (-50, -60, -40, -30, -20, -10) THEN 0");
    qIn->SQL->Add("WHEN owner_id = -12 THEN 1");
    qIn->SQL->Add("WHEN owner_id = -22 THEN 2");
    qIn->SQL->Add("WHEN owner_id = -32 THEN 3");
    qIn->SQL->Add("WHEN owner_id = -42 THEN 4");
    qIn->SQL->Add("ELSE owner_id END, order_nr");

    qIn->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qIn->ParamByName("God")->AsInteger() = God;
    qIn->Open();
}

void TDB_Userctgr::GetUserctgrUser_All_sort_end_date(TQueryUni *qIn, int user_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM User_userctgr, Userctgr");
    qIn->SQL->Add("WHERE user_id = :user_id");
    qIn->SQL->Add("AND Userctgr.userctgr_id = User_userctgr.userctgr_id");
    qIn->SQL->Add("ORDER BY end_date DESC");

    qIn->ParamByName("user_id")->AsInteger() = user_id;
    qIn->Open();
}

int TDB_Userctgr::GetUserUserctgrRO(int user_id, int userctgr_id) {
    int readonly = 0;
    try {
        TQueryUni q(DefDBconnect.Conn);
        q.SQL->Clear();
        q.SQL->Add("SELECT readonly FROM User_userctgr");
        q.SQL->Add("WHERE (user_id = :user_id) and (userctgr_id = :userctgr_id)");
        q.ParamByName("user_id")->AsInteger() = user_id;
        q.ParamByName("userctgr_id")->AsInteger() = userctgr_id;
        q.Open();
        int readonly = q.FieldByName("readonly")->AsInteger();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return readonly;
}









// вернуть информацию категории по user_id и userctgr_obozn
int TDB_Userctgr::GetUserctgrUserAndObozn(const std::string& userctgr_obozn, int user_id,
                                          std::string& dostupn_funcii, int& isNoNasledMenu, int& MenuVariant) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int userctgr_id = 0;
    q->SQL->Add("SELECT * FROM User_userctgr, Userctgr");
    q->SQL->Add("WHERE user_id = :user_id");
    q->SQL->Add("AND userctgr_obozn = :userctgr_obozn");
    q->SQL->Add("AND Userctgr.userctgr_id = User_userctgr.userctgr_id");
    q->SQL->Add("ORDER BY order_nr");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("userctgr_obozn")->AsString() = userctgr_obozn;
    q->Open();
    int Result = 0;
    if (!q->EoF()) {
        userctgr_id = q->FieldByName("userctgr_id")->AsInteger();
        dostupn_funcii = q->FieldByName("dostupn_funcii")->AsString();
        isNoNasledMenu = q->FieldByName("isNoNasledMenu")->AsInteger();
        MenuVariant = q->FieldByName("MenuVariant")->AsInteger();
        // Assuming Protokol_AddItem is defined somewhere
        // TDB_GurnProtokol.Protokol_AddItem('Актив-я польз-ой катег.', SystFuncsLogic.GetUserLogin, SystFuncsLogic.GetUserId,
        //                                    userctgr_id{SystFuncsLogic.GetUserctgr_id}, userctgr_obozn{SystFuncsLogic.GetUserctgr_Obozn});
        Result = userctgr_id;
    }
    q->Close();
    delete q;
    return Result;
}

// вернуть информацию о Первой категории для пользователя user_id
int TDB_Userctgr::GetUserctgrUserN1(int user_id, std::string& dostupn_funcii, std::string& userctgr_obozn,
                                    int& isNoNasledMenu, int& MenuVariant) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int userctgr_id = 0;
    q->SQL->Add("SELECT * FROM User_userctgr, Userctgr");
    q->SQL->Add("WHERE user_id = :user_id");
    q->SQL->Add("AND Userctgr.userctgr_id = User_userctgr.userctgr_id");
    q->SQL->Add("ORDER BY order_nr");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->Open();
    int Result = 0;
    if (!q->EoF()) {
        userctgr_id = q->FieldByName("userctgr_id")->AsInteger();
        dostupn_funcii = q->FieldByName("dostupn_funcii")->AsString();
        userctgr_obozn = q->FieldByName("userctgr_obozn")->AsString();
        isNoNasledMenu = q->FieldByName("isNoNasledMenu")->AsInteger();
        MenuVariant = q->FieldByName("MenuVariant")->AsInteger();
        // Assuming Protokol_AddItem is defined somewhere
        // TDB_GurnProtokol.Protokol_AddItem('Актив-я польз-ой катег.', SystFuncsLogic.GetUserLogin, SystFuncsLogic.GetUserId,
        //                                    userctgr_id{SystFuncsLogic.GetUserctgr_id}, userctgr_obozn{SystFuncsLogic.GetUserctgr_Obozn});
        Result = userctgr_id;
    }
    q->Close();
    delete q;
    return Result;
}

// вернуть информацию о пользовательской пользователя user_id
void TDB_Userctgr::GetUserCtgrInfo(int userctgr_id, std::string& dostupn_funcii, std::string& userctgr_obozn,
                                   int& isNoNasledMenu, int& MenuVariant) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Userctgr::GetUserctgr(q, userctgr_id);
    dostupn_funcii = q->FieldByName("dostupn_funcii")->AsString();
    userctgr_obozn = q->FieldByName("userctgr_obozn")->AsString();
    isNoNasledMenu = q->FieldByName("isNoNasledMenu")->AsInteger();
    MenuVariant = q->FieldByName("MenuVariant")->AsInteger();
    delete q;
    // Assuming Protokol_AddItem is defined somewhere
    // TDB_GurnProtokol.Protokol_AddItem('Актив-я польз-ой катег.', SystFuncsLogic.GetUserLogin, SystFuncsLogic.GetUserId,
    //                                    userctgr_id{SystFuncsLogic.GetUserctgr_id}, userctgr_obozn{SystFuncsLogic.GetUserctgr_Obozn});
}

//Вернуть обозначение польз. катиегории
std::string TDB_Userctgr::GetUserCtgrObozn(int userctgr_id) {
    std::string Result = "";
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Userctgr::GetUserctgr(q, userctgr_id);
    Result = q->FieldByName("userctgr_obozn")->AsString();
    delete q;
    return Result;
}

// заполнить TStrings - все категорий для пользователя user_id - сортировано по наименованию категории
// возвращает количество каткгориф у данного пользователя, которые прошлогодние или еще старше
int TDB_Userctgr::LoadUserctgrUser_All_sort(TStringList* Items, int user_id) {
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    TDB_Userctgr::GetUserctgrUser_All_sort(qW, user_id);
    int count = 0;
    while (!qW->EoF()) {
        if (qW->FieldByName("end_date")->AsInteger() >= CurrentYear())
          Items->AddObject(qW->FieldByName("userctgr_obozn")->AsString(),qW->FieldByName("userctgr_id")->AsInteger());

        else
            count++;
        qW->Next();
    }
    delete qW;
    return count;
}

// чтение всех категорий для пользователя user_id - сортировано по имени
//void TDB_Userctgr::GetUserctgrUser_All_sort(TQueryUni* qIn, int user_id) {
//    qIn->SQL->Clear();
//    qIn->SQL->Add("SELECT * FROM User_userctgr, Userctgr");
//    qIn->SQL->Add("WHERE user_id = :user_id");
//    qIn->SQL->Add("AND abstract_ctgr = :abstract_ctgr");
//    qIn->SQL->Add("AND Userctgr.userctgr_id = User_userctgr.userctgr_id");
//    qIn->SQL->Add("ORDER BY userctgr_obozn");
//    qIn->ParamByName("user_id")->AsInteger() = user_id;
//    qIn->ParamByName("abstract_ctgr")->AsInteger() = 0;
//    qIn->Open();
//}

// чтение всех категорий для пользователя user_id - сортировано по году end_date
//void TDB_Userctgr::GetUserctgrUser_All_sort_end_date(TQueryUni* qIn, int user_id) {
//    qIn->SQL->Clear();
//    qIn->SQL->Add("SELECT * FROM User_userctgr, Userctgr");
//    qIn->SQL->Add("WHERE user_id = :user_id");
//    qIn->SQL->Add("AND Userctgr.userctgr_id = User_userctgr.userctgr_id");
//    qIn->SQL->Add("ORDER BY end_date DESC");
//    qIn->ParamByName("user_id")->AsInteger() = user_id;
//    qIn->Open();
//}

// чтение всех альбомов, использующих категорию userctgr_id, но с ограничением на год
//void TDB_Userctgr::GetAlbumUserctgr_All_byGog(TQueryUni* qIn, int userctgr_id, int God) {
//    qIn->SQL->Clear();
//    qIn->SQL->Add("SELECT * FROM Album");
//    qIn->SQL->Add("WHERE userctgr_id = :userctgr_id");
//  switch (GetDBType()) {
//      case TDBType::MSSQL:
//            qIn->SQL->Add("AND ((:God BETWEEN YEAR(alb_beg_date) AND YEAR(alb_end_date))");
//            break;
//        case TDBType::Postgre:
//            qIn->SQL->Add("AND ((:God BETWEEN date_part(''year'', alb_beg_date) AND date_part(''year'', alb_end_date))");
//            break;
//    }
//
//    qIn->SQL->Add("     OR (alb_beg_date IS NULL))");
//    qIn->SQL->Add("ORDER BY CASE WHEN owner_id IN (-50, -60, -40, -30, -20, -10) THEN 0 WHEN owner_id = -12 THEN 1 ");
//    qIn->SQL->Add("WHEN owner_id = -22 THEN 2 WHEN owner_id = -32 THEN 3 WHEN owner_id = -42 THEN 4 ELSE owner_id END, order_nr");
//    qIn->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
//    qIn->ParamByName("God")->AsInteger() = God;
//    qIn->Open();
//}

// чтение всех альбомов, использующих категорию userctgr_id
void TDB_Userctgr::GetAlbumUserctgr_All(TQueryUni* qIn, int userctgr_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Album");
    qIn->SQL->Add("WHERE userctgr_id = :userctgr_id");
    qIn->SQL->Add("ORDER BY CASE WHEN owner_id IN (-50, -60, -40, -30, -20, -10) THEN 0 WHEN owner_id = -12 THEN 1 ");
    qIn->SQL->Add("WHEN owner_id = -22 THEN 2 WHEN owner_id = -32 THEN 3 WHEN owner_id = -42 THEN 4 ELSE owner_id END, order_nr");
    qIn->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qIn->Open();
}

// возвращает id последней добавленной записи
int TDB_Userctgr::GetId() {
    int Result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as userctgr_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Userctgr', 'userctgr_id')) as userctgr_id");
            break;
    }
    q->Open();
    Result = q->FieldByName("userctgr_id")->AsInteger();
    delete q;
    return Result;
}

// чтение всех пользовательских категорий
void TDB_Userctgr::GetUserctgr_All(TQueryUni* qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Userctgr");
    qIn->SQL->Add("ORDER BY userctgr_obozn");
    qIn->Open();
}

void TDB_Userctgr::GetUserctgr_Allabstr(TQueryUni* qIn, int abstract_ctgr) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Userctgr");
    qIn->SQL->Add("WHERE abstract_ctgr = :abstract_ctgr");
    qIn->SQL->Add("ORDER BY userctgr_obozn");
    qIn->ParamByName("abstract_ctgr")->AsInteger() = abstract_ctgr;
    qIn->Open();
}

// чтение всех категорий для пользователя user_id
void TDB_Userctgr::GetUserctgrUser_All(TQueryUni* qIn, int user_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM User_userctgr, Userctgr");
    qIn->SQL->Add("WHERE user_id = :user_id");
    qIn->SQL->Add("AND Userctgr.userctgr_id = User_userctgr.userctgr_id");
    qIn->SQL->Add("ORDER BY order_nr");
    qIn->ParamByName("user_id")->AsInteger() = user_id;
    qIn->Open();
}

// добавление новой записи возвращает id новой записи
int TDB_Userctgr::AddUserctgr(std::string userctgr_obozn, std::string comment, std::string dostupn_funcii,
    int isNoNasledMenu, int MenuVariant, int beg_date, int end_date, int abstract_ctgr) {
    int Result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Userctgr");
    q->SQL->Add("(userctgr_obozn, comment, dostupn_funcii, isNoNasledMenu, MenuVariant, beg_date, end_date, abstract_ctgr)");
    q->SQL->Add("values (:userctgr_obozn, :comment, :dostupn_funcii, :isNoNasledMenu, :MenuVariant, :beg_date, :end_date, :abstract_ctgr)");
    q->ParamByName("userctgr_obozn")->AsString() = userctgr_obozn;
    q->ParamByName("comment")->AsString() = comment;
    q->ParamByName("dostupn_funcii")->AsString() = dostupn_funcii;
    q->ParamByName("isNoNasledMenu")->AsInteger() = isNoNasledMenu;
    q->ParamByName("MenuVariant")->AsInteger() = MenuVariant;
    q->ParamByName("beg_date")->AsInteger() = beg_date;
    q->ParamByName("end_date")->AsInteger() = end_date;
    q->ParamByName("abstract_ctgr")->AsInteger() = abstract_ctgr;
    q->ExecSQL();
    delete q;
    Result = TDB_Userctgr::GetId();
    return Result;
}

// изменение записи для userctgr_id
void TDB_Userctgr::UpdUserctgr(int userctgr_id, std::string userctgr_obozn, std::string comment, std::string dostupn_funcii,
    int isNoNasledMenu, int MenuVariant, int beg_date, int end_date, int abstract_ctgr) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Userctgr");
    q->SQL->Add("SET userctgr_obozn = :userctgr_obozn, comment = :comment, dostupn_funcii = :dostupn_funcii, ");
    q->SQL->Add("MenuVariant = :MenuVariant, isNoNasledMenu = :isNoNasledMenu, beg_date = :beg_date, end_date = :end_date, abstract_ctgr = :abstract_ctgr");
    q->SQL->Add("WHERE userctgr_id = :userctgr_id");
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("userctgr_obozn")->AsString() = userctgr_obozn;
    q->ParamByName("comment")->AsString() = comment;
    q->ParamByName("dostupn_funcii")->AsString() = dostupn_funcii;
    q->ParamByName("isNoNasledMenu")->AsInteger() = isNoNasledMenu;
    q->ParamByName("MenuVariant")->AsInteger() = MenuVariant;
    q->ParamByName("beg_date")->AsInteger() = beg_date;
    q->ParamByName("end_date")->AsInteger() = end_date;
    q->ParamByName("abstract_ctgr")->AsInteger() = abstract_ctgr;
    q->ExecSQL();
    delete q;
}

// изменение только ReadOnly для user_id + userctgr_id
void TDB_Userctgr::UpdUserUserctgrRO(int user_id, int userctgr_id, int ReadOnly) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE User_userctgr");
    q->SQL->Add("SET readonly = :readonly");
    q->SQL->Add("WHERE (user_id = :user_id) and (userctgr_id = :userctgr_id)");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("readonly")->AsInteger() = ReadOnly;
    q->ExecSQL();
    delete q;
}

// чтение свойства только ReadOnly user_id + userctgr_id
//int TDB_Userctgr::GetUserUserctgrRO(int user_id, int userctgr_id) {
//    int Result = 0;
//    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
//    q->SQL->Clear();
//    q->SQL->Add("SELECT readonly FROM User_userctgr");
//    q->SQL->Add("WHERE (user_id = :user_id) and (userctgr_id = :userctgr_id)");
//    q->ParamByName("user_id")->AsInteger() = user_id;
//    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
//    q->Open();
//    Result = q->FieldByName("readonly")->AsInteger();
//    delete q;
//    return Result;
//}

// чтение свойства isNoNasledMenu из категории userctgr_id
int TDB_Userctgr::GetIsNoNasledMenu(int userctgr_id) {
    int Result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Userctgr");
    q->SQL->Add("WHERE userctgr_id = :userctgr_id");
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->Open();
    Result = q->FieldByName("isNoNasledMenu")->AsInteger();
    delete q;
    return Result;
}

// чтение свойства dostupn_funcii из категории userctgr_id
std::string TDB_Userctgr::GetDostupn_funcii(int userctgr_id, int& isNoNasledMenu) {
    std::string Result = "";
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Userctgr");
    q->SQL->Add("WHERE userctgr_id = :userctgr_id");
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->Open();
    Result = q->FieldByName("dostupn_funcii")->AsString();
    isNoNasledMenu = q->FieldByName("isNoNasledMenu")->AsInteger();
    delete q;
    return Result;
}

// чтение пользовательского категория по userctgr_id
bool TDB_Userctgr::GetUserctgr(TQueryUni* qIn, int userctgr_id) {
    bool Result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Userctgr");
    qIn->SQL->Add("WHERE userctgr_id = :userctgr_id");
    qIn->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qIn->Open();
    if (qIn->EoF()) {
        Result = false;
    }
    return Result;
}

//функция проверки уникальности имени при добавлении
bool TDB_Userctgr::UnikName(std::string userctgr_obozn) {
    bool Result = true;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT userctgr_id FROM Userctgr");
    q->SQL->Add("WHERE userctgr_obozn = :userctgr_obozn");
    q->ParamByName("userctgr_obozn")->AsString() = userctgr_obozn;
    q->Open();
    if (!q->EoF()) {
        Result = false;
    }
    delete q;
    return Result;
}

//функция проверки уникальности имени при изменении
bool TDB_Userctgr::UnikName_Izm(std::string userctgr_obozn, int userctgr_id) {
    bool Result = true;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT userctgr_id FROM Userctgr");
    q->SQL->Add("WHERE userctgr_obozn = :userctgr_obozn");
    q->SQL->Add("AND userctgr_id <> :userctgr_id");
    q->ParamByName("userctgr_obozn")->AsString() = userctgr_obozn;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->Open();
    if (!q->EoF()) {
        Result = false;
    }
    delete q;
    return Result;
}

// чтение всех пользователей, использующих категорию userctgr_id
void TDB_Userctgr::GetUserUserctgr_All(TQueryUni* qIn, int userctgr_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM User_userctgr, Users");
    qIn->SQL->Add("WHERE userctgr_id = :userctgr_id");
    qIn->SQL->Add("AND Users.user_id = User_userctgr.user_id");
    qIn->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    qIn->Open();
}

// удаление пользовательской категорий
void TDB_Userctgr::DelUserctgr(int userctgr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Userctgr");
    q->SQL->Add("WHERE userctgr_id = :userctgr_id");
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ExecSQL();
    delete q;
}

// чтение всех категорий для всех пользователей - сортировано по категории или пользователю
// в зависимости от параметра sortByCtgr
void TDB_Userctgr::GetUserCtgrsForUsers(TQueryUni* qIn, bool sortByCtgr, int abstractCtgr) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM User_userctgr, Userctgr, Users");
    qIn->SQL->Add("WHERE User_userctgr.user_id = Users.user_id");
    qIn->SQL->Add("AND Userctgr.userctgr_id = User_userctgr.userctgr_id");
    qIn->SQL->Add("AND abstract_ctgr = :abstract_ctgr");
    qIn->ParamByName("abstract_ctgr")->AsInteger() = abstractCtgr;
    if (sortByCtgr) {
        qIn->SQL->Add("ORDER BY userctgr_obozn, user_name");
    } else {
        qIn->SQL->Add("ORDER BY user_name, userctgr_obozn");
    }
    qIn->Open();
}

// удаление всех категорий для пользователя user_id
void TDB_Userctgr::DelUserctgrUser_All(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM User_userctgr");
    q->SQL->Add("WHERE user_id = :user_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ExecSQL();
    delete q;
}

// удаление категории userctgr_id для всех пользователя
void TDB_Userctgr::DelUserctgrUser_AllUsers(int userctgr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM User_userctgr");
    q->SQL->Add("WHERE userctgr_id = :userctgr_id");
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ExecSQL();
    delete q;
}

// добавление пользовательского категория для пользователя user_id
void TDB_Userctgr::AddUserctgrUser(int user_id, int userctgr_id, int order_nr) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO User_userctgr");
    q->SQL->Add("(user_id, userctgr_id, order_nr)");
    q->SQL->Add("values (:user_id, :userctgr_id, :order_nr)");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("order_nr")->AsInteger() = order_nr;
    q->ExecSQL();
    delete q;
}

void TDB_Userctgr::UpdOrder_nr(int user_id, int userctgr_id, int order_nr) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE User_userctgr");
    q->SQL->Add("SET order_nr = :order_nr");
    q->SQL->Add("WHERE (user_id = :user_id) and (userctgr_id = :userctgr_id)");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("order_nr")->AsInteger() = order_nr;
    q->ExecSQL();
    delete q;
}

// изменение свойства только IsBlock user_id + userctgr_id
void TDB_Userctgr::UpdUserUserctgrIsBlock(int user_id, int userctgr_id, int is_block) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE User_userctgr");
    q->SQL->Add("SET is_block = :is_block");
    q->SQL->Add("WHERE (user_id = :user_id) and (userctgr_id = :userctgr_id)");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("is_block")->AsInteger() = is_block;
    q->ExecSQL();
    delete q;
}

// чтение свойства только IsBlock user_id + userctgr_id
int TDB_Userctgr::GetUserUserctgrIsBlock(int user_id, int userctgr_id) {
    int Result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT is_block FROM User_userctgr");
    q->SQL->Add("WHERE (user_id = :user_id) and (userctgr_id = :userctgr_id)");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->Open();
    Result = q->FieldByName("is_block")->AsInteger();
    delete q;
    return Result;
}

// изменение свойства только projected user_id + userctgr_id
void TDB_Userctgr::UpdUserUserctgrProjected(int user_id, int userctgr_id, int Projected) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE User_userctgr");
    q->SQL->Add("SET projected = :projected");
    q->SQL->Add("WHERE (user_id = :user_id) and (userctgr_id = :userctgr_id)");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("projected")->AsInteger() = Projected;
    q->ExecSQL();
    delete q;
}

// чтение свойства только projected user_id + userctgr_id
int TDB_Userctgr::GetUserUserctgrProjected(int user_id, int userctgr_id) {
    int Result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT projected FROM User_userctgr");
    q->SQL->Add("WHERE (user_id = :user_id) and (userctgr_id = :userctgr_id)");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->Open();
    Result = q->FieldByName("projected")->AsInteger();
    delete q;
    return Result;
}

// выделить N-й байт справа (нумерация с 1)
int TDB_Userctgr::getUserCtgrId_N(int isNoNasledMenu, int N) {
    std::string ws = IntToHex(isNoNasledMenu);
    ws = ws.substr(ws.length() - (N * 2), 2);
    return std::stoi(ws, nullptr, 16);
}

// положить info в N-й байт справа и вернуть (нумерация с 1)
int TDB_Userctgr::setUserCtgrId_N(int info, int isNoNasledMenu, int N) {
    std::string ws = IntToHex(isNoNasledMenu);
    std::string wsB = IntToHex(info);
    ws[ws.length() - (N * 2)] = wsB[0];
    ws[ws.length() - (N * 2) + 1] = wsB[1];
    return std::stoi(ws, nullptr, 16);
}





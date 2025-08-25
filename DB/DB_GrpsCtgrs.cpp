#include "DB_GrpsCtgrs.h"
#include "../SystFuncs/SystFuncsLogic.h"

// вернуть состояние чтения/редактирования при формировании проекта ПСЭР
int TDB_GrpsCtgrs::getIsReadOnly_GrpsCtgrs(int user_id, int userctgr_id, int god) {
    int Result = 0;
    try {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        q->SQL->Clear();
        q->SQL->Add("SELECT isReadOnly FROM GrpsCtgrs");
        q->SQL->Add("WHERE god = :god");
        q->SQL->Add("AND user_id = :user_id");
        q->SQL->Add("AND userctgr_id = :userctgr_id");
        q->ParamByName("god")->AsInteger() = god;
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
        q->Open();
        if (!q->EoF())
            Result = q->FieldByName("isReadOnly")->AsInteger();
       //MAK q->Free();
delete q;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return Result;
}

//// вернуть состояние чтения/редактирования при формировании проекта ПСЭР
//int TDB_GrpsCtgrs::getIsReadOnly_GrpsCtgrs(int user_id, int userctgr_id, int god) {
//    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
//    int Result = 0;
//    q->SQL->Clear();
//    q->SQL->Add("SELECT isReadOnly FROM GrpsCtgrs");
//    q->SQL->Add("WHERE god = :god");
//    q->SQL->Add("AND user_id = :user_id");
//    q->SQL->Add("AND userctgr_id = :userctgr_id");
//    q->ParamByName("god")->AsInteger() = god;
//    q->ParamByName("user_id")->AsInteger() = user_id;
//    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
//    q->Open();
//    if (!q->EoF())
//        Result = q->FieldByName("isReadOnly")->AsInteger();
//    delete q;
//    return Result;
//}

// возвращает grct_id добавленной записи
int TDB_GrpsCtgrs::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result;
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as grct_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('GrpsCtgrs', 'grct_id')) as grct_id");
            break;
    }
    q->Open();
    Result = q->FieldByName("grct_id")->AsInteger();
    delete q;
    return Result;
}

// Проверить, нет ли такого планируемого года
bool TDB_GrpsCtgrs::IsPlanirGod(int God) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT grct_id FROM GrpsCtgrs");
    q->SQL->Add("WHERE god = :god");
    q->SQL->Add("AND vidGrp = 1");
    q->SQL->Add("AND user_id  = 0");
    q->SQL->Add("AND userctgr_id  = 0");
    q->ParamByName("god")->AsInteger() = God;
    q->Open();
    if (!q->EoF())
        Result = true;
    delete q;
    return Result;
}

// добавить новый планируемый год
int TDB_GrpsCtgrs::AddPlanirGod(int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO GrpsCtgrs");
    q->SQL->Add("(user_id, userctgr_id, vidGrp, god, date_create)");
    q->SQL->Add("values (0, 0, 1, :god, :date_create)");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("date_create")->AsDateTime() = Date();
    q->ExecSQL();
    int Result = TDB_GrpsCtgrs::GetId();
    delete q;
    return Result;
}

// добавить нового разработчика
int TDB_GrpsCtgrs::AddRazrabotcik(int god, int user_id, int userctgr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO GrpsCtgrs");
    q->SQL->Add("(user_id, userctgr_id, vidGrp, god, date_create)");
    q->SQL->Add("values (:user_id, :userctgr_id, 1, :god, :date_create)");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("userctgr_id")->AsInteger() = userctgr_id;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("date_create")->AsDateTime() = Date();
    q->ExecSQL();
    int Result = TDB_GrpsCtgrs::GetId();
    delete q;
    return Result;
}

// чтение всех строк для вида 1
void TDB_GrpsCtgrs::GetAll1(TQueryUni* qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM GrpsCtgrs");
    qIn->SQL->Add("WHERE vidGrp = 1");
    qIn->SQL->Add("AND user_id = 0");
    qIn->SQL->Add("ORDER BY god DESC");
    qIn->Open();
}

// чтение для grct_id - это заголоаок, здесь user_id = 0
void TDB_GrpsCtgrs::GetForGrctId(TQueryUni* qIn, int grct_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM GrpsCtgrs");
    qIn->SQL->Add("WHERE grct_id = :grct_id");
    qIn->ParamByName("grct_id")->AsInteger() = grct_id;
    qIn->Open();
}

// чтение для god
void TDB_GrpsCtgrs::GetForGod(TQueryUni* qIn, int god) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM GrpsCtgrs, Users, Userctgr");
    qIn->SQL->Add("WHERE god = :god");
    qIn->SQL->Add("AND GrpsCtgrs.user_id > 0");
    qIn->SQL->Add("AND Users.user_id = GrpsCtgrs.user_id");
    qIn->SQL->Add("AND Userctgr.userctgr_id = GrpsCtgrs.userctgr_id");
    qIn->SQL->Add("ORDER BY user_name");
    qIn->ParamByName("god")->AsInteger() = god;
    qIn->Open();
}

// удаление строки
void TDB_GrpsCtgrs::DelItem(int grct_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM GrpsCtgrs");
    q->SQL->Add("WHERE grct_id = :grct_id");
    q->ParamByName("grct_id")->AsInteger() = grct_id;
    q->ExecSQL();
    delete q;
}

// изменить признак редактирования
void TDB_GrpsCtgrs::UpdReadOnly(int grct_id, int isReadOnly) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE GrpsCtgrs");
    q->SQL->Add("SET isReadOnly = :isReadOnly");
    q->SQL->Add("WHERE grct_id = :grct_id");
    q->ParamByName("isReadOnly")->AsInteger() = isReadOnly;
    q->ParamByName("grct_id")->AsInteger() = grct_id;
    q->ExecSQL();
    delete q;
}

// изменить docimage_id
void TDB_GrpsCtgrs::UpdDocimageId(int grct_id, int docimage_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE GrpsCtgrs");
    q->SQL->Add("SET docimage_id = :docimage_id");
    q->SQL->Add("WHERE grct_id = :grct_id");
    q->ParamByName("docimage_id")->AsInteger() = docimage_id;
    q->ParamByName("grct_id")->AsInteger() = grct_id;
    q->ExecSQL();
    delete q;
}

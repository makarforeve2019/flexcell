
#include <iostream>
#include "unDefDBconnect.h"
#include "DB_AccessAlbum.h"

int TDB_AccessAlbum::GetIdAccessAlbum(int album_id, int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT accessalbum_id FROM AccessAlbum WHERE album_id = :album_id AND plan_id = :plan_id");
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->Open();
        if (!q->EoF()) {
            result = q->FieldByName("accessalbum_id")->AsInteger();
        }
        q->Close();
    } catch (const std::exception& e) {
        result = -1;
    }
   //MAK q->Free();
delete q;
    return result;
}

//void TDB_AccessAlbum::GetAccessAlbum(TQueryUni* qIn, int album_id) {
//    qIn->SQL->Add("SELECT accessalbum_id, album_id, god, plan_id FROM AccessAlbum WHERE album_id = :album_id");
//    qIn->ParamByName("album_id")->AsInteger() = album_id;
//    qIn->Open();
//
//    if (qIn->EoF())
//        return;
//
//    if (qIn->FieldByName("plan_id")->AsInteger() < 0)
//        return;
//
//    qIn->SQL->Clear();
//    qIn->SQL->Add("SELECT Plans.*, accessalbum_id, album_id, god FROM AccessAlbum, Plans WHERE album_id = :album_id AND AccessAlbum.plan_id = Plans.plan_id ORDER BY plan_order");
//    qIn->ParamByName("album_id")->AsInteger() = album_id;
//    qIn->Open();
//}

void TDB_AccessAlbum::GetAccessAlbum(TQueryUni *qIn, int album_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT accessalbum_id, album_id, god, plan_id");
    qIn->SQL->Add("FROM AccessAlbum");
    qIn->SQL->Add("WHERE album_id = :album_id");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->Open();

    if (qIn->EoF())
        return;

    if (qIn->FieldByName("plan_id")->AsInteger() < 0)
        return;

    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT Plans.*, accessalbum_id, album_id, god");
    qIn->SQL->Add("FROM AccessAlbum, Plans");
    qIn->SQL->Add("WHERE album_id = :album_id");
    qIn->SQL->Add("AND AccessAlbum.plan_id = Plans.plan_id");
    qIn->SQL->Add("ORDER BY plan_order");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->Open();
}

void TDB_AccessAlbum::GetAccessAlbumById(TQueryUni* qIn, int accessalbum_id) {
    qIn->SQL->Add("SELECT accessalbum_id, album_id, god, plan_id FROM AccessAlbum WHERE accessalbum_id = :accessalbum_id");
    qIn->ParamByName("accessalbum_id")->AsInteger() = accessalbum_id;
    qIn->Open();
}

void TDB_AccessAlbum::GetAccessAlbumForUser(TQueryUni* qIn, int album_id, int user_id) {
    qIn->SQL->Clear();

    qIn->SQL->Add("SELECT accessalbum_id, album_id, god, plan_id FROM AccessAlbum WHERE album_id = :album_id");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->Open();

    if (qIn->EoF())
        return;

    if ( qIn->FieldByName("plan_id")->AsInteger() < 0)
        album_id = -qIn->FieldByName("plan_id")->AsInteger();

    qIn->Close();
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT Plans.*, accessalbum_id, album_id, god, user_perm FROM AccessAlbum, Plans, User_plan WHERE album_id = :album_id AND AccessAlbum.plan_id = Plans.plan_id AND User_plan.user_id = :idUser AND User_plan.plan_id = Plans.plan_id ORDER BY plan_order");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->ParamByName("idUser")->AsInteger() = user_id;
    qIn->Open();
}

void TDB_AccessAlbum::GetAccessBlockAlbumForUser(TQueryUni* qIn, int album_id) {
    qIn->SQL->Clear();

    qIn->SQL->Add("SELECT accessalbum_id, Al.album_id, Al.album_obozn FROM AccessAlbum AcA, Album Al WHERE AcA.album_id = :album_id AND (-AcA.plan_id) = Al.album_id");
    qIn->ParamByName("album_id")->AsInteger() = album_id;
    qIn->Open();
}

int TDB_AccessAlbum::AddAccessAlbum(int album_id, int plan_id, int god, TDateTime date_create) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    try {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO AccessAlbum (god, plan_id, album_id, date_create) VALUES (:god, :plan_id, :album_id, :date_create)");
        q->ParamByName("god")->AsInteger() = god;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("album_id")->AsInteger() = album_id;
        q->ParamByName("date_create")->AsDateTime() = date_create;
        q->ExecSQL();
        result = TDB_AccessAlbum::GetIdAccessAlbum(album_id, plan_id);
    } catch (const std::exception& e) {
        result = -1;
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_AccessAlbum::DelAccessAlbum(int accessalbum_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM AccessAlbum WHERE accessalbum_id = :accessalbum_id");
    q->ParamByName("accessalbum_id")->AsInteger() = accessalbum_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_AccessAlbum::DelAccessAlbumAll(int album_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM AccessAlbum WHERE album_id = :album_id");
    q->ParamByName("album_id")->AsInteger() = album_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}
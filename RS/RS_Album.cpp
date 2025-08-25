//
// Created by Alex on 15.04.2024.
//

#include "RS_Album.h"
#include "../DB/DB_Album.h"
#include "../DB/DB_AccessAlbum.h"
#include <../nlohmann/json.hpp>
#include "../DB/unDefDBconnect.h"

using json = nlohmann::json;

std::string Album::GetAlbumInfoYQM(int album_id) {
    // Предварительные объявления переменных для хранения результатов
    int album_vid_func, album_isbp, readOnly, plan_id_fiks, vid_deistv, album_typ, isYearQuartMonth, YearFixed, isUpdDateSave;
    std::string album_obozn, JSONFiltr;

    // Вызов функции для получения информации об альбоме
    TDB_Album::GetAlbumInfoYQM(album_id, album_vid_func, album_isbp, readOnly, album_obozn,
                               plan_id_fiks, vid_deistv, album_typ, isYearQuartMonth, YearFixed, JSONFiltr);
    TDB_Album::GetAlbumInfo2(album_id, isUpdDateSave);

    // Создание JSON объекта для формирования ответа
    json jsonObj;
    jsonObj["album_isbp"] = album_isbp;
    jsonObj["album_obozn"] = album_obozn;
    jsonObj["album_typ"] = album_typ;
    jsonObj["album_vid_func"] = album_vid_func;
    jsonObj["isYearQuartMonth"] = isYearQuartMonth;
    jsonObj["JSONFiltr"] = JSONFiltr;
    jsonObj["plan_id_fiks"] = plan_id_fiks;
    jsonObj["readOnly"] = readOnly;
    jsonObj["vid_deistv"] = vid_deistv;
    jsonObj["YearFixed"] = YearFixed;
    jsonObj["isUpdDateSave"] = isUpdDateSave;

    // Конвертация JSON объекта в строку
    std::string result = jsonObj.dump();

    return result;
}

std::string Album::GetAlbForms(int album_id) {
    std::string result = "Not Result";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_Album::GetAlbForms(&qTmp, album_id);
        json root;
        json contentArray = json::array();

        while (!qTmp.EoF()) {
            json formObject;
            formObject["form_id"] = qTmp.FieldByName("form_id")->AsInteger();
            formObject["form_is_main"] = qTmp.FieldByName("form_is_main")->AsInteger();
            formObject["form_obozn"] = qTmp.FieldByName("form_obozn")->AsString();
            formObject["vid_form"] = qTmp.FieldByName("vid_form")->AsInteger();
            formObject["form_order"] = qTmp.FieldByName("form_order")->AsInteger();
            formObject["nmodulofmetods"] = qTmp.FieldByName("nmodulofmetods")->AsInteger();
            formObject["readonly"] = qTmp.FieldByName("readonly")->AsInteger();
            bool b = (qTmp.FieldByName("for_class")->AsInteger()) && (qTmp.FieldByName("pokaz_id")->AsInteger());
            formObject["b"] = b;
            //formObject["form_beg_date"] = qTmp.FieldByName("form_beg_date")->AsDateTime().ToUnixStamp();
            //formObject["form_end_date"] = qTmp.FieldByName("form_end_date")->AsDateTime().ToUnixStamp();
            //formObject["form_create_date"] = qTmp.FieldByName("form_create_date")->AsDateTime().ToUnixStamp();
            formObject["pokaz_id"] = qTmp.FieldByName("pokaz_id")->AsInteger();
            formObject["note"] = qTmp.FieldByName("note")->AsString();
            formObject["col_n"] = qTmp.FieldByName("col_n")->AsInteger();
            formObject["form_readonly"] = qTmp.FieldByName("form_readonly")->AsInteger();
            formObject["form_vid_period"] = qTmp.FieldByName("form_vid_period")->AsInteger();
            formObject["form_k1"] = qTmp.FieldByName("form_k1")->AsInteger();
            formObject["form_k2"] = qTmp.FieldByName("form_k2")->AsInteger();
            formObject["form_k_zagtop"] = qTmp.FieldByName("form_k_zagtop")->AsInteger();
            formObject["form_k_zagbottom"] = qTmp.FieldByName("form_k_zagbottom")->AsInteger();
            formObject["is_dublir_itogo"] = qTmp.FieldByName("is_dublir_itogo")->AsInteger();
            formObject["is_noout_nstr"] = qTmp.FieldByName("is_noout_nstr")->AsInteger();
            formObject["plan_id"] = qTmp.FieldByName("plan_id")->AsInteger();
            formObject["userctgr_id"] = qTmp.FieldByName("userctgr_id")->AsInteger();
            formObject["work_with"] = qTmp.FieldByName("work_with")->AsInteger();
            formObject["class_id"] = qTmp.FieldByName("class_id")->AsInteger();
            formObject["ogr_class_id"] = qTmp.FieldByName("ogr_class_id")->AsInteger();
            formObject["for_class"] = qTmp.FieldByName("for_class")->AsInteger();
            formObject["is_add"] = qTmp.FieldByName("is_add")->AsInteger();
            formObject["is_copy"] = qTmp.FieldByName("is_copy")->AsInteger();
            formObject["is_svod"] = qTmp.FieldByName("is_svod")->AsInteger();
            formObject["name_svod"] = qTmp.FieldByName("name_svod")->AsString();
            formObject["is_sort"] = qTmp.FieldByName("is_sort")->AsInteger();
            formObject["vid_form"] = qTmp.FieldByName("vid_form")->AsInteger();
            formObject["is_edizm"] = qTmp.FieldByName("is_edizm")->AsInteger();
            formObject["formula_svod"] = qTmp.FieldByName("formula_svod")->AsString();
            formObject["tocnost"] = qTmp.FieldByName("tocnost")->AsInteger();
            formObject["work_with_gf"] = qTmp.FieldByName("work_with_gf")->AsInteger();
            formObject["islandscape"] = qTmp.FieldByName("islandscape")->AsInteger();
            formObject["isshowzero"] = qTmp.FieldByName("isshowzero")->AsInteger();
            formObject["ogruseroprcii"] = qTmp.FieldByName("ogruseroprcii")->AsString();
            formObject["sheetobozn"] = qTmp.FieldByName("sheetobozn")->AsString();
            formObject["isshapkacolor"] = qTmp.FieldByName("isshapkacolor")->AsInteger();
            formObject["islinescolor"] = qTmp.FieldByName("islinescolor")->AsInteger();
            formObject["ispanelsearch"] = qTmp.FieldByName("ispanelsearch")->AsInteger();
            formObject["colorlockedcell"] = qTmp.FieldByName("colorlockedcell")->AsInteger();
            formObject["n3tablename"] = qTmp.FieldByName("n3tablename")->AsString();
            contentArray.push_back(formObject);
            qTmp.Next();
        }

        root["Album"] = contentArray;
        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

std::string  Album::GetAlbumZaPeriod(int album_id, int byDate) {
    TQueryUni qTmp(DefDBconnect.Conn);
    json root;
    std::string  result;
    boost::posix_time::ptime firstDayOfYear = boost::posix_time::ptime(boost::gregorian::date(byDate, 1, 1));
    try {
        root["isForPeriod"] =  TDB_Album::GetAlbumZaPeriod(&qTmp, album_id, firstDayOfYear);
        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

std::string Album::GetAccessAlbumForUser(int context_id, int album_id) {
    json root;
    std::string  result;
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_AccessAlbum::GetAccessAlbumForUser(&qTmp, album_id, context_id);
        json contentArray = json::array();
        while (!qTmp.EoF()) {
            json formObject;
            formObject["plan_id"] = qTmp.FieldByName("plan_id")->AsInteger();
            formObject["plan_name"] = qTmp.FieldByName("plan_name")->AsString();
            formObject["plan_owner_id"] = qTmp.FieldByName("plan_owner_id")->AsInteger();
            formObject["plan_short_name"] = qTmp.FieldByName("plan_short_name")->AsString();
            formObject["plan_is_rajon"] = qTmp.FieldByName("plan_is_rajon")->AsInteger();
            formObject["god_beg"] = qTmp.FieldByName("god_beg")->AsString();
            formObject["god_end"] = qTmp.FieldByName("god_end")->AsString();
            formObject["accessalbum_id"] = qTmp.FieldByName("accessalbum_id")->AsInteger();
            formObject["album_id"] = qTmp.FieldByName("album_id")->AsInteger();
            formObject["god"] = qTmp.FieldByName("god")->AsInteger();
            formObject["user_perm"] = qTmp.FieldByName("user_perm")->AsString();
            contentArray.push_back(formObject);
            qTmp.Next();
        }
        root["AccessAlbum"] = contentArray;
        result = root.dump();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

std::string Album::GetAccessBlockAlbumForUser(int context_id, int album_id) {
    json root;
    std::string  result;
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_AccessAlbum::GetAccessBlockAlbumForUser(&qTmp, album_id);
        json contentArray = json::array();
        while (!qTmp.EoF()) {
            json formObject;
            formObject["accessalbum_id"] = qTmp.FieldByName("accessalbum_id")->AsInteger();
            formObject["album_id"] = qTmp.FieldByName("album_id")->AsInteger();
            formObject["album_obozn"] = qTmp.FieldByName("album_obozn")->AsString();
            contentArray.push_back(formObject);
            qTmp.Next();
        }
        root["AccessAlbum"] = contentArray;
        result = root.dump();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}


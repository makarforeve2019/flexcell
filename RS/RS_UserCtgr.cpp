//
// Created by Alex on 15.04.2024.
//

#include "RS_UserCtgr.h"
#include "../DB/DB_Common.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/DB_Userctgr.h"
#include "../DB/DB_GrpsCtgrs.h"
#include <../nlohmann/json.hpp>
#include <chrono>

using json = nlohmann::json;

std::string Userctgr::GetUserctgrAll(int context_id) {
    std::string JsonResult = "";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_Userctgr::GetUserctgrUser_All_sort(&qTmp, context_id);
        json root;
        json userCtgrArray = json::array();

        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm local_time = *std::localtime(&now_time);
        int currentYear = local_time.tm_year + 1900;

        if (!qTmp.IsEmpty()) {
            qTmp.First();
            while (!qTmp.EoF()) {
                int end_date = qTmp.FieldByName("end_date")->AsInteger();
                if (end_date >= currentYear) {
                    json userCtgr;
                    userCtgr["userctgr_id"] = qTmp.FieldByName("userctgr_id")->AsInteger();
                    userCtgr["userctgr_obozn"] = qTmp.FieldByName("userctgr_obozn")->AsString();
                    userCtgr["isnonasledmenu"] = qTmp.FieldByName("isnonasledmenu")->AsInteger();
                    userCtgr["menuvariant"] = qTmp.FieldByName("menuvariant")->AsInteger();
                    userCtgr["dostupn_funcii"] = qTmp.FieldByName("dostupn_funcii")->AsString();
                    userCtgrArray.push_back(userCtgr);
                }
                qTmp.Next();
            }
        }

        int prevYearCount = qTmp.RecordCount();
        root["prevyearcount"] = prevYearCount;
        root["userctgr"] = userCtgrArray;

        JsonResult = root.dump();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        JsonResult = "";
    }
    return JsonResult;
}

std::string Userctgr::GetUserctgrContent(int userctgr_id, int year) {
    std::string result = "";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_Userctgr::GetAlbumUserctgr_All_byGog(&qTmp, userctgr_id, year);
        json root;
        json contentArray = json::array();

        while (!qTmp.EoF()) {
            json albumObject;
            albumObject["album_typ"] = qTmp.FieldByName("album_typ")->AsString();
            albumObject["album_id"] = qTmp.FieldByName("album_id")->AsInteger();
            albumObject["album_obozn"] = qTmp.FieldByName("album_obozn")->AsString();
            albumObject["owner_id"] = qTmp.FieldByName("owner_id")->AsInteger();
            albumObject["album_isbp"] = qTmp.FieldByName("album_isbp")->AsString();
            albumObject["album_vid_func"] = qTmp.FieldByName("album_vid_func")->AsString();
            contentArray.push_back(albumObject);
            qTmp.Next();
        }

        root["Content"] = contentArray;

        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = "";
    }
    return result;
}

std::string Userctgr::GetUserctgrUserAllEndDate(int user_id) {
    std::string result = "";
    json root;
    json userctgrArray = json::array(); // Инициализация пустого массива
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_Userctgr::GetUserctgrUser_All_sort_end_date(&qTmp, user_id);

        while (!qTmp.EoF()) {
            json userctgrObject;
            userctgrObject["userctgr_id"] = qTmp.FieldByName("userctgr_id")->AsInteger();
            userctgrObject["userctgr_obozn"] = qTmp.FieldByName("userctgr_obozn")->AsString();
            userctgrObject["beg_date"] = qTmp.FieldByName("beg_date")->AsString();
            userctgrObject["end_date"] = qTmp.FieldByName("end_date")->AsString();
            userctgrObject["isnonasledmenu"] = qTmp.FieldByName("isnonasledmenu")->AsInteger();
            userctgrObject["menuvariant"] = qTmp.FieldByName("menuvariant")->AsInteger();
            userctgrArray.push_back(userctgrObject);
            qTmp.Next();
        }
        if (userctgrArray.empty()) {
            root["userctgr"] = userctgrArray; // Добавляем пустой массив в корневой объект
        } else {
            root["userctgr"] = userctgrArray; // Добавляем заполненный массив в корневой объект
        }
        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = "";
    }
    return result;
}

std::string Userctgr::GetUserUserctgrRO(int context_id, int userctgr_id) {
    int userctgrRO = TDB_Userctgr::GetUserUserctgrRO(context_id, userctgr_id);
    json jsonObj;
    jsonObj["isReadOnly"] = ((userctgrRO) ? true : false);
    // Конвертация JSON объекта в строку
    std::string result = jsonObj.dump();
    return result;
}

std::string Userctgr::ReadOnlyGrpsCtgrs(int context_id, int userctgr_id, int year) {
    int userctgrRO = TDB_GrpsCtgrs::getIsReadOnly_GrpsCtgrs(context_id, userctgr_id, year);
    json jsonObj;
    jsonObj["isReadOnly"] = ((userctgrRO) ? true : false);
    // Конвертация JSON объекта в строку
    std::string result = jsonObj.dump();
    return result;
}



//
// Created by Admin on 19.04.2024.
//

#include "RS_Forms.h"

using json = nlohmann::json;

std::string Forms::GetAllFormAlbum(int album_id, TDateTime byDate) {
    std::string result = "Not Result";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_Forms::GetAllFormAlbum(&qTmp, album_id, byDate);
        json root;
        json contentArray = json::array();

        while (!qTmp.EoF()) {
            json formObject;
            formObject["form_is_main"] = qTmp.FieldByName("form_is_main")->AsInteger();
            formObject["form_is_readonly"] = qTmp.FieldByName("form_is_readonly")->AsInteger();
            formObject["nModulOfMetods"] = qTmp.FieldByName("nmodulofmetods")->AsInteger();
            formObject["form_id"] = qTmp.FieldByName("form_id")->AsInteger();
            formObject["form_order"] = qTmp.FieldByName("form_order")->AsInteger();
            formObject["ogruseroprcii"] = qTmp.FieldByName("ogruseroprcii")->AsString();
            contentArray.push_back(formObject);
            qTmp.Next();
        }

        root["Form"] = contentArray;
        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

std::string Forms::Get1FormWithoutAlbum(int album_id, TDateTime byDate) {
    std::string result = "Not Result";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_Forms::Get1FormWithoutAlbum(&qTmp, album_id, byDate);
        json root;
        json contentArray = json::array();

        while (!qTmp.EoF()) {
            json formObject;
            formObject["form_is_main"] = qTmp.FieldByName("form_is_main")->AsInteger();
            formObject["form_is_readonly"] = qTmp.FieldByName("form_readonly")->AsInteger();
            formObject["nModulOfMetods"] = qTmp.FieldByName("nmodulofmetods")->AsInteger();
            formObject["form_id"] = qTmp.FieldByName("form_id")->AsInteger();
            formObject["form_order"] = qTmp.FieldByName("form_order")->AsInteger();
            formObject["ogruseroprcii"] = qTmp.FieldByName("ogruseroprcii")->AsString();
            contentArray.push_back(formObject);
            qTmp.Next();
        }

        root["Form"] = contentArray;
        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

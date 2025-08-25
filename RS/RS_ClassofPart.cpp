//
//

#include "RS_ClassofPart.h"

using json = nlohmann::json;

std::string ClassofPart::GetClassConteiners(int class_id) {
    std::string result = "Not Result";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_ClassofPart::GetClassConteiners(&qTmp, class_id);
        json root;
        json contentArray = json::array();

        while (!qTmp.EoF()) {
            json planObject;
            planObject["conteiner_id"] = qTmp.FieldByName("conteiner_id")->AsInteger();
            planObject["pokaz_id"] = qTmp.FieldByName("pokaz_id")->AsInteger();
            planObject["pokaz_name"] = qTmp.FieldByName("pokaz_name")->AsString();
            planObject["pokaz_type"] = qTmp.FieldByName("pokaz_type")->AsInteger();
            contentArray.push_back(planObject);
            qTmp.Next();
        }

        root["ClassofPart"] = contentArray;
        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

std::string ClassofPart::GetSubElems(int conteiner_id) {
    std::string result = "Not Result";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_ClassofPart::GetSubElems(&qTmp, conteiner_id);
        json root;
        json contentArray = json::array();
        while (!qTmp.EoF()) {
            json planObject;
            planObject["conteiner_id"] = qTmp.FieldByName("conteiner_id")->AsInteger();
            planObject["class_id"] = qTmp.FieldByName("class_id")->AsInteger();
            planObject["pokaz_id"] = qTmp.FieldByName("pokaz_id")->AsInteger();
            planObject["pokaz_name"] = qTmp.FieldByName("pokaz_name")->AsString();
            planObject["pokaz_type"] = qTmp.FieldByName("pokaz_type")->AsInteger();
            contentArray.push_back(planObject);
            qTmp.Next();
        }
        root["ClassofPart"] = contentArray;
        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}
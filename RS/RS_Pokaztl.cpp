//
//

#include "RS_Pokaztl.h"

using json = nlohmann::json;

std::string Pokaztl::GetPokaz(int pokaz_id) {
    std::string result = "Not Result";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);

        json jsonObj;
        if ( (TDB_Pokaztl::GetPokaz(&qTmp, pokaz_id) && (!qTmp.EoF())) ) {
            jsonObj["pokaz_id"] = qTmp.FieldByName("pokaz_id")->AsInteger();
            jsonObj["objekt_class_id"] = qTmp.FieldByName("objekt_class_id")->AsInteger();
            jsonObj["is_list"] = qTmp.FieldByName("is_list")->AsInteger();
            jsonObj["in_cast"] = qTmp.FieldByName("in_cast")->AsInteger();
            jsonObj["typeentity"] = qTmp.FieldByName("typeentity")->AsInteger();
            jsonObj["createinupd"] = qTmp.FieldByName("createinupd")->AsInteger();
            jsonObj["issurgtname"] = qTmp.FieldByName("issurgtname")->AsInteger();
            jsonObj["ismultiplans"] = qTmp.FieldByName("ismultiplans")->AsInteger();
            jsonObj["idslagaemoeforitogo"] = qTmp.FieldByName("idslagaemoeforitogo")->AsInteger();
            jsonObj["iditogoforslagaemoe"] = qTmp.FieldByName("iditogoforslagaemoe")->AsInteger();
            jsonObj["pokaz_is_razd"] = qTmp.FieldByName("pokaz_is_razd")->AsInteger();
            jsonObj["pokaz_owner_id"] = qTmp.FieldByName("pokaz_owner_id")->AsInteger();
            jsonObj["pokaz_vert"] = qTmp.FieldByName("pokaz_vert")->AsInteger();
            jsonObj["pokaz_type"] = qTmp.FieldByName("pokaz_type")->AsInteger();
            jsonObj["enum_id"] = qTmp.FieldByName("enum_id")->AsInteger();
            jsonObj["edizm_id"] = qTmp.FieldByName("edizm_id")->AsInteger();
            std::string ws = qTmp.FieldByName("pokaz_beg_date")->AsString();
            TDateTime dateTimeBEG(ws);
            jsonObj["pokaz_beg_date"] = to_string(DateTimeToUnixTimestamp(dateTimeBEG));  //?
            std::string ws2 = qTmp.FieldByName("pokaz_end_date")->AsString();
            TDateTime dateTimeEND(ws2);
            long long wi = DateTimeToUnixTimestamp(dateTimeEND);
            jsonObj["pokaz_end_date"] = to_string(wi);  //?
            jsonObj["pokaz_is_mes"] = qTmp.FieldByName("pokaz_is_mes")->AsInteger();
            jsonObj["pokaz_is_kv"] = qTmp.FieldByName("pokaz_is_kv")->AsInteger();
            jsonObj["pokaz_is_god"] = qTmp.FieldByName("pokaz_is_god")->AsInteger();
            jsonObj["pokaz_is_ist_fin"] = qTmp.FieldByName("pokaz_is_ist_fin")->AsInteger();
            jsonObj["pokaz_is_din_izm"] = qTmp.FieldByName("pokaz_is_din_izm")->AsInteger();
            jsonObj["pokaz_base_id"] = qTmp.FieldByName("pokaz_base_id")->AsInteger();
            jsonObj["pokaz_vsego_id"] = qTmp.FieldByName("pokaz_vsego_id")->AsInteger();
            jsonObj["np_vid_id"] = qTmp.FieldByName("np_vid_id")->AsInteger();
            jsonObj["level_sign"] = qTmp.FieldByName("level_sign")->AsInteger();
            jsonObj["create_date"] = qTmp.FieldByName("create_date")->AsInteger();
            jsonObj["pokaz_is_const"] = qTmp.FieldByName("pokaz_is_const")->AsInteger();
            jsonObj["pokaz_name"] = qTmp.FieldByName("pokaz_name")->AsString();
            jsonObj["level_text"] = qTmp.FieldByName("level_text")->AsString();
            jsonObj["primecanie"] = qTmp.FieldByName("primecanie")->AsString();
            jsonObj["forunikidentif"] = qTmp.FieldByName("forunikidentif")->AsString();
            jsonObj["pokaz_code"] = qTmp.FieldByName("pokaz_code")->AsString();
            jsonObj["pokaz_bk_code"] = qTmp.FieldByName("pokaz_bk_code")->AsString();
            jsonObj["formultiitogo"] = qTmp.FieldByName("formultiitogo")->AsString();
            jsonObj["idsownerfielditogo"] = qTmp.FieldByName("idsownerfielditogo")->AsString();
            jsonObj["vert_str"] = qTmp.FieldByName("vert_str")->AsString();
        }

        // Конвертация JSON объекта в строку
        result = jsonObj.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

std::string Pokaztl::GetRootPokaztl(int pokaz_id) {
    int wPokaz_id;
    std::string wPokaz_name;
    int wPokaz_is_razd;
    int wPokaz_type;
    int wPokaz_vert;
    std::string result = "Not Result";
    if (TDB_Pokaztl::GetRootPokaztl(pokaz_id, wPokaz_id, wPokaz_name, wPokaz_is_razd, wPokaz_type, wPokaz_vert)) {
        json jsonObj;
        jsonObj["pokaz_id"] = wPokaz_id;
        jsonObj["pokaz_name"] = wPokaz_name;
        jsonObj["pokaz_is_razd"] = wPokaz_is_razd;
        jsonObj["pokaz_type"] = wPokaz_type;
        jsonObj["pokaz_vert"] = wPokaz_vert;
        result = jsonObj.dump();
    }
    return result;
}

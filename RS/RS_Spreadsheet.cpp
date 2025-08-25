
#include "RS_Spreadsheet.h"
#include "DB_Common.h"
#include "unDefDBconnect.h"
#include "json.hpp"
#include "FormTypes.h"
#include "unFormLogicF1.h"
#include "../unServerConst.h"
#include "FormDopInp.h"
#include "FormDopUpdLogic.h"
#include "DB_PlanStr.h"
#include "DB_Pokaztl.h"
#include "dmGlobalVars.h"
#include "UpdBudgPro2.h"
#include "DB_Request_state.h"
#include "TracerManager.h"
#include "../OtherServices/Common.h"

//#include <boost/beast/http.hpp>
//#include <boost/beast/core.hpp>
//#include <boost/beast/core/detail/base64.hpp>

namespace http = boost::beast::http;

using Json = nlohmann::json;
/*
 * СТАРЫЙ OPEN FORM
std::string Spreadsheet::OpenForm(int port, std::string JsonStringws, http::response<http::string_body>& res) {
    TQueryUni *qTmp = new TQueryUni(DefDBconnect.Conn);
    nlohmann::json json;
    nlohmann::json resultArray;
    try {
        json = nlohmann::json::parse(JsonStringws);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Ошибка разбора: " << e.what() << std::endl;
        std::cerr << "Строка, вызвавшая ошибку: " << JsonStringws << std::endl;
    }
    string sessionId = json["TblSessionId"];
    int user_id = json["UserSessionId"].get<int>();
    Json formsArray = json["forms"];
    nlohmann::json j;
    j["ActionView"]["Actions"] = nlohmann::json::array();
    int indexform = 0;
    for (Json &formItem: formsArray) {
        bool isCurForm = formItem["isCurForm"].get<bool>();
        int CurrAlbumId = formItem["CurrAlbumId"].get<int>();
        int FormId = formItem["FormId"].get<int>();
        int FormOrder = formItem["FormOrder"].get<int>();
        int ownerFormPlanId = formItem["ownerFormPlanId"].get<int>();

        TInfoAlbumLogic InfoAlbumLogic;
        Json infoAlbumLogicObject = formItem["InfoAlbumLogic"];


        InfoAlbumLogic.CurrAlbumName = infoAlbumLogicObject["CurrAlbumName"].get<std::string>();
        InfoAlbumLogic.accessalbum_idSelBlockSubj = infoAlbumLogicObject["accessalbum_idSelBlockSubj"].get<int>();
        InfoAlbumLogic.CurrPlanId = infoAlbumLogicObject["CurrPlanId"].get<int>();
        InfoAlbumLogic.CanEdit = infoAlbumLogicObject["CanEdit"].get<bool>();
        InfoAlbumLogic.CurrPlanName = infoAlbumLogicObject["CurrPlanName"].get<std::string>();
        InfoAlbumLogic.CurrVidOtch = infoAlbumLogicObject["CurrVidOtch"].get<int>();
        InfoAlbumLogic.CurrYear = infoAlbumLogicObject["CurrYear"].get<int>();
        InfoAlbumLogic.CurrQuarter = infoAlbumLogicObject["CurrQuarter"].get<int>();
        InfoAlbumLogic.CurrMonth = infoAlbumLogicObject["CurrMonth"].get<int>();

        string CurrDatews = infoAlbumLogicObject["CurrDate"].get<std::string>();
        double currDateString = StrToFloat(CurrDatews);
        InfoAlbumLogic.CurrDate = UnixTimestampToDateTime(currDateString);

        InfoAlbumLogic.aShablonKBK = infoAlbumLogicObject["aShablonKBK"].get<std::string>();
        InfoAlbumLogic.aId_Owner_link1n = infoAlbumLogicObject["aId_Owner_link1n"].get<int>();

        TInfoFormLogic InfoFormLogic;
        auto infoFormLogicObject = formItem["InfoFormLogic"];
        InfoFormLogic.isISTZNACHVVODPK = infoFormLogicObject["isISTZNACHVVODPK"].get<bool>();
        InfoFormLogic.modeNumber = infoFormLogicObject["modeNumber"].get<int>();
        string wsselectedDate = infoFormLogicObject["selectedDate"].get<std::string>();
        double selectedDate = StrToFloat(wsselectedDate);
        InfoFormLogic.selectedDate = UnixTimestampToDateTime(selectedDate);
        InfoFormLogic.isOtlogenPokaz = infoFormLogicObject["isOtlogenPokaz"].get<bool>();
        InfoFormLogic.OtlogenPokazId = infoFormLogicObject["OtlogenPokazId"].get<int>();


        TFormLogicF1 *formLogic = nullptr;
        bool sessionExists = false;
        if (GlobalSessionManager) {
            sessionExists = GlobalSessionManager->GetSessionData(sessionId, formLogic);
        }

        if (!sessionExists) {
            formLogic = new TFormLogicF1();
            formLogic->quForms = qTmp;
        }

        bool loadContent = false;
        if (isCurForm == true) {
            loadContent = true;
        }
        GlobalVars->UserId = user_id;
        formLogic->OpenForm(isCurForm, CurrAlbumId, InfoAlbumLogic, InfoFormLogic, 0, 0, "", FormId, FormOrder,
                            ownerFormPlanId);

//        formLogic->f1Forms->Save(u"/home/user/makarov/oloo.xlsx");

;
        if (!sessionExists && GlobalSessionManager) {
            GlobalSessionManager->AddSessionData(sessionId, formLogic);
        }

        Json CurrElementJson;
        if (isCurForm == true) {
            CurrElementJson = Json::parse(formLogic->CurrElementStr);
            resultArray = CurrElementJson;
            std::string resXlsToStr;
            formLogic->XlsToStrJSON(formLogic->f1Forms->Sheet, resXlsToStr);

            Json resXlsToStrJSON = Json::parse(resXlsToStr);

            CurrElementJson["FormJSON"] = resXlsToStrJSON;

            //j["ActionView"]["Actions"] = nlohmann::json::array();
            j["ActionView"]["Actions"].push_back(CurrElementJson);
            j["hdlprt"] = port;
//            std::cerr << "Сессия:" << sessionId << ", порт:"  << port << std::endl;
        } else {
            TwwStringList* pTww = formLogic->GetTwwByIndex(indexform);
            if (pTww  != nullptr) {
                CurrElementJson["formObozn"] = pTww->FormObozn;
            }
            CurrElementJson["FormJSON"] = "";
            j["ActionView"]["Actions"].push_back(CurrElementJson);
        }
        indexform++;
    }
    res.set(http::field::set_cookie, "tblSession=" + sessionId + "; Path=/");
    return to_string(j);
}
*/

std::string Spreadsheet::OpenForm(int port, std::string JsonStringws, http::response<http::string_body>& res, std::string xRequestId) {
    TracerManager::Instance().StartFunctionTrace("Spreadsheet::OpenForm/Main");

    TQueryUni *qTmp = new TQueryUni(DefDBconnect.Conn);
    nlohmann::json json;
    nlohmann::json resultArray;


    try {
        json = nlohmann::json::parse(JsonStringws);
    } catch (const nlohmann::json::parse_error& e) {
        TracerManager::Instance().AddSpanLog("Ошибка разбора JSON", {{"JSON", JsonStringws},
                                                                     {"ошибка", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка разбора: " << e.what() << std::endl;
        std::cerr << "Строка, вызвавшая ошибку: " << JsonStringws << std::endl;

        TDB_Request_state::UpdateResponseCode(xRequestId, 400);
        TracerManager::Instance().EndFunctionTrace();
        return "";
    }

    std::string sessionId = json["TblSessionId"];
    int user_id = json["UserSessionId"].get<int>();
    Json formsArray = json["forms"];
    nlohmann::json j;
    j["ActionView"]["Actions"] = nlohmann::json::array();
    int indexform = 0;

    for (Json &formItem : formsArray) {
        bool isCurForm = formItem["isCurForm"].get<bool>();
        int CurrAlbumId = formItem["CurrAlbumId"].get<int>();
        int FormId = formItem["FormId"].get<int>();
        int FormOrder = formItem["FormOrder"].get<int>();
        int ownerFormPlanId = formItem["ownerFormPlanId"].get<int>();

        TInfoAlbumLogic InfoAlbumLogic;
        Json infoAlbumLogicObject = formItem["InfoAlbumLogic"];

        InfoAlbumLogic.CurrAlbumName = infoAlbumLogicObject["CurrAlbumName"].get<std::string>();
        InfoAlbumLogic.accessalbum_idSelBlockSubj = infoAlbumLogicObject["accessalbum_idSelBlockSubj"].get<int>();
        InfoAlbumLogic.CurrPlanId = infoAlbumLogicObject["CurrPlanId"].get<int>();
        InfoAlbumLogic.CanEdit = infoAlbumLogicObject["CanEdit"].get<bool>();
        InfoAlbumLogic.CurrPlanName = infoAlbumLogicObject["CurrPlanName"].get<std::string>();
        InfoAlbumLogic.CurrVidOtch = infoAlbumLogicObject["CurrVidOtch"].get<int>();
        InfoAlbumLogic.CurrYear = infoAlbumLogicObject["CurrYear"].get<int>();
        InfoAlbumLogic.CurrQuarter = infoAlbumLogicObject["CurrQuarter"].get<int>();
        InfoAlbumLogic.CurrMonth = infoAlbumLogicObject["CurrMonth"].get<int>();

        std::string CurrDatews = infoAlbumLogicObject["CurrDate"].get<std::string>();
        double currDateString = StrToFloat(CurrDatews);
        InfoAlbumLogic.CurrDate = UnixTimestampToDateTime(currDateString);

        InfoAlbumLogic.aShablonKBK = infoAlbumLogicObject["aShablonKBK"].get<std::string>();
        InfoAlbumLogic.aId_Owner_link1n = infoAlbumLogicObject["aId_Owner_link1n"].get<int>();

        TInfoFormLogic InfoFormLogic;
        auto infoFormLogicObject = formItem["InfoFormLogic"];
        InfoFormLogic.isISTZNACHVVODPK = infoFormLogicObject["isISTZNACHVVODPK"].get<bool>();
        InfoFormLogic.modeNumber = infoFormLogicObject["modeNumber"].get<int>();
        std::string wsselectedDate = infoFormLogicObject["selectedDate"].get<std::string>();
        double selectedDate = StrToFloat(wsselectedDate);
        InfoFormLogic.selectedDate = UnixTimestampToDateTime(selectedDate);
        InfoFormLogic.isOtlogenPokaz = infoFormLogicObject["isOtlogenPokaz"].get<bool>();
        InfoFormLogic.OtlogenPokazId = infoFormLogicObject["OtlogenPokazId"].get<int>();

        TFormLogicF1 *formLogic = nullptr;
        bool sessionExists = false;
        if (GlobalSessionManager) {
            sessionExists = GlobalSessionManager->GetSessionData(sessionId, formLogic);
        }

        if (!sessionExists) {
            formLogic = new TFormLogicF1();
            formLogic->quForms = qTmp;
        }

        bool loadContent = isCurForm;
        GlobalVars->UserId = user_id;
        formLogic->OpenForm(isCurForm, CurrAlbumId, InfoAlbumLogic, InfoFormLogic, 0, 0, "", FormId, FormOrder, ownerFormPlanId);
        //double wsdb = formLogic->f1Forms->NumberRC_Get(11, 7);

        if (!sessionExists && GlobalSessionManager) {
            GlobalSessionManager->AddSessionData(sessionId, formLogic);
        }

        Json CurrElementJson;
        if (isCurForm) {
            CurrElementJson = Json::parse(formLogic->CurrElementStr);
            Json resXlsToStrJSON;
            formLogic->XlsToStrJSON(formLogic->f1Forms->Sheet, resXlsToStrJSON);

            CurrElementJson["FormJSON"] = resXlsToStrJSON;

            j["ActionView"]["Actions"].push_back(CurrElementJson);
            j["hdlprt"] = port;
        } else {
            TwwStringList* pTww = formLogic->GetTwwByIndex(indexform);
            if (pTww != nullptr) {
                CurrElementJson["formObozn"] = pTww->FormObozn;
            }
            CurrElementJson["FormJSON"] = "";
            j["ActionView"]["Actions"].push_back(CurrElementJson);
        }
        indexform++;
    }

    TracerManager::Instance().EndFunctionTrace();
    TracerManager::Instance().StartFunctionTrace("Spreadsheet::OpenForm/RequestStateClose");

    res.set(http::field::set_cookie, "tblSession=" + sessionId + "; Path=/");

        Common common(endpointNg);
        int responseStatus = 0;


        std::string response = common.RequestStateClose("/MWSRestAPI/UBPRosleshoz", xRequestId, j.dump(), 200, responseStatus);

        if (responseStatus == 200) {
            //std::cout << "RequestStateClose succeeded: " << response << std::endl;
        } else {
            std::cout << "RequestStateClose failed with status: " << responseStatus << std::endl;
            TracerManager::Instance().AddSpanLog("RequestStateClose failed with error status", {{"status", std::to_string(static_cast<int>(responseStatus))}},
                                                 TracerManager::LOG_LEVEL_ERROR);
        }

    std::string openform_result = to_string(j);
    TracerManager::Instance().AddSpanLog("Результат Openform", {{"JSON", openform_result}},
                                         TracerManager::LOG_LEVEL_INFO);

    return openform_result;
}

std::string Spreadsheet::GetJsonForm(std::string TblSessionId) {
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }
    Json CurrElementJson;
    CurrElementJson = Json::parse(formLogic->CurrElementStr);
    Json resXlsToStrJSON;
    formLogic->XlsToStrJSON(formLogic->f1Forms->Sheet, resXlsToStrJSON);

    CurrElementJson["FormJSON"] = resXlsToStrJSON;
//            std::cerr << "Сессия:" << sessionId << ", порт:"  << port << std::endl;

    return to_string(CurrElementJson);
}


std::string Spreadsheet::CellValueAtr(std::string TblSessionId, int nSheet, int nRow, int nCol) {
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }

    Json JSONresult;
    formLogic->f1Forms->Sheet = nSheet;
    formLogic->TblSessionId = TblSessionId;
//    std::cerr << "Сессия:" << TblSessionId << std::endl;

    PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(formLogic, nRow, nCol);
    if (vd == nullptr){
        return "";
    }
    TwwStringList* p = formLogic->FormsValuesArray->GetItem(nSheet - 1);
    if (nRow < p->form_begBodyRow || nRow > p->form_endBodyRow){
        return "";
    }

    bool cellValueResult = formLogic->CellValueAtr(nSheet, nRow, nCol, JSONresult);
    if (cellValueResult) {
        std::string JSONString = to_string(JSONresult);
        return JSONString; // Возвращаем строковое представление JSON
    } else {
        std::string JSONString = to_string(JSONresult);
        return JSONString; // Возвращаем обозначение ошибки
    }
}

std::string Spreadsheet::CellUpdate(std::string JsonStringws, boost::beast::http::status& status) {
    TracerManager::Instance().StartFunctionTrace("CellUpdate");
    // Парсим JSON строку
    Json jsonObj = nlohmann::json::parse(JsonStringws);
    std::string TblSessionId = "";
    std::string editValue = "";
    int nSheet = 0;
    int nRow = 0;
    int nCol = 0;
    int valueID = 0;
    if (jsonObj != nullptr) {
        // Извлекаем данные
        TblSessionId = jsonObj["TblSessionId"];
        nSheet = jsonObj["nSheet"];
        nRow = jsonObj["nRow"];
        nCol = jsonObj["nCol"];
    }
    Json JSONresult = {
            {"nSheet", nSheet},
            {"Row", json::array()},
            {"Col", json::array()},
            {"Values", json::array()},
            {"Styles", json::array()}
    };

    if (!GlobalSessionManager) {
        TracerManager::Instance().AddSpanLog("GlobalSessionManager не найден", {},
                                             TracerManager::LOG_LEVEL_ERROR);
        TracerManager::Instance().EndFunctionTrace();
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        TracerManager::Instance().AddSpanLog("Сессия TblSessionId не найдена", {{"TblSessionId", TblSessionId}},
                                             TracerManager::LOG_LEVEL_ERROR);
        TracerManager::Instance().EndFunctionTrace();
        return ""; // Сессия не найдена или другая ошибка
    }
    PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(formLogic, nRow, nCol);
    if (vd->pokaz_type == 3){
        valueID = jsonObj["editValue"]["id"];
        editValue = jsonObj["editValue"]["name"];
    } else {
        if (jsonObj["editValue"] == ""){
            editValue = "0";
        } else if (jsonObj["editValue"].is_null()){
            editValue = "";
        } else{
            editValue = jsonObj["editValue"];
        }

    }
    std::string JSONString = "";
    JSONresult["Row"].push_back(nRow);
    JSONresult["Col"].push_back(nCol);
    JSONresult["Values"].push_back(utf16_to_utf8(formLogic->f1Forms->TextRC_Get(nRow, nCol)));
    JSONresult["Styles"].push_back(utf16_to_utf8(formLogic->f1Forms->StyleNameRC_Get(nRow, nCol)));
    JSONString = to_string(JSONresult);
    bool cellValueResult = formLogic->CellEndingEdit(nSheet, nRow, nCol, editValue, valueID, JSONresult);
    if (cellValueResult) {
        JSONresult["Row"].push_back(nRow);
        JSONresult["Col"].push_back(nCol);
        JSONString = to_string(JSONresult);
        if (vd->pokaz_type == 5) {
            if (editValue == ""){
                JSONresult["Values"].push_back("");
                JSONString = to_string(JSONresult);
            } else {
                double FloatValue = StrToFloat(editValue);
                TDateTime valTDT = UnixTimestampToDateTime(FloatValue);
                string ws = valTDT.ToFormattedDate();
                JSONresult["Values"].push_back(ws);
                JSONString = to_string(JSONresult);
            }
        } else {
            JSONresult["Values"].push_back(editValue);
        }
        JSONresult["Styles"].push_back(utf16_to_utf8(formLogic->f1Forms->StyleNameRC_Get(nRow, nCol)));
        formLogic->UpdExecuteCellsEndInput(nSheet, nRow, nCol, true, JSONresult);
        JSONString = to_string(JSONresult);
        TracerManager::Instance().EndFunctionTrace();
        return JSONString; // Возвращаем строковое представление JSON
    } else {
        status = http::status::unauthorized;
        std::string JSONString = to_string(JSONresult);
        TracerManager::Instance().EndFunctionTrace();
        return JSONString; // Возвращаем обозначение ошибки
    }
}

std::string Spreadsheet::CloseForm(std::string JsonStringws) {
    Json jsonObj = nlohmann::json::parse(JsonStringws);
    std::string TblSessionId = "";
    std::string editValue = "";
    TblSessionId = jsonObj["TblSessionId"];
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.



    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return "400"; // Сессия не найдена или другая ошибка
    } else {

        if  (formLogic->f1Forms->NumSheets == 1) {
            GlobalSessionManager->DeleteSession(TblSessionId);
            return "\"TblSessionId\": \""+TblSessionId+"\",\"AlbumClosed\": true";
        } else {
            //НАПИСАТЬ ЗАКРЫТИЕ ОДНОГО ЛИСТА
            formLogic->CloseForm(formLogic->CurrSheet);
            return "\"TblSessionId\": \""+TblSessionId+"\",\"AlbumClosed\": false";
        }
    }
}

std::string Spreadsheet::Close(std::string JsonStringws) {
    Json jsonObj = nlohmann::json::parse(JsonStringws);
    std::string TblSessionId = "";
    TblSessionId = jsonObj["TblSessionId"];
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return "400"; // Сессия не найдена или другая ошибка
    } else {
        GlobalSessionManager->DeleteSession(TblSessionId);
//        delete formLogic;
        return "200";
    }
}

std::string Spreadsheet::GetTww(std::string context_id, std::string TblSessionId, int nSheet) {
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }
    TFormLogicF1* formLogic = nullptr;
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }
//    TwwStringList* p = TFormDopInp::GetTww(formLogic);
    TwwStringList* p = formLogic->FormsValuesArray->GetItem(nSheet - 1);
    nlohmann::json j;

    j = {
            {"FormId", p->FormId},
            {"FormOrder", p->FormOrder},
            {"FormObozn", p->FormObozn},
            {"plan_id", p->plan_id},
            {"plan_name", p->plan_name},
            {"is_dublir_itogo", p->is_dublir_itogo},
            {"IsParamForm", p->IsParamForm},
            {"ParamObjectId", p->ParamObjectId},
            {"ParamObjectName", p->ParamObjectName},
            {"ParamClassIdlist", p->ParamClassIdlist},
            {"work_with", p->work_with},
            {"work_with_gf", p->work_with_gf},
            {"form_is_main", p->form_is_main},
            {"RootPokazId", p->RootPokazId},
            {"cast_id_UP", p->cast_id_UP},
            {"cast_id_UP_name", p->cast_id_UP_name},
            {"form_readonly", p->form_readonly},
            {"isRefreshRows", p->isRefreshRows},
            {"is_add", p->is_add},
            {"is_copy", p->is_copy},
            {"is_sort", p->is_sort},
            {"is_edIzm", p->is_edIzm},
            {"tocnost", p->tocnost},
            {"NameOwnerRaskrit", p->NameOwnerRaskrit},
            {"ObjectIdOwnerRaskrit", p->ObjectIdOwnerRaskrit},
            {"nRowRaskrit", p->nRowRaskrit},
            {"IsRefreshOwner", p->IsRefreshOwner},
            {"ColFuncs", p->ColFuncs},
            {"vid_form", p->vid_form},
            {"PokazCol", p->PokazCol},
            {"PokazColAlter", p->PokazColAlter},
            {"PokazColother", p->PokazColother},
            {"nameColOkraska", p->nameColOkraska},
            {"DeltaRow", p->DeltaRow},//заголовок
            {"MaxHrRow", p->MaxHrRow},
            {"MaxHrCol", p->MaxHrCol},
            {"form_k_zagtop", p->form_k_zagtop},
            {"form_k_zagbottom", p->form_k_zagbottom},//подвал
            {"form_k1", p->form_k1},//шапка
            {"form_begBodyRow", p->form_begBodyRow},//первая строка
            {"form_endBodyRow", p->form_endBodyRow},// последняя
            {"isLandscape", p->isLandscape},
            {"isShowZero", p->isShowZero},
            {"nColForBP", p->nColForBP},
            {"InitKolCol", p->InitKolCol},
            {"form_str_n_AlignmentObjedinit", p->form_str_n_AlignmentObjedinit},
            {"OgrUserOprcii", p->OgrUserOprcii},
            {"ogr_class_id", p->ogr_class_id},
            {"grnlutkp_id", p->grnlutkp_id},
            {"grnlutkp_id_ForRedactir", p->grnlutkp_id_ForRedactir},
            {"IsStmntPrisvoit", p->IsStmntPrisvoit},
            {"nRowOfStmntTestReadOnly", p->nRowOfStmntTestReadOnly},
            {"isStmntTestReadOnly", p->isStmntTestReadOnly},
            {"isPU0", p->isPU0},
            {"ComplexForm", p->ComplexForm},
            {"nModulOfMetods", p->nModulOfMetods},
            {"isGroupSelect", p->isGroupSelect},
            {"isShapkaColor", p->isShapkaColor},
            {"isLinesColor", p->isLinesColor},
            {"isPanelSearch", p->isPanelSearch},
            {"ColorLockedCell", p->ColorLockedCell},
            {"is_noout_nstr", p->is_noout_nstr},
            {"isOnActivForm_RefreshROcells", p->isOnActivForm_RefreshROcells},
            {"isExecAllVspomogForms", p->isExecAllVspomogForms},
            {"isDoOkraska", p->isDoOkraska},
            {"oldRow", p->oldRow},
            {"pPattern", p->pPattern},
            {"pPatternNew", p->pPatternNew},
            {"pcrFG", p->pcrFG},
            {"pcrFGNew", p->pcrFGNew},
            {"isKBKfiltr", p->isKBKfiltr},
            {"KBKfiltrRow", p->KBKfiltrRow},
            {"KBKfiltrCol", p->KBKfiltrCol},
            {"isDatefiltr", p->isDatefiltr},
            {"DatefiltrFromRow", p->DatefiltrFromRow},
            {"DatefiltrFromCol", p->DatefiltrFromCol},
            {"DatefiltrToRow", p->DatefiltrToRow},
            {"DatefiltrToCol", p->DatefiltrToCol},
            {"stmntODINAKOVZNAC", p->stmntODINAKOVZNAC},
            {"isPlFctOstatok", p->isPlFctOstatok},
            {"isPriglashKVstavke", p->isPriglashKVstavke},
            {"isISTZNACHVVODPK", p->isISTZNACHVVODPK},
            {"modeNumber", p->modeNumber},
            {"selectedDate", DateTimeToStr(p->selectedDate)},
            {"wwPlan_id", p->wwPlan_id},
            {"OwnerFormPlanId", p->OwnerFormPlanId},
            {"colTree1", p->colTree1},
            {"isGroupSost", p->isGroupSost}
    };
    return to_string(j);
}

std::string Spreadsheet::GetAttRow(std::string context_id, std::string TblSessionId, int nSheet, int nRow) {
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }
    TFormLogicF1* formLogic = nullptr;
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }
    TwwStringList* p = formLogic->FormsValuesArray->GetItem(nSheet - 1);
//    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(formLogic, nRow);
    PAtrRow AtrRow;
    if (p != nullptr && p->ListRows != nullptr) {
        // Если индекс превышает размер списка, увеличиваем его на 50 элементов
        if (nRow >= p->ListRows->Count()) {
            p->ListRows->SetCount(p->ListRows->Count() + 50);
        }

        // Убедитесь, что индекс находится в допустимом диапазоне
        if (nRow > 0 && nRow < p->ListRows->Count()) {
            AtrRow = ((*p->ListRows)[nRow]);
        }
    }
    if (AtrRow == nullptr){
        return "";
    }


    nlohmann::json j;

    j =  {
            {"Row", AtrRow->Row},
            {"IsTerminal", AtrRow->IsTerminal},
            {"OwnerNomRow", AtrRow->OwnerNomRow},
            {"OwnerId", AtrRow->OwnerId},
            {"ObjectId", AtrRow->ObjectId},
            {"objekt_class_id", AtrRow->objekt_class_id},
            {"CurRowVid", AtrRow->CurRowVid},
            {"level_sign", AtrRow->level_sign},
            {"pokaz_vert", AtrRow->pokaz_vert},
            {"L", AtrRow->L},
            {"inSort", AtrRow->inSort},
            {"IsUpd", AtrRow->IsUpd},
            {"str_id", AtrRow->str_id},
            {"is_filter2", AtrRow->is_filter2},
            {"ColOnFilter2", AtrRow->ColOnFilter2},
            {"vrem_hidden", AtrRow->vrem_hidden},
            {"isRowHidden", AtrRow->isRowHidden},
            {"isReadOnly", AtrRow->isReadOnly},
            {"isRefresh", AtrRow->isRefresh},
            {"tocnost", AtrRow->tocnost},
            {"pokaz_bk_code", AtrRow->pokaz_bk_code},
            {"ist_fin_grad_id", AtrRow->ist_fin_grad_id},
            {"plan_id", AtrRow->plan_id},
            {"god", AtrRow->god},
            {"colReplValue", AtrRow->colReplValue},
            {"NameReplValue", AtrRow->NameReplValue},
            {"FormatStyle", {
                            {"align_vert", AtrRow->FormatStyle.align_vert},
                            {"align_gor", AtrRow->FormatStyle.align_gor},
                            {"font", AtrRow->FormatStyle.font},
                            {"h", AtrRow->FormatStyle.h},
                            {"is_bold", AtrRow->FormatStyle.is_bold},
                            {"isOrientation", AtrRow->FormatStyle.isOrientation},
                            {"ofset", AtrRow->FormatStyle.ofset},
                            {"FontColor", AtrRow->FormatStyle.FontColor},
                            {"fonColor", AtrRow->FormatStyle.fonColor},
                            {"borderStyle", AtrRow->FormatStyle.borderStyle},
                            {"borderColor", AtrRow->FormatStyle.borderColor},
                            {"NumberFormat", AtrRow->FormatStyle.NumberFormat}
                    }},
            {"msg", AtrRow->msg},
            {"isPriglashKVstavke", AtrRow->isPriglashKVstavke},
            {"NomUrovRazdela", AtrRow->NomUrovRazdela},
            {"isRow1n", AtrRow->isRow1n},
            {"isRow1nReadOnly", AtrRow->isRow1nReadOnly},
            {"isRow1nhidden", AtrRow->isRow1nhidden},
            {"isMarked", AtrRow->isMarked},
            {"groupSpan", AtrRow->groupSpan},
            {"groupMarkLevel", AtrRow->groupMarkLevel},
            {"is_filter", AtrRow->is_filter}
    };

    return to_string(j);
}

std::string Spreadsheet::GetNumberActiveSessions() {
    if (!GlobalSessionManager) {
        return "0"; // Или любое другое обозначение ошибки
    }
    return IntToStr(GlobalSessionManager->GetCount());
}

std::string Spreadsheet::ButtonClick(std::string JsonStringws)  {
    Json jsonObj = nlohmann::json::parse(JsonStringws);
    std::string TblSessionId = "";
    int nSheet = 0;
    int nRow = 0;
    if (jsonObj != nullptr) {
        // Извлекаем данные
        TblSessionId = jsonObj["TblSessionId"];
        nSheet = jsonObj["nSheet"];
        nRow = jsonObj["nRow"];
    }
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }
    Json JSONresult = {
            {"nSheet", nSheet},
            {"Row", json::array()},
            {"Col", json::array()},
            {"Values", json::array()},
            {"Styles", json::array()},
            {"Message", ""}
    };
    TwwStringList* p = formLogic->FormsValuesArray->GetItem(nSheet - 1);
    PAtrRow AtrRow;
    AtrRow = ((*p->ListRows)[nRow]);
    int res = formLogic->f1Forms->NumberRC_Get(nRow, AtrRow->ColOnFilter2);
    std:string ws;
    if (p->isStmntTestReadOnly){
        ws = to_string(JSONresult);
        return ws;
    }
    if (res != 0) {
        TFormDopUpdLogic::ClickButton(formLogic, nSheet, nRow, JSONresult);
        ws = to_string(JSONresult);
        return ws;
    } else {
        JSONresult["Message"] = AtrRow->msg;
        ws = to_string(JSONresult);
        return ws;

    }
}
// Все возвращают либо 200 либо 400
std::string Spreadsheet::insertPokaztl(int port, std::string JsonString) {
    TQueryUni *qTmp = new TQueryUni(DefDBconnect.Conn);
    nlohmann::json json = nlohmann::json::parse(JsonString);

    struct Form {
        bool isCurForm;
        int FormId;
        int CurrAlbumId;
        int IsParamForm;
        int ParamObjectId;
        std::string ParamObjectName;
        int FormOrder;
        int ownerFormPlanId;
    };
    TInfoFormLogic InfoFormLogic;
    TInfoAlbumLogic InfoAlbumLogic;
    int pokaz_id = json.at("pokaz_id").get<int>();
    int plan_id = json.at("plan_id").get<int>();
    bool isItogo = json.at("isItogo").get<bool>();
    int pokaz_is_razd = json.at("pokaz_is_razd").get<int>();
    int pokaz_owner_id = json.at("pokaz_owner_id").get<int>();
    int in_cast = json.at("in_cast").get<int>();
    std::string pokaz_name = json.at("pokaz_name").get<std::string>();
    std::string Primecanie = json.at("Primecanie").get<std::string>();
    int is_list = json.at("is_list").get<int>();
    int level_sign = json.at("level_sign").get<int>();
    std::string level_text = json.at("level_text").get<std::string>();
    int pokaz_vert = json.at("pokaz_vert").get<int>();
    std::string vert_str = json.at("vert_str").get<std::string>();
    int pokaz_type = json.at("pokaz_type").get<int>();
    std::string pokaz_code = json.at("pokaz_code").get<std::string>();
    std::string pokaz_bk_code = json.at("pokaz_bk_code").get<std::string>();
    std::string pokaz_beg_date = json.at("pokaz_beg_date").get<std::string>();
    std::string pokaz_end_date = json.at("pokaz_end_date").get<std::string>();
    int enum_id = json.at("enum_id").get<int>();
    int edizm_id = json.at("edizm_id").get<int>();
    int pokaz_is_const = json.at("pokaz_is_const").get<int>();
    int pokaz_is_god = json.at("pokaz_is_god").get<int>();
    int pokaz_is_kv = json.at("pokaz_is_kv").get<int>();
    int pokaz_is_mes = json.at("pokaz_is_mes").get<int>();
    int pokaz_is_ist_fin = json.at("pokaz_is_ist_fin").get<int>();
    int pokaz_is_din_izm = json.at("pokaz_is_din_izm").get<int>();
    std::string TblSessionId = json.at("TblSessionId").get<std::string>();
    int nSheet = json.at("nSheet").get<int>();
    int nRow = json.at("nRow").get<int>();
    int nCol = json.at("nCol").get<int>();


    const auto& formsArray = json.at("forms");
    Form formObj;
    for (const auto& form : formsArray) {

        formObj.isCurForm = form.at("isCurForm").get<bool>();
        formObj.FormId = form.at("FormId").get<int>();
        formObj.CurrAlbumId = form.at("CurrAlbumId").get<int>();
        formObj.IsParamForm = form.at("IsParamForm").get<int>();
        formObj.ParamObjectId = form.at("ParamObjectId").get<int>();
        formObj.ParamObjectName = form.at("ParamObjectName").get<std::string>();
        formObj.FormOrder = form.at("FormOrder").get<int>();
        formObj.ownerFormPlanId = form.at("ownerFormPlanId").get<int>();

        const auto& infoAlbumLogicJson = form.at("InfoAlbumLogic");
        InfoAlbumLogic.CurrAlbumName = infoAlbumLogicJson.at("CurrAlbumName").get<std::string>();
        InfoAlbumLogic.accessalbum_idSelBlockSubj = infoAlbumLogicJson.at("accessalbum_idSelBlockSubj").get<int>();
        InfoAlbumLogic.CurrPlanId = infoAlbumLogicJson.at("CurrPlanId").get<int>();
        InfoAlbumLogic.CurrPlanName = infoAlbumLogicJson.at("CurrPlanName").get<std::string>();
        InfoAlbumLogic.CanEdit = infoAlbumLogicJson.at("CanEdit").get<bool>();
        InfoAlbumLogic.CurrVidOtch = infoAlbumLogicJson.at("CurrVidOtch").get<int>();
        InfoAlbumLogic.CurrYear = infoAlbumLogicJson.at("CurrYear").get<int>();
        InfoAlbumLogic.CurrQuarter = infoAlbumLogicJson.at("CurrQuarter").get<int>();
        InfoAlbumLogic.CurrMonth = infoAlbumLogicJson.at("CurrMonth").get<int>();
//        InfoAlbumLogic.CurrDate = infoAlbumLogicJson.at("CurrDate").get<std::int>();
        string CurrDatews = infoAlbumLogicJson.at("CurrDate").get<std::string>();
        double currDateString = StrToFloat(CurrDatews);
        InfoAlbumLogic.CurrDate = UnixTimestampToDateTime(currDateString);
        InfoAlbumLogic.aShablonKBK = infoAlbumLogicJson.at("aShablonKBK").get<std::string>();
        InfoAlbumLogic.aId_Owner_link1n = infoAlbumLogicJson.at("aId_Owner_link1n").get<int>();

        const auto& infoFormLogicJson = form.at("InfoFormLogic");
        InfoFormLogic.isISTZNACHVVODPK = infoFormLogicJson.at("isISTZNACHVVODPK").get<bool>();
        InfoFormLogic.modeNumber = infoFormLogicJson.at("modeNumber").get<int>();
//        InfoFormLogic.selectedDate = infoFormLogicJson.at("selectedDate").get<std::string>();
        string wsselectedDate = infoFormLogicJson.at("selectedDate").get<std::string>();
        double selectedDate = StrToFloat(wsselectedDate);
        InfoFormLogic.selectedDate = UnixTimestampToDateTime(selectedDate);
    }

    int nRowSel = nRow;
    int isSub = 1;

    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }
    formLogic->quForms = qTmp;
    TDateTime pokaz_beg_dateTDateTime = UnixTimestampToDateTime(StrToFloat(pokaz_beg_date));
    TDateTime pokaz_end_dateTDateTime = UnixTimestampToDateTime(StrToFloat(pokaz_end_date));

    TwwStringList* p = formLogic->FormsValuesArray->GetItem(nSheet - 1);
    PAtrRow AtrRow;
    AtrRow = ((*p->ListRows)[nRow]);


    int pokaz_id_Return = formLogic->insertPokaztlToF1Back(nSheet, nRowSel, isSub, InfoAlbumLogic.CurrPlanId, isItogo,
                                        pokaz_is_razd, AtrRow->ObjectId, in_cast, pokaz_name, Primecanie, is_list, level_sign, level_text,
                                        pokaz_vert, vert_str, pokaz_type, pokaz_code,pokaz_bk_code, pokaz_beg_dateTDateTime, pokaz_end_dateTDateTime,
                                        enum_id, edizm_id, pokaz_is_const, pokaz_is_god, pokaz_is_kv, pokaz_is_mes, pokaz_is_ist_fin, pokaz_is_din_izm);
    int user_id = GlobalVars->UserId;
    GlobalSessionManager->DeleteSession(TblSessionId);

    formLogic = new TFormLogicF1();
    formLogic->quForms = new TQueryUni(DefDBconnect.Conn);
    GlobalVars->UserId = user_id;
    formLogic->OpenForm(formObj.isCurForm, formObj.CurrAlbumId, InfoAlbumLogic, InfoFormLogic, 0, 0, "", formObj.FormId, formObj.FormOrder,
                        formObj.ownerFormPlanId);
    GlobalSessionManager->AddSessionData(TblSessionId, formLogic);
    nlohmann::json j;
    j["ActionView"]["Actions"] = nlohmann::json::array();
    nlohmann::json CurrElementJson;
    CurrElementJson = Json::parse(formLogic->CurrElementStr);

    nlohmann::json resXlsToStrJSON;
    formLogic->XlsToStrJSON(formLogic->f1Forms->Sheet, resXlsToStrJSON);

    CurrElementJson["FormJSON"] = resXlsToStrJSON;

    j["ActionView"]["Actions"].push_back(CurrElementJson);
    j["hdlprt"] = port;
    j["pokaz_id"] = pokaz_id_Return;

    return to_string(j);
}

std::string Spreadsheet::deletePokaztl(int port, std::string JsonString) {
    TInfoFormLogic InfoFormLogic;
    TInfoAlbumLogic InfoAlbumLogic;
    struct Form {
        bool isCurForm;
        int FormId;
        int CurrAlbumId;
        int IsParamForm;
        int ParamObjectId;
        std::string ParamObjectName;
        int FormOrder;
        int ownerFormPlanId;
    };
    std::string TblSessionId;
    int nSheet;
    int nRow;
    int nCol;
    auto json = nlohmann::json::parse(JsonString);

    TblSessionId = json.at("TblSessionId").get<std::string>();
    nSheet = json.at("nSheet").get<int>();
    nRow = json.at("nRow").get<int>();
    nCol = json.at("nCol").get<int>();

    Form formObj;

    for (const auto& formJson : json.at("forms")) {

        formObj.isCurForm = formJson.at("isCurForm").get<bool>();
        formObj.FormId = formJson.at("FormId").get<int>();
        formObj.CurrAlbumId = formJson.at("CurrAlbumId").get<int>();
        formObj.IsParamForm = formJson.at("IsParamForm").get<int>();
        formObj.ParamObjectId = formJson.at("ParamObjectId").get<int>();
        formObj.ParamObjectName = formJson.at("ParamObjectName").get<std::string>();
        formObj.FormOrder = formJson.at("FormOrder").get<int>();
        formObj.ownerFormPlanId = formJson.at("ownerFormPlanId").get<int>();
        const auto& infoAlbumLogicJson = formJson.at("InfoAlbumLogic");
        InfoAlbumLogic.CurrAlbumName = infoAlbumLogicJson.at("CurrAlbumName").get<std::string>();
        InfoAlbumLogic.accessalbum_idSelBlockSubj = infoAlbumLogicJson.at("accessalbum_idSelBlockSubj").get<int>();
        InfoAlbumLogic.CurrPlanId = infoAlbumLogicJson.at("CurrPlanId").get<int>();
        InfoAlbumLogic.CurrPlanName = infoAlbumLogicJson.at("CurrPlanName").get<std::string>();
        InfoAlbumLogic.CanEdit = infoAlbumLogicJson.at("CanEdit").get<bool>();
        InfoAlbumLogic.CurrVidOtch = infoAlbumLogicJson.at("CurrVidOtch").get<int>();
        InfoAlbumLogic.CurrYear = infoAlbumLogicJson.at("CurrYear").get<int>();
        InfoAlbumLogic.CurrQuarter = infoAlbumLogicJson.at("CurrQuarter").get<int>();
        InfoAlbumLogic.CurrMonth = infoAlbumLogicJson.at("CurrMonth").get<int>();
        string CurrDatews = infoAlbumLogicJson.at("CurrDate").get<std::string>();
        double currDateString = StrToFloat(CurrDatews);
        InfoAlbumLogic.CurrDate = UnixTimestampToDateTime(currDateString);
        InfoAlbumLogic.aShablonKBK = infoAlbumLogicJson.at("aShablonKBK").get<std::string>();
        InfoAlbumLogic.aId_Owner_link1n = infoAlbumLogicJson.at("aId_Owner_link1n").get<int>();

        const auto& infoFormLogicJson = formJson.at("InfoFormLogic");
        InfoFormLogic.isISTZNACHVVODPK = infoFormLogicJson.at("isISTZNACHVVODPK").get<bool>();
        InfoFormLogic.modeNumber = infoFormLogicJson.at("modeNumber").get<int>();
        string wsselectedDate = infoFormLogicJson.at("selectedDate").get<std::string>();
        double selectedDate = StrToFloat(wsselectedDate);
        InfoFormLogic.selectedDate = UnixTimestampToDateTime(selectedDate);
    }

    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }
    std::string msg;
    GlobalVars->idSelPlan = InfoAlbumLogic.CurrPlanId;
    bool result = formLogic->delDefRowByPokaz(nSheet, nRow, nCol, msg);
    int user_id = GlobalVars->UserId;
    GlobalSessionManager->DeleteSession(TblSessionId);

    formLogic = new TFormLogicF1();
    formLogic->quForms = new TQueryUni(DefDBconnect.Conn);
    GlobalVars->UserId = user_id;
    formLogic->OpenForm(formObj.isCurForm, formObj.CurrAlbumId, InfoAlbumLogic, InfoFormLogic, 0, 0, "", formObj.FormId, formObj.FormOrder,
                        formObj.ownerFormPlanId);
    GlobalSessionManager->AddSessionData(TblSessionId, formLogic);
    nlohmann::json jempty;

    nlohmann::json j;
    j["ActionView"]["Actions"] = nlohmann::json::array();
    nlohmann::json CurrElementJson;
    CurrElementJson = Json::parse(formLogic->CurrElementStr);
    nlohmann::json resXlsToStrJSON;
    formLogic->XlsToStrJSON(formLogic->f1Forms->Sheet, resXlsToStrJSON);

    CurrElementJson["FormJSON"] = resXlsToStrJSON;

    j["ActionView"]["Actions"].push_back(CurrElementJson);
    j["hdlprt"] = port;

    return to_string(j);
}

std::string Spreadsheet::changePokaztl(int port, std::string JsonString) {
    TQueryUni *qTmp = new TQueryUni(DefDBconnect.Conn);
    nlohmann::json json = nlohmann::json::parse(JsonString);

    struct Form {
        bool isCurForm;
        int FormId;
        int CurrAlbumId;
        int IsParamForm;
        int ParamObjectId;
        std::string ParamObjectName;
        int FormOrder;
        int ownerFormPlanId;
    };
    TInfoFormLogic InfoFormLogic;
    TInfoAlbumLogic InfoAlbumLogic;
    int pokaz_id = json.at("pokaz_id").get<int>();
//    int plan_id = json.at("plan_id").get<int>();
//    bool isItogo = json.at("isItogo").get<bool>();
//    int pokaz_is_razd = json.at("pokaz_is_razd").get<int>();
//    int pokaz_owner_id = json.at("pokaz_owner_id").get<int>();
//    int in_cast = json.at("in_cast").get<int>();
    std::string pokaz_name = json.at("pokaz_name").get<std::string>();
    std::string Primecanie = json.at("Primecanie").get<std::string>();
//    int is_list = json.at("is_list").get<int>();
//    int level_sign = json.at("level_sign").get<int>();
//    std::string level_text = json.at("level_text").get<std::string>();
//    int pokaz_vert = json.at("pokaz_vert").get<int>();
//    std::string vert_str = json.at("vert_str").get<std::string>();
//    int pokaz_type = json.at("pokaz_type").get<int>();
//    std::string pokaz_code = json.at("pokaz_code").get<std::string>();
//    std::string pokaz_bk_code = json.at("pokaz_bk_code").get<std::string>();
//    std::string pokaz_beg_date = json.at("pokaz_beg_date").get<std::string>();
//    std::string pokaz_end_date = json.at("pokaz_end_date").get<std::string>();
//    int enum_id = json.at("enum_id").get<int>();
//    int edizm_id = json.at("edizm_id").get<int>();
//    int pokaz_is_const = json.at("pokaz_is_const").get<int>();
//    int pokaz_is_god = json.at("pokaz_is_god").get<int>();
//    int pokaz_is_kv = json.at("pokaz_is_kv").get<int>();
//    int pokaz_is_mes = json.at("pokaz_is_mes").get<int>();
//    int pokaz_is_ist_fin = json.at("pokaz_is_ist_fin").get<int>();
//    int pokaz_is_din_izm = json.at("pokaz_is_din_izm").get<int>();
    std::string TblSessionId = json.at("TblSessionId").get<std::string>();
    int nSheet = json.at("nSheet").get<int>();
    int nRow = json.at("nRow").get<int>();
    int nCol = json.at("nCol").get<int>();


    const auto& formsArray = json.at("forms");
    Form formObj;
    for (const auto& form : formsArray) {

        formObj.isCurForm = form.at("isCurForm").get<bool>();
        formObj.FormId = form.at("FormId").get<int>();
        formObj.CurrAlbumId = form.at("CurrAlbumId").get<int>();
        formObj.IsParamForm = form.at("IsParamForm").get<int>();
        formObj.ParamObjectId = form.at("ParamObjectId").get<int>();
        formObj.ParamObjectName = form.at("ParamObjectName").get<std::string>();
        formObj.FormOrder = form.at("FormOrder").get<int>();
        formObj.ownerFormPlanId = form.at("ownerFormPlanId").get<int>();

        const auto& infoAlbumLogicJson = form.at("InfoAlbumLogic");
        InfoAlbumLogic.CurrAlbumName = infoAlbumLogicJson.at("CurrAlbumName").get<std::string>();
        InfoAlbumLogic.accessalbum_idSelBlockSubj = infoAlbumLogicJson.at("accessalbum_idSelBlockSubj").get<int>();
        InfoAlbumLogic.CurrPlanId = infoAlbumLogicJson.at("CurrPlanId").get<int>();
        InfoAlbumLogic.CurrPlanName = infoAlbumLogicJson.at("CurrPlanName").get<std::string>();
        InfoAlbumLogic.CanEdit = infoAlbumLogicJson.at("CanEdit").get<bool>();
        InfoAlbumLogic.CurrVidOtch = infoAlbumLogicJson.at("CurrVidOtch").get<int>();
        InfoAlbumLogic.CurrYear = infoAlbumLogicJson.at("CurrYear").get<int>();
        InfoAlbumLogic.CurrQuarter = infoAlbumLogicJson.at("CurrQuarter").get<int>();
        InfoAlbumLogic.CurrMonth = infoAlbumLogicJson.at("CurrMonth").get<int>();
//        InfoAlbumLogic.CurrDate = infoAlbumLogicJson.at("CurrDate").get<std::int>();
        string CurrDatews = infoAlbumLogicJson.at("CurrDate").get<std::string>();
        double currDateString = StrToFloat(CurrDatews);
        InfoAlbumLogic.CurrDate = UnixTimestampToDateTime(currDateString);
        InfoAlbumLogic.aShablonKBK = infoAlbumLogicJson.at("aShablonKBK").get<std::string>();
        InfoAlbumLogic.aId_Owner_link1n = infoAlbumLogicJson.at("aId_Owner_link1n").get<int>();

        const auto& infoFormLogicJson = form.at("InfoFormLogic");
        InfoFormLogic.isISTZNACHVVODPK = infoFormLogicJson.at("isISTZNACHVVODPK").get<bool>();
        InfoFormLogic.modeNumber = infoFormLogicJson.at("modeNumber").get<int>();
//        InfoFormLogic.selectedDate = infoFormLogicJson.at("selectedDate").get<std::string>();
        string wsselectedDate = infoFormLogicJson.at("selectedDate").get<std::string>();
        double selectedDate = StrToFloat(wsselectedDate);
        InfoFormLogic.selectedDate = UnixTimestampToDateTime(selectedDate);
    }

    int nRowSel = nRow;
    int isSub = 1;

    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }
    formLogic->quForms = qTmp;
//    TDateTime pokaz_beg_dateTDateTime = UnixTimestampToDateTime(StrToFloat(pokaz_beg_date));
//    TDateTime pokaz_end_dateTDateTime = UnixTimestampToDateTime(StrToFloat(pokaz_end_date));

    TwwStringList* p = formLogic->FormsValuesArray->GetItem(nSheet - 1);
    PAtrRow AtrRow;
    AtrRow = ((*p->ListRows)[nRow]);


//    int pokaz_id_Return = formLogic->insertPokaztlToF1Back(nSheet, nRowSel, isSub, InfoAlbumLogic.CurrPlanId, isItogo,
//                                                           pokaz_is_razd, AtrRow->ObjectId, in_cast, pokaz_name, Primecanie, is_list, level_sign, level_text,
//                                                           pokaz_vert, vert_str, pokaz_type, pokaz_code,pokaz_bk_code, pokaz_beg_dateTDateTime, pokaz_end_dateTDateTime,
//                                                           enum_id, edizm_id, pokaz_is_const, pokaz_is_god, pokaz_is_kv, pokaz_is_mes, pokaz_is_ist_fin, pokaz_is_din_izm);
    formLogic->UpdPokazMrprObj(pokaz_id, pokaz_name, Primecanie);
    int user_id = GlobalVars->UserId;
    GlobalSessionManager->DeleteSession(TblSessionId);

    formLogic = new TFormLogicF1();
    formLogic->quForms = new TQueryUni(DefDBconnect.Conn);
    GlobalVars->UserId = user_id;
    formLogic->OpenForm(formObj.isCurForm, formObj.CurrAlbumId, InfoAlbumLogic, InfoFormLogic, 0, 0, "", formObj.FormId, formObj.FormOrder,
                        formObj.ownerFormPlanId);
    GlobalSessionManager->AddSessionData(TblSessionId, formLogic);
    nlohmann::json j;
    j["ActionView"]["Actions"] = nlohmann::json::array();
    nlohmann::json CurrElementJson;
    CurrElementJson = Json::parse(formLogic->CurrElementStr);

    nlohmann::json resXlsToStrJSON;
    formLogic->XlsToStrJSON(formLogic->f1Forms->Sheet, resXlsToStrJSON);

    CurrElementJson["FormJSON"] = resXlsToStrJSON;

    j["ActionView"]["Actions"].push_back(CurrElementJson);
    j["hdlprt"] = port;
//    j["pokaz_id"] = pokaz_id_Return;

    return to_string(j);
}

std::string Spreadsheet::getPokaztl(std::string context_id, std::string TblSessionId, int nSheet, int nRow) {
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }
    TwwStringList* p = formLogic->FormsValuesArray->GetItem(nSheet - 1);
    PAtrRow AtrRow;
    AtrRow = ((*p->ListRows)[nRow]);
    int plan_id = 0;
    std::string plan_name =  TDB_PlanStr::getPlanNameByPokazId(AtrRow->ObjectId, plan_id);
    int plan_str_order = TDB_PlanStr::getOrderByPokazId(plan_id, AtrRow->ObjectId);
    bool isItogo = false;
    if (plan_str_order == 1000){
        isItogo = true;
    }
    int is_list = 0;
    TDB_Pokaztl::GetIs_List(AtrRow->ObjectId, is_list);
    TDateTime pokaz_beg_date;
    TDateTime pokaz_end_date;
    int level_sign = 0;
    TDB_Pokaztl::GetBegEndLevel(AtrRow->ObjectId, pokaz_beg_date, pokaz_end_date, level_sign);
    std::string pokaz_beg_dateUNIX = to_string(DateTimeToUnixTimestamp(pokaz_beg_date));
    std::string pokaz_end_dateUNIX = to_string(DateTimeToUnixTimestamp(pokaz_end_date));
    int pokaz_vert;
    std::string vert_str;
    TDB_Pokaztl::GetVert_Level(AtrRow->ObjectId, pokaz_vert, vert_str, level_sign);
    int edizm_id = 0;
    std::string edizm_id_str = TDB_Pokaztl::GetPokazName_EdIzm(AtrRow->ObjectId, edizm_id);
    int pokaz_is_mes;
    int pokaz_is_kv;
    int pokaz_is_god;
    int pokaz_is_ist_fin;
    int pokaz_is_din_izm;
    int pokaz_is_const;
    TDB_Pokaztl::GetOrtIzm(AtrRow->ObjectId, pokaz_is_mes, pokaz_is_kv, pokaz_is_god, pokaz_is_ist_fin, pokaz_is_din_izm, pokaz_is_const);

    nlohmann::json j;
    j["pokaz_id"] = AtrRow->ObjectId;
    j["plan_id"] = plan_id;
    j["isItogo"] = isItogo;
    j["pokaz_is_razd"] = TDB_Pokaztl::GetPokazIsRazd(AtrRow->ObjectId);
    j["pokaz_owner_id"] = TDB_Pokaztl::GetPokazOwnerId(AtrRow->ObjectId);
    j["in_cast"] = TDB_Pokaztl::GetInCast(AtrRow->ObjectId);
    j["pokaz_name"] = TDB_Pokaztl::GetPokazName(AtrRow->ObjectId);
    j["Primecanie"] = TDB_Pokaztl::GetPrimecanie(AtrRow->ObjectId);
    j["is_list"] = is_list;
    j["level_sign"] = level_sign;
    j["level_text"] = TDB_Pokaztl::GetLevelText(AtrRow->ObjectId);
    j["pokaz_vert"] = pokaz_vert;
    j["vert_str"] = vert_str;
    j["pokaz_type"] = TDB_Pokaztl::GetTypePokaz(AtrRow->ObjectId);
    j["pokaz_code"] = TDB_Pokaztl::GetPokazCode(AtrRow->ObjectId);
    j["pokaz_bk_code"] = TDB_Pokaztl::GetCodeBK(AtrRow->ObjectId);
    j["pokaz_beg_date"] = pokaz_beg_dateUNIX;
    j["pokaz_end_date"] = pokaz_end_dateUNIX;
    j["enum_id"] = TDB_Pokaztl::GetEnumId(AtrRow->ObjectId);
    j["edizm_id"] = edizm_id;
    j["pokaz_is_const"] = pokaz_is_const;
    j["pokaz_is_god"] = pokaz_is_god;
    j["pokaz_is_kv"] = pokaz_is_kv;
    j["pokaz_is_mes"] = pokaz_is_mes;
    j["pokaz_is_ist_fin"] = pokaz_is_ist_fin;
    j["pokaz_is_din_izm"] = pokaz_is_din_izm;

    return to_string(j);
}

std::string Spreadsheet::BudgetChange(std::string JsonString) {
    Json jsonObj = nlohmann::json::parse(JsonString);
    std::string TblSessionId = jsonObj.at("TblSessionId").get<std::string>();
    int nSheet = jsonObj.at("nSheet").get<int>();
    int nRow = jsonObj.at("nRow").get<int>();
    int nCol = jsonObj.at("nCol").get<int>();

    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }
    int rashodi = formLogic->f1Forms->NumberRC_Get(21, 10);
    int obiom = formLogic->f1Forms->NumberRC_Get(21, 9);
    nlohmann::json j = TUpdBudgPro2::F1Book1DblClickEvent(formLogic, nSheet, nRow, nCol);

    return to_string(j);
}

// Функция для чтения файла в память
std::vector<char> ReadFileToMemory(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Ошибка при открытии файла для чтения: " + filePath);
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return buffer;
}

std::string Spreadsheet::ExportInPDF(std::string TblSessionId, int nSheet) {
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }

    // Генерация 8 случайных символов
    std::string randomString = generateRandomString(8);
    std::string baseFileName = formLogic->CurrAlbumName;
    std::string uniqueFileName = baseFileName + "_" + randomString + ".pdf";

    std::u16string baseFileNameUTF16 = utf8_to_utf16(baseFileName);
    std::u16string authorUTF16 = utf8_to_utf16(GlobalVars->UserName);
    std::u16string subjectUTF16 = utf8_to_utf16("");
    std::u16string keywordsUTF16 = utf8_to_utf16("");
    std::u16string creatorUTF16 = utf8_to_utf16("");

    std::u16string fileName = utf8_to_utf16(uniqueFileName);
    const char16_t* fileName16 = fileName.c_str();

    std::u16string settingPDF = u"{\"title\":\"" + baseFileNameUTF16 +
                                u"\",\"author\":\"" + authorUTF16 +
                                u"\",\"subject\":\"" + subjectUTF16 +
                                u"\",\"keywords\":\"" + keywordsUTF16 +
                                u"\",\"creator\":\"" + creatorUTF16 + u"\"}";

    formLogic->f1Forms->SaveToPDF(fileName16, settingPDF.c_str());

    // Чтение содержимого файла
    std::ifstream file(uniqueFileName, std::ios::binary);
    if (!file.is_open()) {
        return ""; // Ошибка при открытии файла
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Кодирование содержимого файла в Base64
    std::string encodedContent;
    encodedContent.resize(boost::beast::detail::base64::encoded_size(buffer.size()));
    boost::beast::detail::base64::encode(&encodedContent[0], buffer.data(), buffer.size());

    // Удаление файла
    std::remove(uniqueFileName.c_str());

    // Создание JSON результата
    json JSONdocum;
    JSONdocum["fileName"] = baseFileName + ".pdf";
    JSONdocum["content"] = encodedContent;

    return JSONdocum.dump();
}

std::string Spreadsheet::ExportInXLS(std::string TblSessionId, int nSheet) {
    if (!GlobalSessionManager) {
        return ""; // Или любое другое обозначение ошибки
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
    // Попытка получить данные сессии. Проверяем результат выполнения метода.
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Сессия не найдена или другая ошибка
    }

    // Генерация 8 случайных символов
    std::string randomString = generateRandomString(8);
    std::string baseFileName = formLogic->CurrAlbumName;
    std::string uniqueFileName = baseFileName + "_" + randomString + ".xls";

    std::u16string fileName = utf8_to_utf16(uniqueFileName);
    const char16_t* fileName16 = fileName.c_str();
    formLogic->f1Forms->Save(fileName16);

    // Чтение содержимого файла
    std::ifstream file(uniqueFileName, std::ios::binary);
    if (!file.is_open()) {
        return ""; // Ошибка при открытии файла
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Кодирование содержимого файла в Base64
    std::string encodedContent;
    encodedContent.resize(boost::beast::detail::base64::encoded_size(buffer.size()));
    boost::beast::detail::base64::encode(&encodedContent[0], buffer.data(), buffer.size());

    // Удаление файла
    std::remove(uniqueFileName.c_str());

    // Создание JSON результата
    json JSONdocum;
    JSONdocum["fileName"] = baseFileName + ".xls";
    JSONdocum["content"] = encodedContent;

    return JSONdocum.dump();
}

std::string Spreadsheet::CellUpdateBatch(std::string JsonStringws) {
    // Парсим JSON строку
    nlohmann::json jsonObj = nlohmann::json::parse(JsonStringws);
    std::string TblSessionId = jsonObj["TblSessionId"];
    int nSheet = jsonObj["nSheet"];

    if (!GlobalSessionManager) {
        return ""; // Ошибка: сессия не найдена
    }

    TFormLogicF1* formLogic = nullptr; // Инициализация указателя
    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
        return ""; // Ошибка: сессия не найдена
    }

    // Создаем объект для логики обновления и вызываем метод CellsImport
    TFormDopUpdLogic dopUpdLogic;
    nlohmann::json resultJson;
    if (dopUpdLogic.CellsImport(formLogic, jsonObj, resultJson)){
        return to_string(resultJson);
    }

    return to_string(resultJson);
}

//std::string Spreadsheet::AllCellValueAtr(std::string TblSessionId, int nSheet) {
//    if (!GlobalSessionManager) {
//        return ""; // Или любое другое обозначение ошибки
//    }
//
//    TFormLogicF1* formLogic = nullptr; // Инициализация указателя нулевым значением
//    // Попытка получить данные сессии. Проверяем результат выполнения метода.
//    if (!GlobalSessionManager->GetSessionData(TblSessionId, formLogic) || !formLogic) {
//        return ""; // Сессия не найдена или другая ошибка
//    }
//
//    Json JSONresult;
//    formLogic->f1Forms->Sheet = nSheet;
//    formLogic->TblSessionId = TblSessionId;
//    TwwStringList* p = formLogic->FormsValuesArray->GetItem(nSheet - 1);
//    nlohmann::json JSONresultArr = nlohmann::json::array(); // Создаем массив для хранения всех результатов
//    int wi = formLogic->f1Forms->MaxCol;
//    for (int nRow = p->form_begBodyRow; nRow <= p->form_endBodyRow; ++nRow) {
//        for (int nCol = 0; nCol < formLogic->f1Forms->MaxCol; ++nCol) {
//            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(formLogic, nRow, nCol);
//            if (vd == nullptr){
//                continue;
//            }
//            nlohmann::json JSONresult; // Создаем JSONresult для каждой ячейки
//
//            // Вызываем функцию для получения значения ячейки и заполняем JSONresult
//            bool cellValueResult = formLogic->CellValueAtr(nSheet, nRow, nCol, JSONresult);
//
//            if (cellValueResult) {
//                // Добавляем результат в массив
//                JSONresultArr.push_back(JSONresult);
//            }
//        }
//    }
//    return to_string(JSONresultArr);
//}


std::string SpreadsheetAttr::AllCellValueAtr(std::string JsonStringws) {
    // Парсим входящий JSON
    nlohmann::json json;
    try {
        json = nlohmann::json::parse(JsonStringws);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Ошибка разбора: " << e.what() << std::endl;
        return ""; // Возврат в случае ошибки разбора
    }
    TQueryUni* qTmp = new TQueryUni(DefDBconnect.Conn);

    std::string sessionId = json["TblSessionId"];
    int nSheet = json["nSheet"].get<int>();
    nlohmann::json formsArray = json["forms"];

    TFormLogicF1* formLogic = nullptr;
    bool sessionExists = false;

    if (GlobalSessionManager) {
        sessionExists = GlobalSessionManager->GetSessionData(sessionId, formLogic);
    }

    // Если сессия не существует, создаем новую
    if (!sessionExists) {
        formLogic = new TFormLogicF1();
        formLogic->quForms = qTmp;

        // Получаем данные из формы и выполняем OpenForm
        for (auto& formItem : formsArray) {
            bool isCurForm = formItem["isCurForm"].get<bool>();
            int CurrAlbumId = formItem["CurrAlbumId"].get<int>();
            int FormId = formItem["FormId"].get<int>();
            int FormOrder = formItem["FormOrder"].get<int>();
            int ownerFormPlanId = formItem["ownerFormPlanId"].get<int>();

            TInfoAlbumLogic InfoAlbumLogic;
            auto infoAlbumLogicObject = formItem["InfoAlbumLogic"];

            InfoAlbumLogic.CurrAlbumName = infoAlbumLogicObject["CurrAlbumName"].get<std::string>();
            InfoAlbumLogic.accessalbum_idSelBlockSubj = infoAlbumLogicObject["accessalbum_idSelBlockSubj"].get<int>();
            InfoAlbumLogic.CurrPlanId = infoAlbumLogicObject["CurrPlanId"].get<int>();
            InfoAlbumLogic.CanEdit = infoAlbumLogicObject["CanEdit"].get<bool>();
            InfoAlbumLogic.CurrPlanName = infoAlbumLogicObject["CurrPlanName"].get<std::string>();
            InfoAlbumLogic.CurrVidOtch = infoAlbumLogicObject["CurrVidOtch"].get<int>();
            InfoAlbumLogic.CurrYear = infoAlbumLogicObject["CurrYear"].get<int>();
            InfoAlbumLogic.CurrQuarter = infoAlbumLogicObject["CurrQuarter"].get<int>();
            InfoAlbumLogic.CurrMonth = infoAlbumLogicObject["CurrMonth"].get<int>();

            std::string CurrDatews = infoAlbumLogicObject["CurrDate"].get<std::string>();
            double currDateString = StrToFloat(CurrDatews);
            InfoAlbumLogic.CurrDate = UnixTimestampToDateTime(currDateString);

            InfoAlbumLogic.aShablonKBK = infoAlbumLogicObject["aShablonKBK"].get<std::string>();
            InfoAlbumLogic.aId_Owner_link1n = infoAlbumLogicObject["aId_Owner_link1n"].get<int>();

            TInfoFormLogic InfoFormLogic;
            auto infoFormLogicObject = formItem["InfoFormLogic"];

            InfoFormLogic.isISTZNACHVVODPK = infoFormLogicObject["isISTZNACHVVODPK"].get<bool>();
            InfoFormLogic.modeNumber = infoFormLogicObject["modeNumber"].get<int>();
            std::string wsselectedDate = infoFormLogicObject["selectedDate"].get<std::string>();
            double selectedDate = StrToFloat(wsselectedDate);
            InfoFormLogic.selectedDate = UnixTimestampToDateTime(selectedDate);
            InfoFormLogic.isOtlogenPokaz = infoFormLogicObject["isOtlogenPokaz"].get<bool>();
            InfoFormLogic.OtlogenPokazId = infoFormLogicObject["OtlogenPokazId"].get<int>();

            formLogic->OpenForm(isCurForm, CurrAlbumId, InfoAlbumLogic, InfoFormLogic, 0, 0, "", FormId, FormOrder, ownerFormPlanId);
        }

        if (GlobalSessionManager) {
            GlobalSessionManager->AddSessionData(sessionId, formLogic);
        }
    } else {
        //MAK qTmp->Free();
        delete qTmp;
    }

    // Логика AllCellValueAtr для конкретного листа
    nlohmann::json JSONresultArr = nlohmann::json::array();

    formLogic->f1Forms->Sheet = nSheet;  // Устанавливаем текущий лист
    formLogic->TblSessionId = sessionId;

    TwwStringList* p = formLogic->FormsValuesArray->GetItem(nSheet - 1);
    for (int nRow = p->form_begBodyRow; nRow <= p->form_endBodyRow; ++nRow) {
        for (int nCol = 0; nCol < formLogic->f1Forms->MaxCol; ++nCol) {
            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(formLogic, nRow, nCol);
            if (vd == nullptr) {
                continue;
            }
            TQueryUni* qTmp2 = new TQueryUni(DefDBconnect.Conn);
            formLogic->quForms = qTmp2;
            nlohmann::json JSONresult; // Создаем JSONresult для каждой ячейки
            bool cellValueResult = formLogic->CellValueAtr(nSheet, nRow, nCol, JSONresult);
            //MAK qTmp2->Free();
            delete qTmp2;
            if (cellValueResult) {
                JSONresultArr.push_back(JSONresult); // Добавляем результат в массив
            }
        }
    }

    // Если сессия была создана в этой функции, удаляем formLogic
    if (!sessionExists) {
        if (GlobalSessionManager) {
            GlobalSessionManager->DeleteSession(sessionId);
        }
        //delete formLogic;
    }

    return to_string(JSONresultArr);
}



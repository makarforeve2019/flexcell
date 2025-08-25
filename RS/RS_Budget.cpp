//
// Created by Alex on 26.06.2024.
//

#include "RS_Budget.h"
#include "json.hpp"
#include "FormTypes.h"
#include "unFormLogicF1.h"
#include "DB_Album.h"
#include "FormDopInp.h"
#include "SystFuncsLogic.h"
#include "UpdDBbyValue.h"
#include "DB_Budget.h"
#include "DB_User.h"
#include "FormDopUpdLogic.h"

//std::string Budget::AcceptChanges(std::string JsonString, boost::beast::http::status& status) {
//    try {
//        nlohmann::json jsonObj = nlohmann::json::parse(JsonString);
//        int grnlUpdBP_id = jsonObj.at("grnlUpdBP_id").get<int>();
//        int plan_id = jsonObj.at("plan_id").get<int>();
//        int user_id = jsonObj.at("context_id").get<int>();
//
//
//
//        TQueryUni *q2 = new TQueryUni(DefDBconnect.Conn);
//        q2->SQL->Clear();
//        q2->SQL->Add("SELECT * FROM grnlupdbudgpro WHERE grnlupdbp_id = :grnlupdbp_id");
//        q2->ParamByName("grnlupdbp_id")->AsInteger() = grnlUpdBP_id;
//        q2->Open();
//        int CurrYear = q2->FieldByName("god")->AsInteger();
//        if (q2->FieldByName("sost")->AsInteger() != 2){
//            status = http::status::bad_request;
//            return "����������� �� � ��������� ������������";
//        }
//        nlohmann::json JSONresult;
//        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
//        q->SQL->Clear();
//        q->SQL->Add("SELECT * FROM doupdbudgpro WHERE grnlupdbp_id = :grnlupdbp_id");
//        q->SQL->Add("ORDER BY form_id ASC");
//        q->ParamByName("grnlupdbp_id")->AsInteger() = grnlUpdBP_id;
//        q->Open();
//        int form_idThis=-1;
//        TFormLogicF1* formLogic;
//        while (!q->EoF()) {
//            if (form_idThis != q->FieldByName("form_id")->AsInteger()){
//                TQueryUni *qTmp = new TQueryUni(DefDBconnect.Conn);
//                formLogic = nullptr;
//                formLogic = new TFormLogicF1();
//                formLogic->quForms = qTmp;
//
//                int idGrn = q->FieldByName("doupdbp_id")->AsInteger();
//
//                bool isCurForm = true;
//                int FormId = q->FieldByName("form_id")->AsInteger();
//                form_idThis = q->FieldByName("form_id")->AsInteger();
//                int CurrAlbumId = q->FieldByName("album_id")->AsInteger();
//                int IsParamForm = 0;
//                int ParamObjectId = 0;
//                std::string ParamObjectName = "";
//                int FormOrder = 1;
//                int ownerFormPlanId = 0;
//                TInfoAlbumLogic InfoAlbumLogic;
//                TInfoFormLogic InfoFormLogic;
//
//
//                // ��������������� ���������� ���������� ��� �������� �����������
//                int album_vid_func, album_isbp, readOnly, plan_id_fiks, vid_deistv, album_typ, isYearQuartMonth, YearFixed, isUpdDateSave;
//                std::string album_obozn, JSONFiltr;
//
//                // ����� ������� ��� ��������� ���������� �� �������
//                TDB_Album::GetAlbumInfoYQM(q->FieldByName("album_id")->AsInteger(), album_vid_func, album_isbp, readOnly, album_obozn,
//                                           plan_id_fiks, vid_deistv, album_typ, isYearQuartMonth, YearFixed, JSONFiltr);
//
//                InfoAlbumLogic.CurrAlbumName = album_obozn;
//                InfoAlbumLogic.accessalbum_idSelBlockSubj = 0;
//                InfoAlbumLogic.CurrPlanId = plan_id;
//                InfoAlbumLogic.CurrPlanName = "";
//                InfoAlbumLogic.CanEdit = true;
//                InfoAlbumLogic.CurrVidOtch = album_vid_func;
//                InfoAlbumLogic.CurrYear = CurrYear;
//                InfoAlbumLogic.CurrQuarter = 0;
//                InfoAlbumLogic.CurrMonth = 0;
//                InfoAlbumLogic.CurrDate = Now();
//                InfoAlbumLogic.aShablonKBK = "";
//                InfoAlbumLogic.aId_Owner_link1n = 0;
//
//                InfoFormLogic.isISTZNACHVVODPK = false;
//                InfoFormLogic.modeNumber = 0;
//                InfoFormLogic.selectedDate = 0;
//
//
//                formLogic->OpenForm(isCurForm, CurrAlbumId, InfoAlbumLogic, InfoFormLogic, 0, 0, "", FormId, FormOrder,
//                                    ownerFormPlanId);
//            }
//
//            PCellValueAtr vd1;
//            int nRow = q->FieldByName("nrow")->AsInteger();
//            int nCol = q->FieldByName("ncol")->AsInteger();
//            vd1 = TFormDopInp::GetCellValueAtrByRC(formLogic, nRow, nCol);
//            double wi1 = q->FieldByName("deltavaluestoim")->AsDouble();
//            double wi2 = q->FieldByName("cellvaluestoim")->AsDouble();
//            double valued = q->FieldByName("deltavaluestoim")->AsDouble() + q->FieldByName("cellvaluestoim")->AsDouble();
//            double valuedPreb = PreobrValue(valued, vd1->pokaz_type, vd1->coef_Pokaz, vd1->coef_Forma);
////            TUpdDBbyValue::UpdDBbyValue3(vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate, q->FieldByName("deltavaluestoim")->AsInteger() + q->FieldByName("cellvaluestoim")->AsInteger(), vd1->ObjectId, false, false);
//            formLogic->f1Forms->NumberRC_Set(nRow, nCol, valuedPreb);
//            TUpdDBbyValue::UpdDBbyValue1(vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate, q->FieldByName("deltavaluestoim")->AsDouble() + q->FieldByName("cellvaluestoim")->AsDouble(), vd1->ObjectId, false, JSONresult);
//            if (q->FieldByName("deltavaluenatur")->AsDouble() != 0) {
//                int nRow = q->FieldByName("nrow")->AsInteger();
//                int nColnatur = q->FieldByName("ncolnatur")->AsInteger();
//                vd1 = TFormDopInp::GetCellValueAtrByRC(formLogic, nRow, nColnatur);
//                valued = q->FieldByName("deltavaluenatur")->AsDouble() + q->FieldByName("cellvaluenatur")->AsDouble();
//                valuedPreb = PreobrValue(valued, vd1->pokaz_type, vd1->coef_Pokaz, vd1->coef_Forma);
//                formLogic->f1Forms->NumberRC_Set(nRow, nColnatur, valuedPreb);
//                //TUpdDBbyValue::UpdDBbyValue3(vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate, q->FieldByName("deltavaluenatur")->AsInteger() + q->FieldByName("cellvaluenatur")->AsInteger(), vd1->ObjectId, false, false);
//                TUpdDBbyValue::UpdDBbyValue1(vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate, q->FieldByName("deltavaluenatur")->AsDouble() + q->FieldByName("cellvaluenatur")->AsDouble(), vd1->ObjectId, false, JSONresult);
//            }
//            q->Next();
//            int form_idNext = -1;
//            if (!q->EoF()) {
//                form_idNext = q->FieldByName("form_id")->AsInteger();
//            } else {
//                TFormDopUpdLogic::doForPrisvoitUpd(formLogic, JSONresult);
//            }
//            if (form_idThis != form_idNext){
//                TFormDopUpdLogic::doForPrisvoitUpd(formLogic, JSONresult);
//                delete formLogic;
//            }
//
//        }
//        delete q;
//
//
//
//        q2->SQL->Clear();
//        q2->SQL->Add("UPDATE GrnlUpdBudgPro");
//        q2->SQL->Add("SET dateSostPri = :dateSostPri, sost=:sost, userPrinjl_id = :userPrinjl_id");
//        q2->SQL->Add("WHERE grnlUpdBP_id = :grnlUpdBP_id");
//        q2->ParamByName("grnlUpdBP_id")->AsInteger() = grnlUpdBP_id;
//        q2->ParamByName("sost")->AsInteger() = 3;
//        q2->ParamByName("dateSostPri")->AsDateTime() = Now();
//        q2->ParamByName("userPrinjl_id")->AsInteger() = user_id;
//        q2->ExecSQL();
//
//        delete q2;
//
//        return "200";
//    }    catch (const std::exception& e) {
//        status = http::status::bad_request;
//        return "";
//    }
//}

std::string Budget::AcceptChanges(std::string JsonString, boost::beast::http::status& status) {
    try {
        nlohmann::json jsonObj = nlohmann::json::parse(JsonString);
        int grnlUpdBP_id = jsonObj.at("grnlUpdBP_id").get<int>();
        int plan_id = jsonObj.at("plan_id").get<int>();
        int user_id = jsonObj.at("context_id").get<int>();

        TQueryUni* q2 = new TQueryUni(DefDBconnect.Conn);
        q2->SQL->Clear();
        q2->SQL->Add("SELECT * FROM grnlupdbudgpro WHERE grnlupdbp_id = :grnlupdbp_id");
        q2->ParamByName("grnlupdbp_id")->AsInteger() = grnlUpdBP_id;
        q2->Open();

        int CurrYear = q2->FieldByName("god")->AsInteger();
        if (q2->FieldByName("sost")->AsInteger() != 2) {
            status = http::status::bad_request;
            delete q2;
            return "Состояние записи не соответствует требованиям";
        }

        nlohmann::json JSONresult;

        TQueryUni* q = new TQueryUni(DefDBconnect.Conn);
        q->SQL->Clear();
        q->SQL->Add("SELECT * FROM doupdbudgpro WHERE grnlupdbp_id = :grnlupdbp_id");
        q->SQL->Add("ORDER BY form_id ASC");
        q->ParamByName("grnlupdbp_id")->AsInteger() = grnlUpdBP_id;
        q->Open();

        int form_idThis = -1;
        TFormLogicF1* formLogic = nullptr;

        while (!q->EoF()) {
            if (form_idThis != q->FieldByName("form_id")->AsInteger()) {
                if (formLogic) {
                    TFormDopUpdLogic::doForPrisvoitUpd(formLogic, JSONresult);
                    delete formLogic;
                    formLogic = nullptr;
                }

                TQueryUni* qTmp = new TQueryUni(DefDBconnect.Conn);
                formLogic = new TFormLogicF1();
                formLogic->quForms = qTmp;

                int idGrn = q->FieldByName("doupdbp_id")->AsInteger();
                bool isCurForm = true;
                int FormId = q->FieldByName("form_id")->AsInteger();
                form_idThis = q->FieldByName("form_id")->AsInteger();
                int CurrAlbumId = q->FieldByName("album_id")->AsInteger();
                int IsParamForm = 0;
                int ParamObjectId = 0;
                std::string ParamObjectName = "";
                int FormOrder = 1;
                int ownerFormPlanId = 0;
                TInfoAlbumLogic InfoAlbumLogic;
                TInfoFormLogic InfoFormLogic;

                int album_vid_func, album_isbp, readOnly, plan_id_fiks, vid_deistv, album_typ, isYearQuartMonth, YearFixed, isUpdDateSave;
                std::string album_obozn, JSONFiltr;

                TDB_Album::GetAlbumInfoYQM(q->FieldByName("album_id")->AsInteger(), album_vid_func, album_isbp, readOnly, album_obozn,
                                           plan_id_fiks, vid_deistv, album_typ, isYearQuartMonth, YearFixed, JSONFiltr);

                InfoAlbumLogic.CurrAlbumName = album_obozn;
                InfoAlbumLogic.accessalbum_idSelBlockSubj = 0;
                InfoAlbumLogic.CurrPlanId = plan_id;
                InfoAlbumLogic.CurrPlanName = "";
                InfoAlbumLogic.CanEdit = true;
                InfoAlbumLogic.CurrVidOtch = album_vid_func;
                InfoAlbumLogic.CurrYear = CurrYear;
                InfoAlbumLogic.CurrQuarter = 0;
                InfoAlbumLogic.CurrMonth = 0;
                InfoAlbumLogic.CurrDate = Now();
                InfoAlbumLogic.aShablonKBK = "";
                InfoAlbumLogic.aId_Owner_link1n = 0;

                InfoFormLogic.isISTZNACHVVODPK = false;
                InfoFormLogic.modeNumber = 0;
                InfoFormLogic.selectedDate = 0;

                formLogic->OpenForm(isCurForm, CurrAlbumId, InfoAlbumLogic, InfoFormLogic, 0, 0, "", FormId, FormOrder, ownerFormPlanId);
            }

            PCellValueAtr vd1;
            int nRow = q->FieldByName("nrow")->AsInteger();
            int nCol = q->FieldByName("ncol")->AsInteger();
            vd1 = TFormDopInp::GetCellValueAtrByRC(formLogic, nRow, nCol);
            double wi1 = q->FieldByName("deltavaluestoim")->AsDouble();
            double wi2 = q->FieldByName("cellvaluestoim")->AsDouble();
            double valued = q->FieldByName("deltavaluestoim")->AsDouble() + q->FieldByName("cellvaluestoim")->AsDouble();
            double valuedPreb = PreobrValue(valued, vd1->pokaz_type, vd1->coef_Pokaz, vd1->coef_Forma);
            formLogic->f1Forms->NumberRC_Set(nRow, nCol, valuedPreb);
            TUpdDBbyValue::UpdDBbyValue1(vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate, valued, vd1->ObjectId, false, JSONresult);
            if (q->FieldByName("deltavaluenatur")->AsDouble() != 0) {
                int nColnatur = q->FieldByName("ncolnatur")->AsInteger();
                vd1 = TFormDopInp::GetCellValueAtrByRC(formLogic, nRow, nColnatur);
                valued = q->FieldByName("deltavaluenatur")->AsDouble() + q->FieldByName("cellvaluenatur")->AsDouble();
                valuedPreb = PreobrValue(valued, vd1->pokaz_type, vd1->coef_Pokaz, vd1->coef_Forma);
                formLogic->f1Forms->NumberRC_Set(nRow, nColnatur, valuedPreb);
                TUpdDBbyValue::UpdDBbyValue1(vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate, valued, vd1->ObjectId, false, JSONresult);
            }

            q->Next();

            int form_idNext = -1;
            if (!q->EoF()) {
                form_idNext = q->FieldByName("form_id")->AsInteger();
            } else {
                TFormDopUpdLogic::doForPrisvoitUpd(formLogic, JSONresult);
            }
            if (form_idThis != form_idNext) {
                TFormDopUpdLogic::doForPrisvoitUpd(formLogic, JSONresult);
                delete formLogic;
                formLogic = nullptr;
            }
        }

        delete q;

        q2->SQL->Clear();
        q2->SQL->Add("UPDATE GrnlUpdBudgPro");
        q2->SQL->Add("SET dateSostPri = :dateSostPri, sost=:sost, userPrinjl_id = :userPrinjl_id");
        q2->SQL->Add("WHERE grnlUpdBP_id = :grnlUpdBP_id");
        q2->ParamByName("grnlUpdBP_id")->AsInteger() = grnlUpdBP_id;
        q2->ParamByName("sost")->AsInteger() = 3;
        q2->ParamByName("dateSostPri")->AsDateTime() = Now();
        q2->ParamByName("userPrinjl_id")->AsInteger() = user_id;
        q2->ExecSQL();

        delete q2;

        return "200";
    } catch (const std::exception& e) {
        status = http::status::bad_request;
        return "";
    }
}


std::string Budget::Proposals(int Year, int plan_id, int user_id, int sost, bool is_orderby, OrderBy proposals_orderby) {
    std::string result = "";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        json contentArray = json::array();

        if (TDB_Budget::GetProposalsAll(Year, plan_id, user_id, sost, is_orderby, proposals_orderby, &qTmp)) {
            while (!qTmp.EoF()) {
                json formObject;
                formObject["grnlupdbp_id"] = qTmp.FieldByName("grnlupdbp_id")->AsInteger();
                formObject["regnumber"] = qTmp.FieldByName("regnumber")->AsString();
                formObject["regdate"] = DateTimeToStr(qTmp.FieldByName("regdate")->AsDateTime());
                formObject["note"] = qTmp.FieldByName("note")->AsString();
                formObject["god"] = qTmp.FieldByName("god")->AsInteger();
                formObject["plan_id"] = qTmp.FieldByName("plan_id")->AsInteger();
                formObject["userispoln_id"] = qTmp.FieldByName("userispoln_id")->AsInteger();
                formObject["userprinjl_id"] = qTmp.FieldByName("userprinjl_id")->AsInteger();
                formObject["vidpredlog"] = qTmp.FieldByName("vidpredlog")->AsInteger();
                formObject["sost"] = qTmp.FieldByName("sost")->AsInteger();
                formObject["noteotkl"] = qTmp.FieldByName("noteotkl")->AsString();
                formObject["datesostred"] = DateTimeToStr(qTmp.FieldByName("datesostred")->AsDateTime());
                formObject["datesostras"] = DateTimeToStr(qTmp.FieldByName("datesostras")->AsDateTime());
                formObject["datesostpri"] = DateTimeToStr(qTmp.FieldByName("datesostpri")->AsDateTime());
                formObject["datesostotk"] = DateTimeToStr(qTmp.FieldByName("datesostotk")->AsDateTime());
                formObject["docum_id"] = qTmp.FieldByName("docum_id")->AsString();
                formObject["podvidpereraspr"] = qTmp.FieldByName("podvidpereraspr")->AsInteger();
                formObject["regdate_leshoz"] = DateTimeToStr(qTmp.FieldByName("regdate_leshoz")->AsDateTime());
                formObject["regnumber_leshoz"] = qTmp.FieldByName("regnumber_leshoz")->AsString();
                formObject["date_finupr"] = DateTimeToStr(qTmp.FieldByName("date_finupr")->AsDateTime());
                formObject["date_ozlupr"] = DateTimeToStr(qTmp.FieldByName("date_ozlupr")->AsDateTime());
                formObject["date_proffice"] = DateTimeToStr(qTmp.FieldByName("date_proffice")->AsDateTime());
                formObject["sost_finupr"] = qTmp.FieldByName("sost_finupr")->AsInteger();
                formObject["sost_ozlupr"] = qTmp.FieldByName("sost_ozlupr")->AsInteger();
                formObject["sost_proffice"] = qTmp.FieldByName("sost_proffice")->AsInteger();
                formObject["usersogl_ozlupr"] = qTmp.FieldByName("usersogl_ozlupr")->AsInteger();
                formObject["usersogl_proffice"] = qTmp.FieldByName("usersogl_proffice")->AsInteger();
                formObject["userPrinjl_name"] = qTmp.FieldByName("userPrinjl_name")->AsString();
                formObject["plan_name"] = qTmp.FieldByName("plan_name")->AsString();
                formObject["approval_data"] = qTmp.FieldByName("approval_data")->AsString();

                std::string approvalData = qTmp.FieldByName("approval_data")->AsString();

                int countSogl;
                std::string ws = Budget::getAllInfoZayavki(qTmp.FieldByName("grnlupdbp_id")->AsInteger(), countSogl, approvalData);
                if (countSogl == 0) {
                    formObject["approval_descr"] = "";

                } else {
                    if (countSogl == 1 && qTmp.FieldByName("sost").AsInteger() == 3) {
                        formObject["approval_descr"] = ws;
                    } else {
                        formObject["approval_descr"] = "Коллегиальное согласование.\n" + ws;
                    }
                }
                contentArray.push_back(formObject);
                qTmp.Next();
            }
        }

        result = contentArray.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}


const int colUpr = 4; // Установите соответствующее значение количества элементов
std::array<std::string, colUpr> naim_uprArr = {
        "Финансовое управление: Согласовано",
        "Управление защиты и воспроизводства лесов: Согласовано",
        "Управление охраны лесов от пожаров: Согласовано",
        "Проектный Офис: Согласовано"
};

std::string Budget::getAllInfoZayavki(int grnlUpdBP_id, int &countSogl, std::string approvalData) {
    std::string result = "";

    size_t wsIn = approvalData.find("[{");
    size_t wsOut = approvalData.find("}]") + 2;

    if (wsOut - 2 == std::string::npos) {
        result = "";
        countSogl = 0;
    } else {
        std::string wsJSONString = approvalData.substr(wsIn, wsOut - wsIn);
        nlohmann::json patternJ = nlohmann::json::parse(wsJSONString);
        countSogl = patternJ.size();

        std::array<std::string, colUpr> allInfoArr = {};

        for (size_t i = 0; i < patternJ.size(); ++i) {
            nlohmann::json jo = patternJ[i];
            for (int num = 1; num <= colUpr; ++num) {
                std::string numStr = std::to_string(num);
                if (jo.contains(numStr)) {
                    nlohmann::json patternJT = jo;
                    std::string wsDate = patternJT["D"].get<std::string>();
                    int wsUserId;
                    if (patternJT["U"].is_string()) {
                        wsUserId = StrToInt(patternJT["U"].get<std::string>());
                    } else if (patternJT["U"].is_number()) {
                        wsUserId = patternJT["U"].get<int>();
                    }
                    std::string wsUserName = TDB_User::GetUserName(wsUserId);
                    allInfoArr[num - 1] = naim_uprArr[num - 1] + ", " + wsDate + ", " + wsUserName;
                }
            }
        }

        for (const auto &info : allInfoArr) {
            if (!info.empty()) {
                result += info + "\n";
            }
        }
    }
    return result;
}


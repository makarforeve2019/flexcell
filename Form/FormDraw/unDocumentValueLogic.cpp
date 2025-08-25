#include "unDocumentValueLogic.h"
#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"

//получкние docum_id из Cell_Value
// Возвращает false, если docum_id равно Null
bool TDocumentValueLogic::GetDocumid(int ObjectId, int CellId, int PlanId, int VidOtch, const TDateTime ValueDate, std::string& Docum_id) {
    try {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        q->SQL->Clear();
        q->SQL->Add("SELECT docum_id FROM Cell_value");
        q->SQL->Add("WHERE objectid = :objectid");
        q->SQL->Add("AND cell_id = :cell_id AND plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
        q->ParamByName("objectid")->AsInteger() = ObjectId;
        q->ParamByName("cell_id")->AsInteger() = CellId;
        q->ParamByName("plan_id")->AsInteger() = PlanId;
        q->ParamByName("vid_otc")->AsInteger() = VidOtch;
        q->ParamByName("value_date")->AsDateTime() = ValueDate;
        q->Open();
        if (q->EoF()) {
            delete q;
            return false;
        }
        if (q->FieldByName("docum_id")->IsNull()) {
            delete q;
            return false;
        } else {
            Docum_id = q->FieldByName("docum_id")->AsString();
            delete q;
            return true;
        }
    }
    catch (const std::exception& e) {
          std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
          return false;
    }
}

bool TDocumentValueLogic::GetDocumid(std::string& Docum_id, int PokazId, int ComnpId, int VidPeriod, int God, int Kvrtl, int Mesjc, int Den, int PlanId, int VidOtch) {
    try {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        q->SQL->Clear();
        q->SQL->Add("SELECT docum_id FROM cnp_value");
        q->SQL->Add("WHERE pokaz_id = :pokaz_id AND comnp_id = :comnp_id");
        q->SQL->Add("AND vid_period = :vid_period AND plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = :vid_otc AND god  = :god");
        q->SQL->Add("AND kvrtl = :kvrtl AND mesjc  = :mesjc");
        q->SQL->Add("AND den = :den");
        q->ParamByName("pokaz_id")->AsInteger() = PokazId;
        q->ParamByName("comnp_id")->AsInteger() = ComnpId;
        q->ParamByName("vid_period")->AsInteger() = VidPeriod;
        q->ParamByName("plan_id")->AsInteger() = PlanId;
        q->ParamByName("vid_otc")->AsInteger() = VidOtch;
        q->ParamByName("god")->AsInteger() = God;
        q->ParamByName("kvrtl")->AsInteger() = Kvrtl;
        q->ParamByName("mesjc")->AsInteger() = Mesjc;
        q->ParamByName("den")->AsInteger() = Den;
        q->Open();
        if (q->EoF()) {
            delete q;
            return false;
        }
        if (q->FieldByName("docum_id")->IsNull()) {
            delete q;
            return false;
        } else {
            Docum_id = q->FieldByName("docum_id")->AsString();
            delete q;
            return true;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        return false;
    }
}

bool TDocumentValueLogic::GetDocumRelation(std::string Docum_id,
                             int& ObjectId, int& CellId, int& PlanId, int& VidOtch, TDateTime& ValueDate,
                             int& PokazId, int& ComnpId, int& VidPeriod, int& God, int& Kvrtl, int& Mesjc, int& Den) {
    bool result = true;
    try {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        q->SQL->Clear();
        q->SQL->Add("SELECT * FROM cnp_value");
        q->SQL->Add("WHERE docum_id = :docum_id");
        q->ParamByName("docum_id")->AsString() = Docum_id;
        q->Open();
        if (!q->EoF()) {
            PokazId = q->FieldByName("pokaz_id")->AsInteger();
            ComnpId = q->FieldByName("comnp_id")->AsInteger();
            VidPeriod = q->FieldByName("vid_period")->AsInteger();
            PlanId = q->FieldByName("plan_id")->AsInteger();
            VidOtch = q->FieldByName("vid_otc")->AsInteger();
            God = q->FieldByName("god")->AsInteger();
            Kvrtl = q->FieldByName("kvrtl")->AsInteger();
            Mesjc = q->FieldByName("mesjc")->AsInteger();
            Den = q->FieldByName("den")->AsInteger();
        }
        else {
            q->SQL->Clear();
            q->SQL->Add("SELECT * FROM Cell_value");
            q->SQL->Add("WHERE docum_id = :docum_id");
            q->ParamByName("docum_id")->AsString() = Docum_id;
            q->Open();
            if (!q->EoF()) {
                ObjectId = q->FieldByName("objectid")->AsInteger();
                CellId = q->FieldByName("cell_id")->AsInteger();
                PlanId = q->FieldByName("plan_id")->AsInteger();
                VidOtch = q->FieldByName("vid_otc")->AsInteger();
                ValueDate = q->FieldByName("value_date")->AsDateTime();
            }
            else
                result = false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        return false;
    }
    return result;
}

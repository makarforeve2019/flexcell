//
//

#include "DB_Budget.h"

bool TDB_Budget::GetProposalsAll(int Year, int plan_id, int user_id, int sost, bool is_orderby, OrderBy proposals_orderby, TQueryUni* qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT g.*, u.user_name as userPrinjl_name, p.plan_name as plan_name ");
    qIn->SQL->Add("FROM GrnlUpdBudgPro as g LEFT OUTER JOIN users as u on u.user_id = g.userispoln_id ");
    qIn->SQL->Add("LEFT OUTER JOIN plans as p on p.plan_id = g.plan_id ");
    qIn->SQL->Add("WHERE g.god = :god");
    //qIn->SQL->Add("SELECT * FROM GrnlUpdBudgPro");
    //qIn->SQL->Add("WHERE god = :god");
    if (plan_id)
        qIn->SQL->Add("AND g.plan_id = :plan_id ");
    if (user_id)
        qIn->SQL->Add("AND g.userispoln_id = :user_id ");
    if (sost)
        qIn->SQL->Add("AND g.sost = :sost ");
    if (is_orderby) {
        qIn->SQL->Add("ORDER BY g.plan_id, " + proposals_orderby.fieldname + " " + proposals_orderby.order);
    }
    else {
        qIn->SQL->Add("ORDER BY g.plan_id, g.grnlupdbp_id");
    }
    qIn->ParamByName("god")->AsInteger() = Year;
    if (plan_id)
        qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    if (user_id)
        qIn->ParamByName("user_id")->AsInteger() = user_id;
    if (sost)
        qIn->ParamByName("sost")->AsInteger() = sost;
    qIn->Open();

    if (qIn->EoF()) {
        return false;
    }
    return true;
}

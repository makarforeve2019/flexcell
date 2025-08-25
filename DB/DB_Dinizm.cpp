//
// Created by Alex on 16.06.2024.
//

#include "DB_Dinizm.h"

void TDB_Dinizm::GetPokazDinizm(TQueryUni *qIn, int pokaz_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Pokaz_din_izm a, Din_izm b");
    qIn->SQL->Add("WHERE pokaz_id = :pokaz_id");
    qIn->SQL->Add("AND a.din_izm_id = b.din_izm_id");
    qIn->SQL->Add("ORDER BY a.din_izm_id");
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->Open();
}

void TDB_Dinizm::GetAllGrad_dinizm(TQueryUni *qIn, int din_izm_owner_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Din_izm");
    qIn->SQL->Add("WHERE din_izm_owner_id = :pokaz_id");
    qIn->SQL->Add("ORDER BY din_izm_order");
    qIn->ParamByName("din_izm_owner_id")->AsInteger() = din_izm_owner_id;
    qIn->Open();
}

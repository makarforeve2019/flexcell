//
// Created by Alex on 30.04.2024.
//



#include "DB_SystSettings.h"
#include "unDefDBconnect.h"


std::string TDB_SystSettings::GetValyeByOboznac(std::string Oboznac) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT Value, recId");
    q->SQL->Add("FROM SystSettings");
    q->SQL->Add("WHERE Oboznac = :Oboznac");
    q->ParamByName("Oboznac")->AsString() = Oboznac;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("Value")->AsString();
    }
    q->Close();
    delete q; // Освободите память после использования
    return Result;
}
std::string AppSpace = "";

std::string TDB_SystSettings::GetAppSpace() {
    if (AppSpace.empty()) {
        AppSpace = TDB_SystSettings::GetValyeByOboznac("AppSpace");
    }
    return AppSpace;
}

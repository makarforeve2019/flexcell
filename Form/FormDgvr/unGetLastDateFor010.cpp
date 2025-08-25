#ifndef unGetLastDateFor010CPP
#define unGetLastDateFor010CPP


#include "ClassesUtils.h"
#include "DB_Common.h"
#include "unDefDBconnect.h"
#include "SystFuncsLogic.h"


static TDateTime GetLastDateFor010(int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);
    try {

         TDateTime dt1, dt2;

        q->SQL->Clear();
        q->SQL->Add("SELECT MAX(dateImpFCU) as dateImpFCU");
        q->SQL->Add("FROM ispolnKBKistor");
        switch (GetDBType()) {
            case TDBType::MSSQL:
                q->SQL->Add("WHERE Year(dateImpFCU) = :god");
                break;
            case TDBType::Postgre:
                q->SQL->Add("WHERE date_part('year', dateImpFCU) = :god");
                break;
        }
        q->ParamByName("god")->AsInteger() = god;
        q->Open();

        if (!q->EoF())
            dt1 = q->FieldByName("dateImpFCU")->AsDateTime();
        else
            dt1 = 0;
        q->Close();

        q->SQL->Clear();
        q->SQL->Add("SELECT MAX(dateImpFCUend) as dateImpFCU");
        q->SQL->Add("FROM ispolnKBKistor");
        switch (GetDBType()) {
            case TDBType::MSSQL:
                q->SQL->Add("WHERE Year(dateImpFCU) = :god");
                break;
            case TDBType::Postgre:
                q->SQL->Add("WHERE date_part('year', dateImpFCU) = :god");
                break;
        }
        q->ParamByName("god")->AsInteger() = god;
        q->Open();

        if (!q->EoF())
            dt2 = q->FieldByName("dateImpFCU")->AsDateTime();
        else
            dt2 = 0;
        q->Close();

        TDateTime result = dt1;
        if (dt2 > dt1) result = dt2;
        result = DateOf(result);

        delete q;
        //vernut  TDB_MWSModuls::CloseLinkMWSModuls();
        return result;
    } catch (...) {
        delete q;
        //vernut  TDB_MWSModuls::CloseLinkMWSModuls();
        throw;
    }
}
#endif
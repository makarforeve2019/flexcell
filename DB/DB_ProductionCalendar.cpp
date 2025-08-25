#include "../SystFuncs/SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_ProductionCalendar.h"



bool TDB_ProductionCalendar::isWorkday(TDateTime data) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM ProductCalendar");
    q->SQL->Add("WHERE god = :god AND mesjc = :mesjc AND den = :den");
    q->ParamByName("god")->AsInteger() = YearOf(data);
    q->ParamByName("mesjc")->AsInteger() = MonthOf(data);
    q->ParamByName("den")->AsInteger() = DayOf(data);
    q->Open();
    bool result = q->EoF();
    delete q;
    return result;
}

int TDB_ProductionCalendar::WorkdaysCountBetweenDates(TDateTime earlyDate, TDateTime laterDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (earlyDate > laterDate) {
        TDateTime tempDate = earlyDate;
        earlyDate = laterDate;
        laterDate = tempDate;
    }
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM ProductCalendar");
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            q->SQL->Add("WHERE CONVERT(DATETIME, CONVERT(varchar(2), den) + '.' + CONVERT(varchar(2), mesjc) + '.' + CONVERT(varchar(4), god), 103) >= :data1");
            q->SQL->Add("AND CONVERT(DATETIME, CONVERT(varchar(2), den) + '.' + CONVERT(varchar(2), mesjc) + '.' + CONVERT(varchar(4), god), 103) <= :data2");
            break;
        }
        case TDBType::Postgre: {
            q->SQL->Add("WHERE to_timestamp((cast(den as text) || ' ' || cast(mesjc as text) || ' ' || cast(god as text)), 'DD MM YYYY') >= :data1");
            q->SQL->Add("AND to_timestamp((cast(den as text) || ' ' || cast(mesjc as text) || ' ' || cast(god as text)), 'DD MM YYYY') <= :data2");
            break;
        }
    }
    q->ParamByName("data1")->AsDateTime() = earlyDate;
    q->ParamByName("data2")->AsDateTime() = laterDate;
    q->Open();
    int result = DaysBetween(laterDate, earlyDate) + 2;
    while (!q->EoF()) {
        result -= 1;
        q->Next();
    }
    delete q;
    return result;
}

int TDB_ProductionCalendar::WorkdaysCountInMonth(int year, int month) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM ProductCalendar");
    q->SQL->Add("WHERE god = :god AND mesjc = :mesjc");
    q->ParamByName("god")->AsInteger() = year;
    q->ParamByName("mesjc")->AsInteger() = month;
    q->Open();
    int result = (q->EoF()) ? -1 : DaysInAMonth(year, month) - q->RecordCount();
    delete q;
    return result;
}

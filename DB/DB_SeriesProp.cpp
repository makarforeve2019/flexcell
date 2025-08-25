#include "DB_SeriesProp.h"
#include "unDefDBconnect.h"

void TDB_SeriesProp::DelAllProps(int series_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM SeriesProp");
    q->SQL->Add("WHERE series_id = :series_id");
    q->ParamByName("series_id")->AsInteger() = series_id;
    q->ExecSQL();
    delete q;
}

void TDB_SeriesProp::AddProp(int series_id, int order_nr, int color, std::string title,
    int ttl_Row, int ttl_Col, int fnc_Row, int fnc_Col) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO SeriesProp");
    q->SQL->Add("(series_id, order_nr, color, title, ttl_Row, ttl_Col, fnc_Row, fnc_Col)");
    q->SQL->Add("VALUES (:series_id, :order_nr, :color, :title, :ttl_Row, :ttl_Col, :fnc_Row, :fnc_Col)");
    q->ParamByName("series_id")->AsInteger() = series_id;
    q->ParamByName("order_nr")->AsInteger() = order_nr;
    q->ParamByName("color")->AsInteger() = color;
    q->ParamByName("title")->AsString() = title;
    q->ParamByName("ttl_Row")->AsInteger() = ttl_Row;
    q->ParamByName("ttl_Col")->AsInteger() = ttl_Col;
    q->ParamByName("fnc_Row")->AsInteger() = fnc_Row;
    q->ParamByName("fnc_Col")->AsInteger() = fnc_Col;
    q->ExecSQL();
    delete q;
}

void TDB_SeriesProp::GetAllProps(TQueryUni *qIn, int series_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM SeriesProp");
    qIn->SQL->Add("WHERE series_id = :series_id");
    qIn->ParamByName("series_id")->AsInteger() = series_id;
    qIn->Open();
}

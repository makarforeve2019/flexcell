#include "DB_Series.h"
#include "unDefDBconnect.h"
#include "DB_SeriesProp.h"

int TDB_Series::AddSeries(int chart_id, int vid, std::string title, int ttl_Row, int ttl_Col) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Series");
    q->SQL->Add("(chart_id, vid, title, ttl_Row, ttl_Col)");
    q->SQL->Add("VALUES (:chart_id, :vid, :title, :ttl_Row, :ttl_Col)");
    q->ParamByName("chart_id")->AsInteger() = chart_id;
    q->ParamByName("vid")->AsInteger() = vid;
    q->ParamByName("title")->AsString() = title;
    q->ParamByName("ttl_Row")->AsInteger() = ttl_Row;
    q->ParamByName("ttl_Col")->AsInteger() = ttl_Col;
    q->ExecSQL();
    
    int series_id = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(series_id) as series_id");
    q->SQL->Add("FROM Series");
    q->SQL->Add("WHERE chart_id = :chart_id");
    q->ParamByName("chart_id")->AsInteger() = chart_id;
    q->Open();
    if (!q->EoF()) {
        series_id = q->FieldByName("series_id")->AsInteger();
    }
    q->Close();
    
   //MAK q->Free();
delete q;
    
    return series_id;
}

void TDB_Series::UpdTitle(int series_id, std::string title, int ttl_Row, int ttl_Col) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Series");
    q->SQL->Add("SET title = :title, ttl_Row = :ttl_Row, ttl_Col = :ttl_Col");
    q->SQL->Add("WHERE series_id = :series_id");
    q->ParamByName("series_id")->AsInteger() = series_id;
    q->ParamByName("title")->AsString() = title;
    q->ParamByName("ttl_Row")->AsInteger() = ttl_Row;
    q->ParamByName("ttl_Col")->AsInteger() = ttl_Col;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_Series::UpdVid(int series_id, int vid) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Series");
    q->SQL->Add("SET vid = :vid");
    q->SQL->Add("WHERE series_id = :series_id");
    q->ParamByName("series_id")->AsInteger() = series_id;
    q->ParamByName("vid")->AsInteger() = vid;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_Series::UpdProp(int series_id, int kol_otdeln_tocek, int from_Row, int from_Col,
    int to_Row, int to_Col, int is_marks, int is_point, int SeriesColor, int marka_tocnost,
    int is_other, int style) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Series");
    q->SQL->Add("SET kol_otdeln_tocek = :kol_otdeln_tocek, from_Row = :from_Row,");
    q->SQL->Add("from_Col = :from_Col, to_Row = :to_Row, to_Col = :to_Col,");
    q->SQL->Add("is_marks = :is_marks, is_point = :is_point, SeriesColor = :SeriesColor, marka_tocnost = :marka_tocnost,");
    q->SQL->Add("is_other = :is_other, style = :style");
    q->SQL->Add("WHERE series_id = :series_id");
    
    q->ParamByName("series_id")->AsInteger() = series_id;
    q->ParamByName("kol_otdeln_tocek")->AsInteger() = kol_otdeln_tocek;
    q->ParamByName("from_Row")->AsInteger() = from_Row;
    q->ParamByName("from_Col")->AsInteger() = from_Col;
    q->ParamByName("to_Row")->AsInteger() = to_Row;
    q->ParamByName("to_Col")->AsInteger() = to_Col;
    q->ParamByName("is_marks")->AsInteger() = is_marks;
    q->ParamByName("is_point")->AsInteger() = is_point;
    q->ParamByName("SeriesColor")->AsInteger() = SeriesColor;
    q->ParamByName("marka_tocnost")->AsInteger() = marka_tocnost;
    q->ParamByName("is_other")->AsInteger() = is_other;
    q->ParamByName("style")->AsInteger() = style;
    
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Series::GetSeries(TQueryUni *qIn, int series_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Series");
    qIn->SQL->Add("WHERE series_id = :series_id");
    qIn->ParamByName("series_id")->AsInteger() = series_id;
    qIn->Open();
}

void TDB_Series::GetAllSeries(TQueryUni *qIn, int chart_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Series");
    qIn->SQL->Add("WHERE chart_id = :chart_id");
    qIn->ParamByName("chart_id")->AsInteger() = chart_id;
    qIn->Open();
}

void TDB_Series::DelSeries(int series_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_SeriesProp::DelAllProps(series_id);
    
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Series");
    q->SQL->Add("WHERE series_id = :series_id");
    q->ParamByName("series_id")->AsInteger() = series_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

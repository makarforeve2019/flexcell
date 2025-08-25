#ifndef DB_SeriesH
#define DB_SeriesH


#include "DB_Common.h"
#include <string>

class TDB_Series {
public:
    static int AddSeries(int chart_id, int vid, std::string title, int ttl_Row, int ttl_Col);
    static void UpdTitle(int series_id, std::string title, int ttl_Row, int ttl_Col);
    static void UpdVid(int series_id, int vid);
    static void UpdProp(int series_id, int kol_otdeln_tocek, int from_Row, int from_Col,
       int to_Row, int to_Col, int is_marks, int is_point, int SeriesColor, int marka_tocnost,
       int is_other, int style);
    static void GetSeries(TQueryUni *qIn, int series_id);
    static void GetAllSeries(TQueryUni *qIn, int chart_id);
    static void DelSeries(int series_id);
};

#endif

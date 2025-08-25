#ifndef DB_SeriesPropH
#define DB_SeriesPropH

#include "DB_Common.h"

class TDB_SeriesProp {
public:
    // Удалить все свойства для серии series_id
    static void DelAllProps(int series_id);

    // Добавить свойство серии
    static void AddProp(int series_id, int order_nr, int color, std::string title,
        int ttl_Row, int ttl_Col, int fnc_Row, int fnc_Col);

    // Прочитать все свойства для серии series_id
    static void GetAllProps(TQueryUni *qIn, int series_id);
};

#endif

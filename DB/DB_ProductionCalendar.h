#ifndef DB_ProductionCalendarH
#define DB_ProductionCalendarH


#include "../system/ClassesUtils.h"
#include "DB_Common.h"

class TDB_ProductionCalendar {
public:
    // Рабочий ли день
    static bool isWorkday(TDateTime data);

    // Количество рабочих дней в месяце
    static int WorkdaysCountInMonth(int year, int month);

    // Количество рабочих дней в промежутке
    static int WorkdaysCountBetweenDates(TDateTime earlyDate, TDateTime laterDate);
};

#endif

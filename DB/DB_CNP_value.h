#ifndef DB_CNP_VALUE_H
#define DB_CNP_VALUE_H

#include <string>
#include "DB_Common.h"


class TDB_CNP_value {
public:
    // удаляет все значения из CNP_value для мероприятия pokaz_id
    static void DelItemsForPokaz(int pokaz_id);
    // Читает все данные для мероприятия/объекта заданного субъекта
    static void GetDataForObject(TQueryUni *qIn, int pokaz_id, int plan_id);
    // ввод/модификация числового значения
    static void addNumberItem(int pokaz_id, int comnp_id, int vid_period, int plan_id,
        int vid_otc, short god, int kvrtl, int mesjc, short den, double value);
    // читает числовое значение
    static bool getNumberItem(int pokaz_id, int comnp_id, int vid_period, int plan_id,
        int vid_otc, short god, int kvrtl, int mesjc, short den, double &value);
    // вычисляет числовое значение - Сумма по всем субъектам
    static bool getNumberItemallPlans(int pokaz_id, int comnp_id, int vid_period, int plan_id,
        int vid_otc, short god, int kvrtl, int mesjc, short den, double &value);
    // ввод/модификация строкового значения
    static void addStringItem(int pokaz_id, int comnp_id, int vid_period, int plan_id,
        int vid_otc, short god, int kvrtl, int mesjc, short den, const std::string &str_value);
    // читает строковое значение
    static bool getStringItem(int pokaz_id, int comnp_id, int vid_period, int plan_id,
        int vid_otc, short god, int kvrtl, int mesjc, short den, std::string &str_value);
    // удаляет только значение без всяких других изменений
    static void DelOnlyValue(int pokaz_id, int comnp_id, int vid_period, int plan_id,
        int vid_otc, short god, int kvrtl, int mesjc, short den);
    // Записывает дату последнего изменения ячейки а так же пользователя
    static void addNote(const std::string &Note, int pokaz_id, int comnp_id, int vid_period, int plan_id,
        int vid_otc, short god, int kvrtl, int mesjc, short den);
    //Читает дату последнего изменения ячейки а так же пользователя
    static std::string GetNote(int pokaz_id, int comnp_id, int vid_period, int plan_id,
        int vid_otc, short god, int kvrtl, int mesjc, short den);
    //Читает docum_id, если null то возвращает false
    static bool getDocumId(int pokaz_id, int comnp_id, int vid_period, int plan_id,
        int vid_otc, short god, int kvrtl, int mesjc, short den, std::string &DocumId);
};

#endif // DB_CNP_VALUE_H

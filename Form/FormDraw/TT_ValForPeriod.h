#ifndef TT_VALFORPERIOD_H
#define TT_VALFORPERIOD_H

#include "DB_Common.h"
#include "dmGlobalVars.h"

class TValForPeriod {
public:
    static TStringList* sl;
    static void CreateObjValForPeriod(); // создает объект класса TValForPeriod
    static void FreeObjValForPeriod(); // освобождает объект класса TValForPeriod
    static std::string CreateKey(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id);
    // добавить запись
    void Addrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                 int cell_id, double value);
    // вычислить, добавить запись для детальной строки и вернуть значение - уточненный план
    double Val_Addrcrd_Det4(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                            int cell_id_Mes);
    // вычислить, добавить запись для детальной строки и вернуть значение - факт
    double Val_Addrcrd_Det5(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                            int cell_id);
    // вычислить, добавить запись для групповой строки "в том числе" и вернуть значение
    double Val_Addrcrd_VtomCisle(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                                 int cell_id);
    // вычислить, добавить запись для строки "Итого по разделу" и вернуть значение
    double Val_Addrcrd_Itogo(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                             int cell_id);
    // организовать вычисления подчиненных и вернуть значение
    double Val_forOwner(int plan_id, int objectid, int pokaz_owner_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id);
    // вернуть числового значение записи
    double Getrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id, int cell_id,
                   int pokaz_is_razd,  int pokaz_vert);
    // Специально для графика финансирования - рассчет факта к началу периода
    static double valFactToBegPeriod(int plan_id, int pokaz_id, int god, int nMesjc, int day);
    // Специально для графика финансирования - рассчет факта за квартал на дату
    static double valFactKvNaDatu(int plan_id, int pokaz_id, int god, int nMesjc, int day, int kv);
};

#endif

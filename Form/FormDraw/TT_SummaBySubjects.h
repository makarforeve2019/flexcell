#ifndef TT_SUMMABYSUBJECTS_H
#define TT_SUMMABYSUBJECTS_H


#include <string>
#include "../DB/DB_Common.h"
#include "../DB/unDBType.h"

class TSummaBySubjects {
public:
    TStringList* sl;
    static void CreateObjSummaBySubjects(); // создает объект класса TSummaBySubjects
    static void FreeObjSummaBySubjects(); // освобождает объект класса TSummaBySubjects
    // дать список субъектов на год
    static std::string getPlanIds(int god);
    static std::string CreateKey(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day);

    // вычислить годовую сумму по всем субъектам и вернуть
    double Val_SumToGodKv(int cell_id, int vid_otc, TDateTime ValueDate);

    // добавить запись
    void Addrcrd(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                 int cell_id, double value);
    // вычислить, добавить запись для детальной строки и вернуть значение - уточненный план
    double Val_Addrcrd_Det4(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                             int cell_id_Mes);
    // вычислить, добавить запись для детальной строки и вернуть значение - факт
    double Val_Addrcrd_Det5(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                             int cell_id);
    // вычислить, добавить запись для групповой строки "в том числе" и вернуть значение
    double Val_Addrcrd_VtomCisle(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                                  int cell_id);
    // организовать вычисления подчиненных и вернуть значение
    double Val_forOwner(int objectid, int pokaz_owner_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id);

    // вернуть числового значение записи
    double Getrcrd(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day,
                   int ist_fin_id, int cell_id, int pokaz_is_razd, int pokaz_vert);
};

//struct TrcrdFM {
//    int objectid;
//    int pokaz_id;
//    int vid_otc;
//    int god;
//    int nMesjc;
//    int day;
//    int ist_fin_id; // ist_fin_id по Cell:константы ist_fin_id_CBRTOkruga, ist_fin_id_TrBudg определены в DB_IstFin
//    int cell_id;
//    double valueUtTB;
//    double valueFcTB;
//};

//typedef TrcrdFM* PrcrdFM;

#endif // TT_SUMMABYSUBJECTS_H

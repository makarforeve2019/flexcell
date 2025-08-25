#ifndef DB_NAT_POKAZ_VID_H
#define DB_NAT_POKAZ_VID_H

#include <string>
#include "DB_Common.h"


class TDB_Nat_Pokaz_Vid {
public:
    // чтение всех натуральных видов показателей
    static void GetNPVAll(TQueryUni* qIn);
    // чтение всей информации про np_vid_id
    static void GetNPV(TQueryUni* qIn, int np_vid_id);
    // чтение наименования про np_vid_id
    static std::string GetNPVName(int np_vid_id);
    // чтение enum_id
    static int GetEnum_id(int np_vid_id);
    // чтение edizm_id
    static int GetEdizm_id(int np_vid_id);
    // выбор np_vid_id с заданным наименованием
    static int GetNPVId(std::string np_vid_name);
    // удаление элемента
    static void DelItem(int np_vid_id);
    // изменяем наименование
    static void UpdName(std::string np_vid_name, int np_vid_id);
    // изменяем enum_id
    static void UpdEnum_id(int enum_id, int np_vid_id);
    // изменяем edizm_id
    static void UpdEdizm_id(int edizm_id, int np_vid_id);
    // вставляем элемени
    static int InsertItem(std::string np_vid_name, int np_vid_type, int enum_id, int edizm_id);
};

#endif // DB_NAT_POKAZ_VID_H

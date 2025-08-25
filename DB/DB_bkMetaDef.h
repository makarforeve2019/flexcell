
#ifndef DB_BKMETADEF_H
#define DB_BKMETADEF_H

#include "DB_Common.h"

class TDB_bkMetaDef {
public:
    // дать имя по metadef_id
    static std::string GetNameVidById(int metadef_id);

    // дать вид по metadef_id
    static int GetVidById(int metadef_id);

    // дать нужную подстроку из КБК для вида
    static std::string GetSubKBKByVid(std::string fullKBK, int vid, int God);

    // дать Раздел из КБК
    static std::string GetRasdel(std::string fullKBK, int God);

    // дать Целевую статью из КБК
    static std::string GetCelSt(std::string fullKBK, int God);

    // дать Вид расходов из КБК
    static std::string GetVidRasx(std::string fullKBK, int God);

    // дать КОСГУ из КБК
    static std::string GetKOSGU(std::string fullKBK, int God);

    // получает всю информацию про все элементы
    static void GetDefAll(TQueryUni *qIn);

    // добавляет элемент
    static void AddDef(int vid, int nacalo_vKBK, int dlina_vKBK, int beg_God, int end_God);

    // модифицирует элемент
    static void ModDef(int metadef_id, int nacalo_vKBK, int dlina_vKBK, int beg_God, int end_God);

    // удаляет элемент
    static void DelDef(int metadef_id);

    // дать metadef_id0 по vid
    static int Get0(int vid);

    // дать новый metadef_id по vid
    static int Getmetadef_id(int vid);

    // дать информацию про metadef_id
    static void GetDefInfo(TQueryUni* qIn, int metadef_id);

    // дать информацию про vid
    static bool GetVidInfo(int vid, int& nacalo_vKBK, int& dlina_vKBK, int curGod);

    // дать длину Раздел+Целевая статья
    static int GetDl_R_CS(int curGod);

    // дать длину Целевая статья
    static int GetDl_CS(int curGod);

    // дать Общую длину
    static int GetDl_All(int curGod);

    // дать длину функциональной классификации
    static int GetDl_R_CS_VR(int curGod);
};

//struct TVidInfoBKMETADEF {
//    int vid;
//    int beg_God;
//    int end_God;
//    int nacalo_vKBK;
//    int dlina_vKBK;
//};
//
//std::array<TVidInfoBKMETADEF, 100> arVidInfo;

static bool isLoad;

static int Nkol;

#endif // DB_BKMETADEF_H

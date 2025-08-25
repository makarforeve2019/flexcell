#ifndef DB_TMPPOKAZTL_H
#define DB_TMPPOKAZTL_H




#include "DB_Common.h"
#include <string>

static TStringList nameCreateTmpByKOSGU;

class TDB_TmpPokaztl {
public:
    // создать временную таблицу #TmpPokaztl
    static void CreateTmpPokaztl(int CurrYear, int root_Pokaz_id, int plan_id);

    // удалить временную таблицу #TmpPokaztl
    static void DropTmpPokaztl();

    // создать саму временную таблицу #TmpPokaztl
    static void CreateTmpTblPokaztl(int aPokaz_id, int aPlan_id, TDateTime aToDate);

    // создать временную таблицу для выборки только КОСГУ
    // имя таблицы передается параметром aTblName
    static void CreateTmpByKOSGU(int isFilter, std::string aTblName, int CurrYear, int root_Pokaz_id);

    // добавить строки для Программы, включая подпрограммы
    static void addPrgrm(std::string aTblName, std::string shablon, int god, int isOpen);

    // добавить строки для Подпрограммы
    static void addSubPrgrm(std::string aTblName, std::string shablon, int god, int isOpen);

    // добавить одну строку с заданным кодом (Программы или подпрограммы)
    static void addCelStatj(std::string aTblName, std::string shablon, int god, int pokaz_is_razd);

    // создать временную таблицу с порядком по целевым программам
    // имя таблицы передается параметром aTblName
    static void CreateTmpOrderCP(std::string aTblName, int CurrYear, int root_Pokaz_id);

    // создать саму временную таблицу #PTB
    static void CreateTmpPTB(int aPokaz_id, TDateTime aToDate);

    // создать саму временную таблицу для остального PTB
    static void CreateTmpPTBanypg(int aPokaz_id, TDateTime aToDate);

    // здесь идет разузлование по принципу в том числе и любые другие
    static void CreateTmpPTBany(int aPokaz_id, TDateTime aToDate);

    // удалить все временные таблицы
    static bool DropAllTmpOrderCP();

    // удалить временную таблицу
    // имя таблицы передается параметром aTblName
    static void DropTmpOrderCP(std::string aTblName);

    static void DropTmpByKOSGUall();

    // удалить временную таблицу для выборки только КОСГУ
    // имя таблицы передается параметром aTblName
    static void DropTmpByKOSGU(std::string aTblName);
};

#endif
